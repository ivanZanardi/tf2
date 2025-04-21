"""
Utility module for inspecting TensorFlow models.

This module provides functions to extract and serialize metadata
(inputs/outputs tensor names and shapes) from a saved TensorFlow model 
into a JSON configuration file. This can be useful for setting up
inference workflows or configuring model deployment tools.
"""

import os
import json

from tensorflow.python.tools import saved_model_utils as sm_utils


# Functions
# =====================================
def generate(
  path_to_model: str
) -> None:
  """
  Generate a JSON file containing input/output tensor names and shapes
  from a TensorFlow model saving directory.

  The output is saved as `inpfile.json` inside the model directory.

  :param path_to_model: Path to the TensorFlow model saving directory.
  :type path_to_model: str
  """
  inputs = {"path_to_model": os.path.abspath(path_to_model)}
  signature = _get_inputs_outputs_signature(path_to_model)
  for (k, s) in signature.items():
    inputs[k+"_id"] = [v["name"] for v in s.values()]
    inputs[k+"_dim"] = [v["shape"][1] for v in s.values()]
  with open(path_to_model + "/inpfile.json", "w") as file:
    json.dump(inputs, file, indent=2)

def _get_inputs_outputs_signature(
  path_to_model: str,
  tag_set: str = "serve",
  signature_def_key: str = "serving_default",
  dumping: bool = False
) -> dict:
  """
  Extracts input and output tensor signatures from a TensorFlow model.

  :param path_to_model: Path to the TensorFlow model saving directory.
  :type path_to_model: str
  :param tag_set: Tag set used to load the MetaGraph (default "serve").
  :type tag_set: str
  :param signature_def_key: Signature definition key (default "serving_default").
  :type signature_def_key: str
  :param dumping: If True, writes the signature to `inout_sign.json`.
  :type dumping: bool
  :return: Dictionary containing input and output tensor metadata.
  :rtype: dict
  """
  # Get meta graph
  meta_graph_def = sm_utils.get_meta_graph_def(path_to_model, tag_set)
  # Get inputs/outputs tensor info
  tensor_info = {
    "inputs": meta_graph_def.signature_def[signature_def_key].inputs,
    "outputs": meta_graph_def.signature_def[signature_def_key].outputs
  }
  # Save inputs/outputs tensor signature info
  signature = {}
  for (ki, ti) in tensor_info.items():
    signature[ki] = {}
    for (kj, tj) in sorted(ti.items()):
      signature[ki][kj] = {
        "shape": [str(dim.size) for dim in tj.tensor_shape.dim],
        "name": tj.name
      }
  if dumping:
    with open(path_to_model + "/inout_sign.json", "w") as file:
      json.dump(signature, file, indent=2)
  return signature

# Main
# =====================================
if (__name__ == "__main__"):
  path_to_model = "./SavedModel"
  generate(path_to_model)
