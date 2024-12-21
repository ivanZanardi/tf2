#ifndef tf2_interface_h_
#define tf2_interface_h_

#include "includes.h"
#include "model.h"

namespace tf2 {

  #ifdef __cplusplus
  extern "C" {
  #endif // __cplusplus

    /**
     * @brief Initialize a TF2 model.
     *
     * @param inpfile Path to the input file.
     * @return A pointer to the initialized TF2 model.
     */
    tf2::model* init_model(char* inpfile);

    /**
     * @brief Get the total input dimension of the model.
     *
     * @param mdl Pointer to the TF2 model.
     * @return Total input dimension.
     */
    std::int32_t get_inp_tot_dim(tf2::model* mdl);

    /**
     * @brief Get the total output dimension of the model.
     *
     * @param mdl Pointer to the TF2 model.
     * @return Total output dimension.
     */
    std::int32_t get_out_tot_dim(tf2::model* mdl);

    /**
     * @brief Delete a TF2 model.
     *
     * @param mdl Pointer to the TF2 model to delete.
     */
    void delete_model(tf2::model* mdl);

    /**
     * @brief Call the TF2 model with single-precision inputs/outputs.
     *
     * @param mdl Pointer to the TF2 model.
     * @param nb_pts Total number of evaluated points.
     * @param inputs 1D array of float inputs (row-/column-major).
     * @param outputs Empty 1D array of float outputs.
     */
    void call_model_float(
      tf2::model* mdl,
      std::int32_t* nb_pts,
      float* inputs,
      float* outputs
    );

    /**
     * @brief Call the TF2 model with double-precision inputs/outputs.
     *
     * @param mdl Pointer to the TF2 model.
     * @param nb_pts Total number of evaluated points.
     * @param inputs 1D array of double inputs (row-/column-major).
     * @param outputs Empty 1D array of double outputs.
     */
    void call_model_double(
      tf2::model* mdl,
      std::int32_t* nb_pts,
      double* inputs,
      double* outputs
    );

  #ifdef __cplusplus
  } // extern "C"
  #endif // __cplusplus
}

#endif // tf2_interface_h_
