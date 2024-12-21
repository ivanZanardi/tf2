#include "interface.h"


pycomet::model *pycomet::init_model(char *inpfile) {
  std::string file(inpfile);
  return new pycomet::model(file);
}

std::int32_t pycomet::get_inp_tot_dim(pycomet::model *mdl) {
  return mdl->inp_tot_dim;
}

std::int32_t pycomet::get_out_tot_dim(pycomet::model *mdl) {
  return mdl->out_tot_dim;
}

void pycomet::delete_model(pycomet::model *mdl) {
  mdl->~model();
}

void pycomet::call_model_float(
  pycomet::model *mdl,
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

void pycomet::call_model_double(
  pycomet::model *mdl,
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
