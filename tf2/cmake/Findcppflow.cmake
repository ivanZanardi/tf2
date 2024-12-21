include(ExternalProject)

# Add the cppflow project using ExternalProject_Add
ExternalProject_Add(cppflow
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/third-party/cppflow
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/cppflow
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    -DBUILD_EXAMPLES=OFF
    -Dtensorflow_INCLUDE_DIRS=${tensorflow_INCLUDE_DIR}
    -Dtensorflow_LIBRARIES=${tensorflow_LIBRARY}
)

# Specify include directory
set(cppflow_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)

# Check if cppflow is found
if(EXISTS ${cppflow_INCLUDE_DIR})
  set(cppflow_FOUND 1)
else()
  set(cppflow_FOUND 0)
endif()

# Provide the cppflow_INCLUDE_DIR variable for other CMake targets
if(cppflow_FOUND)
  mark_as_advanced(cppflow_INCLUDE_DIR)
endif()
