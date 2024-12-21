#ifndef pycomet_model_h
#define pycomet_model_h

#include "includes.h"
#include "utils.h"
#include <cppflow/cppflow.h>

namespace pycomet {

  /**
   * @brief Class representing a TensorFlow-based PyCOMET model.
   */
  class model {

  private:

    // TensorFlow model
    std::string path_to_model;
    std::unique_ptr<cppflow::model> tfmodel;
    std::vector<std::string>* config = nullptr;

    // Inputs/Outputs
    const std::string inputs_id_prefix = "serving_default";
    const std::string outputs_id_prefix = "StatefulPartitionedCall";
    const std::int32_t max_nb_io = 100;

    // Data major ordering
    bool rowmajor = true;

    // Batched inference
    std::int32_t batch_size = -1;

    // IO operations
    pycomet::data::list_tuple_vector<int64_t> ops;

    /**
     * @brief Check the availability of input/output operations
     *        and retrieve their dimensions.
     *
     * This function checks the availability of input/output operations
     * for the specified identifiers and retrieves their dimensions.
     * If an identifier is not found, a std::runtime_error is thrown.
     *
     * @param identifiers The list of identifiers to check.
     * @return A vector of integers representing the dimensions
     *         of the identified operations.
     * @throws std::runtime_error If an identifier is not found
     *         among available operations.
     */
    std::vector<std::int32_t> check_io_ops(
      const std::vector<std::string> identifiers
    );

    /**
     * @brief Retrieve information about the
     *        operations in the TensorFlow model.
     *
     * This function retrieves information about the operations
     * in the TensorFlow model. It obtains the identifiers of all
     * operations, filters them based on the specified input and
     * output prefixes, and stores the identifier-shape pairs in
     * the internal data structure.
     */
    void get_ops_info();

    /**
     * @brief Parse input information from a JSON file to construct the model.
     *
     * This function reads input information from the specified JSON file and
     * initializes the internal model parameters based on the parsed input data.
     *
     * @param inpfile The path to the JSON input file.
     */
    void parse_inputs(const std::string inpfile);

    /**
     * @brief Set the global TensorFlow context with
     *        the specified configuration.
     *
     * This function creates a new TensorFlow context options
     * object with the provided configuration and replaces the
     * global TensorFlow context with the new options.
     *
     * @param config The vector of strings representing the TensorFlow
     *               configuration. Each string in the vector corresponds
     *               to a configuration option.
     */
    void set_global_context(const std::vector<std::string>& config);

    /**
     * @brief Compose input data for the TensorFlow model.
     *
     * This function composes input data for the TensorFlow model
     * based on the provided inputs and the number of points.
     * It processes each input, applies column-major ordering if
     * necessary, converts the input to a TensorFlow tensor with
     * the proper shape, and constructs a list of tuples containing
     * the input identifiers and corresponding tensors.
     *
     * @tparam T The type of the input data.
     * @param inputs The vector of input data.
     * @param nb_pts The number of points.
     * @return A list of tuples representing the composed
     *         input data in the specified format.
     */
    template <typename T>
    pycomet::data::list_tuple_tensor compose_inputs(
      const std::vector<T>& inputs,
      const std::int32_t nb_pts
    );

    /**
     * @brief Compose output data from TensorFlow model.
     *
     * This function composes output data from the TensorFlow
     * model based on the TensorFlow output tensors and the
     * number of points. It handles both single- and multi-output
     * cases, adjusting for row-major and column-major ordering.
     *
     * @tparam T The type of the output data.
     * @param outputs The vector to store the composed output data.
     * @param tf_outputs The TensorFlow output tensors.
     * @param nb_pts The number of points.
     */
    template <typename T>
    void compose_outputs(
      std::vector<T>& outputs,
      const std::vector<cppflow::tensor>& tf_outputs,
      const std::int32_t nb_pts
    );

  public:

    // Constructor
    explicit model(const std::string inpfile);

    // Destructor
    ~model() = default;

    // Inputs/Outputs
    std::vector<std::string> inputs_id;
    std::vector<std::string> outputs_id;
    std::vector<std::int32_t> inputs_dim;
    std::vector<std::int32_t> outputs_dim;

    // Inputs/Outputs total dimensions
    std::int32_t inp_tot_dim;
    std::int32_t out_tot_dim;

    /**
     * @brief Evaluate the TensorFlow model.
     *
     * This function performs an evaluation of the TensorFlow
     * model based on the provided input data, updating the
     * specified output vector for the given number of points.
     *
     * @tparam T The type of the input and output data.
     * @param inputs The vector of input data.
     * @param outputs The vector to store the output data.
     * @param nb_pts The number of points.
     */
    template <typename T>
    void evaluate(
      const std::vector<T>& inputs,
      std::vector<T>& outputs,
      const std::int32_t nb_pts
    );

    /**
     * @brief Perform model inference on input data, handling
     *        batched inference if applicable.
     *
     * This function performs inference on the provided input data,
     * updating the specified output vector. If batched inference is
     * enabled, it processes the input data in batches.
     *
     * @tparam T The type of the input and output data.
     * @param inputs The vector of input data.
     * @param outputs The vector to store the output data.
     * @param nb_pts The number of points in the input data.
     *
     * @throws std::runtime_error If column-major ordered
     *         input data are provided for batched inference
     *         (which is not currently supported).
     */
    template <typename T>
    void call(
      const std::vector<T>& inputs,
      std::vector<T>& outputs,
      const std::int32_t nb_pts
    );
  
    /**
     * @brief Perform a model inference using the provided inputs
     *        and return the outputs.
     *
     * This function performs a model inference using the provided
     * inputs and returns the resulting outputs. It internally calls
     * the `call` function to update the specified outputs vector.
     *
     * @tparam T The type of the input and output data.
     * @param inputs The vector of input data.
     * @param nb_pts The number of points.
     * @return A vector containing the model outputs.
     */
    template <typename T>
    std::vector<T> call(
      const std::vector<T>& inputs,
      const std::int32_t nb_pts
    );

    /**
     * @brief Perform a model inference using the provided inputs
     *        and return the outputs.
     *
     * This function performs a model inference for the given vector
     * of input sequences. It first forces the internal data major
     * ordering to be column-major, then flattens the input sequences,
     * performs the inference using the flattened inputs, and finally
     * reshapes the output to match the specified shape.
     *
     * @tparam T The type of the input and output data.
     * @param inputs The vector of input sequences.
     * @param nb_pts The number of points in each input sequence.
     * @return A 2D vector representing the batched outputs.
     */
    template <typename T>
    std::vector<std::vector<T>> call(
      const std::vector<std::vector<T>>& inputs,
      const std::int32_t nb_pts
    );

  };

} // namespace pycomet

#endif // pycomet_model_h
