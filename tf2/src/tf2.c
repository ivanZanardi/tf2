#include <cppflow/cppflow.h>
#include "tf2.h"


std::string tf2::version() {
  return cppflow::version() + " TF2: 1.0";
}