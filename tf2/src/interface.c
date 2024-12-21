#include "interface.h"


tf2::model *tf2::init_model(char *inpfile) {
  std::string file(inpfile);
  return new tf2::model(file);
}

std::int32_t tf2::get_inp_tot_dim(tf2::model *mdl) {
  return mdl->inp_tot_dim;
}

std::int32_t tf2::get_out_tot_dim(tf2::model *mdl) {
  return mdl->out_tot_dim;
}

void tf2::delete_model(tf2::model *mdl) {
  mdl->~model();
}

void tf2::call_model_float(
  tf2::model *mdl,
  std::int32_t *nb_pts,
  float *inputs,
  float *outputs
) {
  // Convert input and output arrays to vectors
  std::vector<float> x(inputs, inputs + mdl->inp_tot_dim * (*nb_pts));
  std::vector<float> y(outputs, outputs + mdl->out_tot_dim * (*nb_pts));
  // Perform in-place inference
  mdl->call(x, y, *nb_pts);
  // Copy the updated outputs back to the 'outputs' array
  std::copy(y.begin(), y.end(), outputs);
}

void tf2::call_model_double(
  tf2::model *mdl,
  std::int32_t *nb_pts,
  double *inputs,
  double *outputs
) {
  // Convert input and output arrays to vectors
  std::vector<double> x(inputs, inputs + mdl->inp_tot_dim * (*nb_pts));
  std::vector<double> y(outputs, outputs + mdl->out_tot_dim * (*nb_pts));
  // Perform in-place inference
  mdl->call(x, y, *nb_pts);
  // Copy the updated outputs back to the 'outputs' array
  std::copy(y.begin(), y.end(), outputs);
}
