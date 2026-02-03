import tensorflow as tf
gpus = tf.config.list_physical_devices('GPU')
for gpu in gpus:
    tf.config.experimental.set_memory_growth(gpu, True)

from tensorflow.keras import mixed_precision
mixed_precision.set_global_policy("mixed_float16")

import os
os.environ["SM_FRAMEWORK"] = "tf.keras"
import segmentation_models as sm
import numpy as np
from keras.layers import Input, Conv2D
from keras.models import Model
from PIL import Image

all_items = os.listdir("../processedData")

x_train = []
y_train = []
x_val = []
y_val = []

all_items = os.listdir("../processedData")
all_items = [f for f in all_items if f.startswith("mask")]

for i in range(410):
    name = all_items[i][5:]
    padded_image = np.fromfile(f"../processedData/image_{name}", dtype=np.uint8)
    padded_image = padded_image.reshape((512,512))

    padded_nodule = np.fromfile(f"../processedData/mask_{name}", dtype=np.bool_)
    padded_nodule = padded_nodule.reshape((512,512))
    
    padded_image = Image.fromarray(padded_image)
    padded_nodule = Image.fromarray(padded_nodule)

    padded_image = padded_image.resize((256, 256), Image.Resampling.LANCZOS)
    padded_nodule = padded_nodule.resize((256, 256), Image.Resampling.LANCZOS)

    padded_image = np.array(padded_image)
    padded_nodule = np.array(padded_nodule)

    x_train.append(padded_image)
    y_train.append(padded_nodule)

for i in range(119):
    name = all_items[480+i][5:]
    padded_image = np.fromfile(f"../processedData/image_{name}", dtype=np.uint8)
    padded_image = padded_image.reshape((512,512))

    padded_nodule = np.fromfile(f"../processedData/mask_{name}", dtype=np.bool_)
    padded_nodule = padded_nodule.reshape((512,512))

    padded_image = Image.fromarray(padded_image)
    padded_nodule = Image.fromarray(padded_nodule)

    padded_image = padded_image.resize((256, 256), Image.Resampling.LANCZOS)
    padded_nodule = padded_nodule.resize((256, 256), Image.Resampling.LANCZOS)

    padded_image = np.array(padded_image)
    padded_nodule = np.array(padded_nodule)

    x_val.append(padded_image)
    y_val.append(padded_nodule)

x_train = np.array(x_train, dtype=np.float16) / 255.0
y_train = np.array(y_train, dtype=np.float16)
x_val = np.array(x_val, dtype=np.float16) / 255.0
y_val = np.array(y_val, dtype=np.float16)

x_train = np.expand_dims(x_train, axis=-1)
x_val = np.expand_dims(x_val, axis=-1)
y_train = np.expand_dims(y_train, axis=-1)
y_val = np.expand_dims(y_val, axis=-1)

BACKBONE = 'resnet34'

preprocess_input = sm.get_preprocessing(BACKBONE)

model = sm.Unet(BACKBONE, encoder_weights=None)

model.summary()

inp = Input(shape=(None, None, 1), dtype='float16')
l1 = Conv2D(3, (1, 1), dtype='float16')(inp)
out = model(l1)

model = Model(inp, out, name=model.name)

def iou_fp32(y_true, y_pred):
    y_true = tf.cast(y_true, tf.float16)
    return sm.metrics.iou_score(y_true, y_pred)

model.compile( 'Adam', loss=sm.losses.bce_jaccard_loss, metrics=[iou_fp32])

model.fit(
   x=x_train,
   y=y_train,
   batch_size=5,
   epochs=40,
   validation_data=(x_val, y_val),
)

model.save("my_model.keras")
