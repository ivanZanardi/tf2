# TF2

**TensorFlow to Fortran**

---
TF2 enables the integration of TensorFlow models into C++/Fortran projects seamlessly, without the need for TensorFlow installation or compilation. Explore the [examples](https://github.com/ivanZanardi/tf2/tree/main/examples) to understand how to incorporate a TensorFlow `tf.Model` into your C++/Fortran project.

TF2 utilizes [CppFlow](https://github.com/serizba/cppflow/) and the [TensorFlow C API](https://www.tensorflow.org/install/lang_c) to execute models. This project simplifies the management and execution of models in C++/Fortran, handling concerns about void, malloc, or free operations. With TF2, you can effortlessly:

* Open saved models created with TensorFlow
* Execute TensorFlow neural networks in C++/Fortran
* Perform tensor manipulation directly from C++

## Dependencies

Before installing the required dependencies, check the official TensorFlow [compatibility matrix](https://www.tensorflow.org/install/source/) to ensure version compatibility with your system and build environment.

* [TensorFlow C API](https://www.tensorflow.org/install/lang_c/) = 2.10.0

### GPU support

The code has been tested with the following configuration:

* [GNU Compiler](https://ftp.gnu.org/gnu/gcc/) = 9.3.0
* [CUDA](https://developer.nvidia.com/cuda-toolkit-archive/) = 11.2
* [cuDNN](https://developer.nvidia.com/rdp/cudnn-archive/) = 8.1

## Installation

To install TF2, follow these steps after downloading the dependencies:

1. Define installation directories

  ```bash
  export SRC_DIR=<path-to-TF2-source-dir>
  export BUILD_DIR=<path-to-build-dir>
  export INSTALL_DIR=<path-to-install-dir>
  ```

2. Define dependencies directories

  ```bash
  export TensorFlow_DIR=<path-to-tensorflow-dir>
  ```

3. Prepare installation directories

  ```bash
  rm -rf $BUILD_DIR $INSTALL_DIR
  mkdir $BUILD_DIR
  cd $BUILD_DIR
  ```

4. Configure CMake

  ```bash
  cmake $SRC_DIR \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
    -DBUILD_EXAMPLES=OFF \
    -Dtensorflow_INCLUDE_DIR=$TensorFlow_DIR/include \
    -Dtensorflow_LIBRARY=$TensorFlow_DIR/lib/libtensorflow.so
  ```

5. Install

  ```bash
  make -j 4
  make install
  ```

## License

TF2 is distributed under the [MIT license](https://github.com/ivanZanardi/tf2/blob/main/LICENSE). You are welcome to utilize, modify, and contribute to this project in accordance with the terms outlined in the license.
