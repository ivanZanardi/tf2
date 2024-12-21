get_filename_component(Fortran_COMPILER_NAME ${CMAKE_Fortran_COMPILER} NAME)

if (CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")
  LogMessage("Setting the gfortran compiler flags ...")
  if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(FATAL_ERROR "Windows builds are currently not supported")
  else()
    set(COMMON_FLAG "-cpp -fno-unsafe-math-optimizations \
      -ffree-line-length-none -frealloc-lhs")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(DEBUG_FLAG "-g -fbounds-check -fbacktrace -fdump-core -ggdb -Wall \
        -pg -Wno-line-truncation -finit-real=inf -Wno-unused-dummy-argument \
        -Wno-unused-function -Wno-uninitialized -Wno-maybe-uninitialized \
        -Wno-surprising -Wno-unused-variable")
      set(OPTIM_FLAG "-O0")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
      set(DEBUG_FLAG "")
      set(OPTIM_FLAG "-O3 -fPIC -fbounds-check -fbacktrace \
        -march=native -funroll-loops -ftree-vectorize")
    else()
      message(FATAL_ERROR "Unknown CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
    endif()
    set(FFLAGS "${COMMON_FLAG} ${OPTIM_FLAG} ${DEBUG_FLAG}")
    set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${FFLAGS}")
    set(CMAKE_SHARED_LIBRARY_LINK_Fortran_FLAGS "")
  endif()
elseif(Fortran_COMPILER_NAME MATCHES "ifort.*")
  message(FATAL_ERROR "Intel compiler is currently not supported")
else()
  message(FATAL_ERROR "Unknown compiler: ${Fortran_COMPILER_NAME}")
endif()
