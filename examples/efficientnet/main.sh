#!/bin/bash
set -e


source ./../env.sh

text="> Generating TF model ..."
print_text "\${text}" "="
load_python
rm -rf model
python model.py

text="> Compiling ..."
print_text "\${text}" "="
load_pycomet true
make clean
make

text="> Running ..."
print_text "\${text}" "="
./test.x

text="> Cleaning ..."
print_text "\${text}" "="
make clean
rm -rf model
