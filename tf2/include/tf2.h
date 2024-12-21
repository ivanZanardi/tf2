#ifndef tf2_tf2_h_
#define tf2_tf2_h_

// TF2 headers
#include "includes.h"
#include "utils.h"
#include "model.h"
#include "interface.h"

namespace tf2 {

  /**
   * @brief Retrieves the version information for TensorFlow, CppFlow, and TF2.
   *
   * This function returns a string representing the version information of
   * TensorFlow, CppFlow, and the TF2 library.
   *
   * @return A string containing the version information.
   */
  std::string version();

} // namespace tf2

#endif // tf2_tf2_h_
