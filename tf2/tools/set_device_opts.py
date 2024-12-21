import json
import tensorflow as tf


# Inputs
# =====================================
# inpfile = './inpfile.json'
inpfile = '/home/zanardi/Codes/ML/PyCOMET/pycomet/api/test/chemreac/model/inpfile.json'
growth = True
nb_threads = 8
mem_fraction = 0.3
visible_device_list = '0'


# Main
# =====================================
def create_serialized_options(
  growth,
  nb_threads,
  mem_fraction,
  visible_device_list
):
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

def main():
  inpdata = json.load(open(inpfile, 'r'))
  inpdata['config'] = create_serialized_options(
    growth,
    nb_threads,
    mem_fraction,
    visible_device_list
  )
  json.dump(inpdata, open(inpfile, 'w'), indent=4)


if (__name__ == '__main__'):
  main()
