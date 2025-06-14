#include "tf2.h"


int main() {

  std::cout << "Started!" << std::endl;

  // Global inputs
  std::string prefix = "/home/zanardi/Codes/ML/TF2/tf2/tf2/examples/msd/";
  int n = 100;

  // Load model from file
  std::cout << "> Loading model" << std::endl;
  auto model = tf2::model(prefix + "cpp/inpfile.json");

  // Read and flatten input data
  std::cout << "> Loading input data" << std::endl;
  std::vector<float> inputs;
  int d = 0;
  for (std::uint16_t i = 0; i < model.inputs_dim.size(); ++i)
    {
      // Extract i-th input
      auto d_i = model.inputs_dim[i];
      auto inp = tf2::csv::read<float>(
        prefix + "data.csv", {1, n+1}, {d, d+d_i}
      );
      d += d_i;
      // Append the i-th input
      auto inp_flt = tf2::ops::flatten(inp);
      // Append inputs
      inputs.insert(inputs.end(), inp_flt.begin(), inp_flt.end());
    }

  // Perform inference
  std::cout << "> Performing inference" << std::endl;
  auto outputs = model.call(inputs, n);

  // Write outputs to file
  std::cout << "> Saving predictions" << std::endl;
  auto out_arr = tf2::ops::reshape(
    outputs, {n, model.out_tot_dim}, false
  );
  tf2::csv::write(prefix + "cpp/pred.csv", out_arr);

  std::cout << "Done!" << std::endl;

  return 0;
}
