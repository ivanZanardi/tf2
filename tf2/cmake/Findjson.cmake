if(NOT EXISTS ${json_INCLUDE_DIR})
  include(ExternalProject)
  ExternalProject_Add(json
    GIT_REPOSITORY https://github.com/nlohmann/json
    TIMEOUT 5
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/json
    CMAKE_ARGS
      -DCMAKE_BUILD_TYPE=Release
      -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
      -DJSON_BuildTests=OFF
  )
  set(json_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
endif()

# Check if json is found and mark the include directory as advanced
if(EXISTS ${json_INCLUDE_DIR})
  set(json_FOUND 1)
else()
  set(json_FOUND 0)
endif()

# Provide the json_INCLUDE_DIR variable for other CMake targets
if(json_FOUND)
  mark_as_advanced(json_INCLUDE_DIR)
endif()
