#include <iostream>
#include <cppflow/cppflow.h>

int main() {

  // Parameters
  int nb_iters = -1;
  std::string MODEL_PATH = "SavedModel";
  std::string IMAGE_PATH = "image.jpg";

  // Input tensor
  auto input = cppflow::decode_jpeg(cppflow::read_file(IMAGE_PATH));
  input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
  input = cppflow::expand_dims(input, 0);

  // Model
  cppflow::model model(MODEL_PATH);

  std::cout << std::boolalpha << "Cuda available: "<< model.is_cuda_available << std::endl;

  // Inference
  auto output = model(input);
  if (nb_iters > 1) {
    for (int i = 0; i < nb_iters; ++i) {
      std::cout << "Iter: " << i << std::endl;
      output = model(input);
    }
  }
  std::cout << "Class: " << cppflow::arg_max(output, 1) << std::endl;

  return 0;
}
