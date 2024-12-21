#include <fstream>
#include <numeric>
#include <nlohmann/json.hpp>
#include <tensorflow/c/c_api.h>
#include "model.h"

using json = nlohmann::json;


// Constructor
// ====================================
// Main function
// ------------------------------------
tf2::model::model(
  const std::string inpfile
) {
  // Parse inputs from json file
  this->parse_inputs(inpfile);
  // Set global TF context
  if (this->config != nullptr) {
    this->set_global_context(*this->config);
  }
  // Initialize TensorFlow model
  this->tfmodel = std::unique_ptr<cppflow::model>(
    new cppflow::model(this->path_to_model)
  );
  // Get input/output operations
  this->get_ops_info();
  // Check inputs/outputs operations
  this->inputs_dim = this->check_io_ops(this->inputs_id);
  this->outputs_dim = this->check_io_ops(this->outputs_id);
  // Inputs/outputs total dimensions
  this->inp_tot_dim = std::accumulate(
    this->inputs_dim.begin(), this->inputs_dim.end(), 0
  );
  this->out_tot_dim = std::accumulate(
    this->outputs_dim.begin(), this->outputs_dim.end(), 0
  );
  // Check if batched inference is allowed
  if (this->batch_size > 0) {
    bool c1 = (this->inputs_dim.size() > 1);
    bool c2 = (this->outputs_dim.size() > 1);
    if (c1 || c2) {
      std::ostringstream message;
      message << "\nFrom tf2::model::model():"
              << "\n> Multi-input-multi-output models are "
              << "currently not supported for batched inference.";
      throw std::runtime_error(message.str());
    }
  }
}

// Util functions
// ------------------------------------
void tf2::model::parse_inputs(
  const std::string inpfile
) {
  // Initialize JSON object reader
  std::ifstream file(inpfile);
  json inputs = json::parse(file);
  // Environmental variables
  if (inputs.contains("env")) {
    for (const auto& item : inputs["env"].items()) {
      const std::string k = item.key();
      const std::string v = item.value();
      setenv(k.c_str(), v.c_str(), 1);
    }
  }
  // Extract inputs
  this->path_to_model = inputs["path_to_model"];
  this->inputs_id = inputs["inputs_id"];
  this->outputs_id = inputs["outputs_id"];
  this->rowmajor = inputs.value("rowmajor", this->rowmajor);
  this->batch_size = inputs.value("batch_size", this->batch_size);
  if (inputs.contains("config")) {
    std::vector<std::string> cfg = inputs["config"];
    this->config = &cfg;
  }
};

void tf2::model::set_global_context(
  const std::vector<std::string>& config
) {
  // Create new options with the new configuration
  TFE_ContextOptions *opts = TFE_NewContextOptions();
  TFE_ContextOptionsSetConfig(
    opts, config.data(), config.size(), cppflow::context::get_status()
  );
  // Replace the global context with user options
  cppflow::get_global_context() = cppflow::context(opts);
};

void tf2::model::get_ops_info() {
  // Get operations identifiers
  std::vector<std::string> ops_id = this->tfmodel->get_operations();
  // Store identifier-shape pairs
  for (const auto &op_id : ops_id) {
    bool c1 = (op_id.rfind(this->inputs_id_prefix, 0) == 0);
    bool c2 = (op_id.rfind(this->outputs_id_prefix, 0) == 0);
    if (c1 || c2) {
      auto shape = this->tfmodel->get_operation_shape(op_id);
      this->ops.emplace_back(op_id, shape);
    }
  }
}

std::vector<std::int32_t> tf2::model::check_io_ops(
  const std::vector<std::string> identifiers
) {
  std::vector<std::int64_t> dim;
  for (const auto &i_id : identifiers) {
    bool found = false;
    for (const auto &op : this->ops) {
      const std::string op_id = std::get<0>(op);
      if (i_id.find(op_id) != std::string::npos) {
        for (auto i = 0; i < this->max_nb_io; ++i) {
          if (i_id == op_id + ":" + std::to_string(i)) {
            dim.push_back(std::get<1>(op)[1]);
            found = true;
            break;
          }
        }
      }
      if (found) break;
    }
    if (!found) {
      std::ostringstream message;
      message << "\nFrom tf2::model::check_io_ops():"
              << "\n> Operation identifier '" << i_id << "' not found!"
              << "\n> Available options are:";
      for (const auto &op : this->ops) {
        message << "\n> - " << std::get<0>(op) << ":<i>";
      }
      throw std::runtime_error(message.str());
    }
  }
  // Recast
  std::vector<std::int32_t> _dim(dim.begin(), dim.end());
  return _dim;
};

// Inputs/Outputs manipulations
// ====================================
template <typename T>
tf2::data::list_tuple_tensor tf2::model::compose_inputs(
  const std::vector<T>& inputs,
  const std::int32_t nb_pts
) {
  // Get the number of inputs (single-/multi-inputs)
  const std::size_t nb_inp = this->inputs_dim.size();
  tf2::data::list_tuple_tensor x;
  // Loop over inputs
  std::int32_t delta, start = 0;
  for (std::size_t i = 0; i < nb_inp; ++i) {
    // > Extract i-th input
    delta = this->inputs_dim[i] * nb_pts;
    std::vector<T> xi(inputs.begin() + start, inputs.begin() + start + delta);
    // > If column-major, reorder the input
    if (!this->rowmajor) {
      tf2::ops::invert_major<T>(xi, this->inputs_dim[i], nb_pts, true);
    }
    // Convert std::vector to cppflow::tensor with the proper shape
    cppflow::tensor xi_tf(xi, {nb_pts, this->inputs_dim[i]});
    // Move cppflow::tensor to GPU if CUDA available
    auto on_gpu = xi_tf.device(true).find("GPU");
    if ((this->tfmodel->is_cuda_available) && (on_gpu != std::string::npos)) {
      xi_tf = cppflow::bitcast(xi_tf, xi_tf.dtype());
    }
    // Make and append the tuple for i-th input
    x.emplace_back(this->inputs_id[i], std::move(xi_tf));
    // > Update the counter
    start += delta;
  }
  return x;
}

template <typename T>
void tf2::model::compose_outputs(
  std::vector<T>& outputs,
  const std::vector<cppflow::tensor>& tf_outputs,
  const std::int32_t nb_pts
) {
  // Get the number of outputs (single-/multi-outputs)
  const std::size_t nb_out = this->outputs_dim.size();
  // Loop over outputs
  std::size_t index = 0;
  for (std::size_t i = 0; i < nb_out; ++i) {
    // Extract data from i-th output (row-major ordering)
    std::vector<T> yi = tf_outputs[i].get_data<T>();
    // > If column-major, reorder the input
    if (!this->rowmajor) {
      tf2::ops::invert_major<T>(yi, nb_pts, this->outputs_dim[i], false);
    }
    // Move to outputs
    std::move(yi.begin(), yi.end(), outputs.begin() + index);
    index += yi.size();
  }
}

// Calling
// ====================================
template <typename T>
void tf2::model::evaluate(
  const std::vector<T>& inputs,
  std::vector<T>& outputs,
  const std::int32_t nb_pts
) {
  // Inputs manipulation
  auto x = tf2::model::compose_inputs<T>(inputs, nb_pts);
  // Perform inference
  auto y = (*this->tfmodel)(x, this->outputs_id);
  // Outputs manipulation
  tf2::model::compose_outputs<T>(outputs, y, nb_pts);
}

template <typename T>
void tf2::model::call(
  const std::vector<T>& inputs,
  std::vector<T>& outputs,
  const std::int32_t nb_pts
) {
  if ((this->batch_size < 1) || (this->batch_size > nb_pts)) {
    this->evaluate<T>(inputs, outputs, nb_pts);
  } else {
    if (this->rowmajor) {
      // Initialize inputs and outputs start/end indices
      std::int32_t is = 0, ie = 0, os = 0;
      // Calculate the number of batches
      std::int32_t size = this->batch_size;
      std::int32_t nb_batches = nb_pts / size;
      std::int32_t last_batch_size = nb_pts % size;
      if (last_batch_size > 0) {
        nb_batches += 1;
      }
      // Initialize batched outputs
      std::vector<T> yb(size * this->out_tot_dim);
      // Loop over batches and perform inference
      for (std::int32_t i = 0; i < nb_batches; ++i) {
        if ((last_batch_size > 0) && (i == nb_batches - 1)) {
          size = last_batch_size;
          yb.resize(size * this->out_tot_dim);
        }
        // Extract the batched inputs
        ie += size * this->inp_tot_dim;
        std::vector<T> xb(inputs.begin() + is, inputs.begin() + ie);
        is = ie;
        // Perform batched inference
        this->evaluate<T>(xb, yb, size);
        // Move the batched outputs back to the main outputs vector
        std::move(yb.begin(), yb.end(), outputs.begin() + os);
        os += size * this->out_tot_dim;
      }
    } else {
      std::ostringstream message;
      message << "\nFrom tf2::model::call():"
              << "\n> Column-major ordered input data are "
              << "currently not supported for batched inference.";
      throw std::runtime_error(message.str());
    }
  }
}

template <typename T>
std::vector<T> tf2::model::call(
  const std::vector<T>& inputs,
  const std::int32_t nb_pts
) {
  std::vector<T> outputs(nb_pts * this->out_tot_dim);
  this->call<T>(inputs, outputs, nb_pts);
  return outputs;
}

template <typename T>
std::vector<std::vector<T>> tf2::model::call(
  const std::vector<std::vector<T>>& inputs,
  const std::int32_t nb_pts
) {
  // Force column-major order
  this->rowmajor = false;
  // Flatten inputs
  auto x = tf2::ops::flatten<T>(inputs, true);
  // Perform inference
  auto y = this->call<T>(x, nb_pts);
  // Reshape outputs
  std::vector<std::int32_t> shape = {nb_pts, this->out_tot_dim};
  return tf2::ops::reshape<T>(y, shape, true);
}

// Explicit templates instantiation
// ====================================
// Single-precision floating-point format
// ------------------------------------
template tf2::data::list_tuple_tensor tf2::model::compose_inputs(
  const std::vector<float>& inputs,
  const std::int32_t nb_pts
);

template void tf2::model::compose_outputs(
  std::vector<float>& outputs,
  const std::vector<cppflow::tensor>& tf_outputs,
  const std::int32_t nb_pts
);

template void tf2::model::evaluate(
  const std::vector<float>& inputs,
  std::vector<float>& outputs,
  const std::int32_t nb_pts
);

template void tf2::model::call(
  const std::vector<float>& inputs,
  std::vector<float>& outputs,
  const std::int32_t nb_pts
);

template std::vector<float> tf2::model::call(
  const std::vector<float>& inputs,
  const std::int32_t nb_pts
);

template std::vector<std::vector<float>> tf2::model::call(
  const std::vector<std::vector<float>>& inputs,
  const std::int32_t nb_pts
);

// Double-precision floating-point format
// ------------------------------------
template tf2::data::list_tuple_tensor tf2::model::compose_inputs(
  const std::vector<double>& inputs,
  const std::int32_t nb_pts
);

template void tf2::model::compose_outputs(
  std::vector<double>& outputs,
  const std::vector<cppflow::tensor>& tf_outputs,
  const std::int32_t nb_pts
);

template void tf2::model::evaluate(
  const std::vector<double>& inputs,
  std::vector<double>& outputs,
  const std::int32_t nb_pts
);

template void tf2::model::call(
  const std::vector<double>& inputs,
  std::vector<double>& outputs,
  const std::int32_t nb_pts
);

template std::vector<double> tf2::model::call(
  const std::vector<double>& inputs,
  const std::int32_t nb_pts
);

template std::vector<std::vector<double>> tf2::model::call(
  const std::vector<std::vector<double>>& inputs,
  const std::int32_t nb_pts
);
