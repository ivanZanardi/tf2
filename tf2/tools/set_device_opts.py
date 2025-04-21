"""
Utility module for configuring TensorFlow GPU/CPU session options.

This script reads an input JSON file (e.g., `inpfile.json`) and adds a 
serialized TensorFlow session configuration to it, specifying GPU memory 
growth, memory fraction, number of CPU threads, and visible GPU devices.

The resulting 'config' can be used to control session behavior when deploying 
or initializing a TensorFlow model in environments where memory and thread  
control are important.
"""

import json
import tensorflow as tf


# Functions
# =====================================
def create_serialized_options(
  growth: bool,
  nb_threads: int,
  mem_fraction: float,
  visible_device_list: str
) -> list:
  """
  Create a serialized TensorFlow ConfigProto session configuration.

  :param growth: Whether to allow GPU memory growth.
  :type growth: bool
  :param nb_threads: Number of intra- and inter-op parallelism threads.
  :type nb_threads: int
  :param mem_fraction: Fraction of total GPU memory to allocate.
  :type mem_fraction: float
  :param visible_device_list: Comma-separated list of visible GPU device indices.
  :type visible_device_list: str
  :return: Serialized ConfigProto as a list of hex strings.
  :rtype: list
  """
  config = tf.compat.v1.ConfigProto()
  # GPU options
  config.gpu_options.per_process_gpu_memory_fraction = mem_fraction
  config.gpu_options.visible_device_list = visible_device_list
  config.gpu_options.allow_growth = growth
  # CPU options
  config.inter_op_parallelism_threads = nb_threads
  config.intra_op_parallelism_threads = nb_threads
  # Serialize
  serialized = config.SerializeToString()
  return list(map(hex, serialized))


if (__name__ == "__main__"):
  # Inputs
  inpfile = "./inpfile.json"
  growth = True
  nb_threads = 8
  mem_fraction = 0.3
  visible_device_list = "0"
  # Set options
  with open(inpfile, "r") as file:
    inpdata = json.load(file)
  inpdata["config"] = create_serialized_options(
    growth,
    nb_threads,
    mem_fraction,
    visible_device_list
  )
  with open(inpfile, "w") as file:
    json.dump(inpdata, file, indent=2)
