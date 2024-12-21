#ifndef pycomet_interface_h_
#define pycomet_interface_h_

#include "includes.h"
#include "model.h"

namespace pycomet {

  #ifdef __cplusplus
  extern "C" {
  #endif // __cplusplus

    /**
     * @brief Initialize a PyCOMET model.
     *
     * @param inpfile Path to the input file.
     * @return A pointer to the initialized PyCOMET model.
     */
    pycomet::model* init_model(char* inpfile);

    /**
     * @brief Get the total input dimension of the model.
     *
     * @param mdl Pointer to the PyCOMET model.
     * @return Total input dimension.
     */
    std::int32_t get_inp_tot_dim(pycomet::model* mdl);

    /**
     * @brief Get the total output dimension of the model.
     *
     * @param mdl Pointer to the PyCOMET model.
     * @return Total output dimension.
     */
    std::int32_t get_out_tot_dim(pycomet::model* mdl);

    /**
     * @brief Delete a PyCOMET model.
     *
     * @param mdl Pointer to the PyCOMET model to delete.
     */
    void delete_model(pycomet::model* mdl);

    /**
     * @brief Call the PyCOMET model with single-precision inputs/outputs.
     *
     * @param mdl Pointer to the PyCOMET model.
     * @param nb_pts Total number of evaluated points.
     * @param inputs 1D array of float inputs (row-/column-major).
     * @param outputs Empty 1D array of float outputs.
     */
    void call_model_float(
      pycomet::model* mdl,
      std::int32_t* nb_pts,
      float* inputs,
      float* outputs
    );

    /**
     * @brief Call the PyCOMET model with double-precision inputs/outputs.
     *
     * @param mdl Pointer to the PyCOMET model.
     * @param nb_pts Total number of evaluated points.
     * @param inputs 1D array of double inputs (row-/column-major).
     * @param outputs Empty 1D array of double outputs.
     */
    void call_model_double(
      pycomet::model* mdl,
      std::int32_t* nb_pts,
      double* inputs,
      double* outputs
    );

  #ifdef __cplusplus
  } // extern "C"
  #endif // __cplusplus
}

#endif // pycomet_interface_h_
