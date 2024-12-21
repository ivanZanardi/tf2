#include "pycomet.h"


int main() {

  std::cout << "Started!" << std::endl;

  // Global inputs
  std::string prefix = "/home/zanardi/Codes/ML/PyCOMET/pycomet/api/pycomet/examples/msd/";
  int n = 100;

  // Load model from file
  std::cout << "> Loading model" << std::endl;
  auto model = pycomet::model(prefix + "cpp/inpfile.json");

  // Read and flatten input data
  std::cout << "> Loading input data" << std::endl;
  auto inputs = pycomet::csv::read<float>(
    prefix + "data.csv", {1, n+1}, {0, model.inp_tot_dim}
  );

  // Perform inference
  std::cout << "> Performing inference" << std::endl;
  auto outputs = model.call(inputs, n);

  // Write outputs to file
  std::cout << "> Saving predictions" << std::endl;
  pycomet::csv::write(prefix + "cpp/pred.csv", outputs);

  std::cout << "Done!" << std::endl;

  return 0;
}
