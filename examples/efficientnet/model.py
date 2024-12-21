import sys
import tensorflow as tf

model = tf.keras.applications.EfficientNetB0()
model.summary()
model.save('model', save_format='tf')
