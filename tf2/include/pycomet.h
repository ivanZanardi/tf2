#ifndef pycomet_pycomet_h_
#define pycomet_pycomet_h_

// PyCOMET headers
#include "includes.h"
#include "utils.h"
#include "model.h"
#include "interface.h"

namespace pycomet {

  /**
   * @brief Retrieves the version information for TensorFlow, CppFlow, and PyCOMET.
   *
   * This function returns a string representing the version information of
   * TensorFlow, CppFlow, and the PyCOMET library.
   *
   * @return A string containing the version information.
   */
  std::string version();

} // namespace pycomet

#endif // pycomet_pycomet_h_
