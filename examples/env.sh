#!/bin/bash -i
set -e

load_python() {
  # Loading Python executable
  export PYTHON_BIN="${HOME}/.conda/envs/sciml/bin"
  PATH=${PYTHON_BIN}:$PATH
  # Set proper environmental variables
  export TF_CPP_MIN_LOG_LEVEL=3
}

load_tf2() {
  eval use_gpu="${1}"
  # Loading the TF2 module
  if ${use_gpu}; then
    module load tf2/1.0-gcc-9.3.0-cuda-11.2.0-tf-2.10.0-gpu
  else
    module load tf2/1.0-gcc-12.3.0-tf-2.10.0-cpu
  fi
  # Set proper environmental variables
  export TF_CPP_MIN_LOG_LEVEL=3
  export TF_ENABLE_ONEDNN_OPTS=1
  export TF_NUM_INTEROP_THREADS=1
  export TF_NUM_INTRAOP_THREADS=1
  export CUDA_VISIBLE_DEVICES=0
  export TF_FORCE_GPU_ALLOW_GROWTH=1
}

print_text() {
  eval text="${1}"
  eval char="${2}"
  echo ""
  echo ${text}
  printf "%0.s${char}" {1..40}
  echo ""
}
