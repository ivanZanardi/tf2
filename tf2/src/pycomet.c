#include <cppflow/cppflow.h>
#include "pycomet.h"


std::string pycomet::version() {
  return cppflow::version() + " PyCOMET: 1.0";
}