#!/bin/bash
set -e


source ./../env.sh

text="> Unpacking TF model ..."
print_text "\${text}" "="
cwd=${PWD}
model_path=${cwd}/model/SavedModel
if [ ! -d "${model_path}" ]; then
  cd ${cwd}/model/
  unzip ${model_path}.zip
  cd ${cwd}
fi
rm -rf output
mkdir output

text="> Compiling ..."
print_text "\${text}" "="
load_pycomet true
make clean
make

text="> Running ..."
print_text "\${text}" "="
./test.x

text="> Postprocessing ..."
print_text "\${text}" "="
cd data
load_python
python postproc.py
cd ../

text="> Cleaning ..."
print_text "\${text}" "="
make clean
rm -rf ${model_path}
