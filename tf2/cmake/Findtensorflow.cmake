# Locate TensorFlow library
find_library(tensorflow_LIBRARY
  NAMES tensorflow
)
find_path(tensorflow_INCLUDE_DIR
  NAMES tensorflow/c/c_api.h
)

# Check if TensorFlow is found and handle the components
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  tensorflow REQUIRED_VARS tensorflow_LIBRARY tensorflow_INCLUDE_DIR
  HANDLE_COMPONENTS
)

# Mark TensorFlow variables as advanced if found
if(tensorflow_FOUND)
  mark_as_advanced(tensorflow_INCLUDE_DIR tensorflow_LIBRARY)
endif()
