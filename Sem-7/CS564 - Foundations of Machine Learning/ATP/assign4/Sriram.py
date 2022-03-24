# %%
# # Loading drive
# from google.colab import drive
# drive.mount('/content/drive')

# # %%
# %tensorflow_version 2.x
# import tensorflow as tf
# print(tf.__version__)

# # %%
# # Setting the working directory 
# !ls
# %cd drive/My\ Drive/ML_Assignment
# !pwd

# # %%
# # Downloading the data
# !wget https://www.dropbox.com/s/tp3l54tnatvbldf/bbc.csv?dl=0 -O 'bbc.csv'

# %%
# All general imports
import pandas as pd
import numpy as np
from sklearn.preprocessing import LabelEncoder
from sklearn.preprocessing import LabelBinarizer
from sklearn.model_selection import train_test_split 

import keras
from keras.preprocessing.text import Tokenizer
from keras.preprocessing.sequence import pad_sequences
from keras.layers import Input, Embedding, Reshape, Conv2D, MaxPool2D, Concatenate, Flatten, Dropout, Dense, Bidirectional, GlobalAveragePooling1D, GRU, GlobalMaxPooling1D, concatenate
from tensorflow.keras.optimizers import Adam
from keras.layers import LSTM, GRU, Conv1D, MaxPool1D, Activation, Add

from keras.models import Model, Sequential
from keras.layers.core import SpatialDropout1D

from keras.layers import Dense, Input, Embedding, Dropout, Activation, Conv1D, Softmax
from keras import initializers, regularizers, constraints, optimizers, layers
from keras import backend as K

from keras.callbacks import EarlyStopping

from sklearn.model_selection import StratifiedKFold
from sklearn.metrics import classification_report, accuracy_score
import io, os, gc

# %%
# loading data
df = pd.read_csv("bbc.csv")
print(df.columns)
le = LabelEncoder()
input_labels = le.fit_transform(df['Class'])
le_name_mapping = dict(zip(le.classes_, le.transform(le.classes_)))
print(le_name_mapping)
print(df.head())

# %%
# Pre-processing data
from gensim.parsing.preprocessing import remove_stopwords
from gensim.parsing.porter import PorterStemmer
import re
porter_stemmer = PorterStemmer()
doc_list = df['Article'].tolist()
processed_list = []
for doc in doc_list:
  doc_str = re.sub(r'[^\w\s]', '', remove_stopwords(doc))
  doc_str_stem_lst = [porter_stemmer.stem(word) for word in doc_str.split(" ")]
  doc_str_stem = " ".join(doc_str_stem_lst)
  processed_list.append(doc_str_stem)

# %%
# Defining the tokenizer
def get_tokenizer(data):
  print('Training tokenizer...')
  tokenizer = Tokenizer()
  print('Read {} Sentences'.format(len(data)))
  tokenizer.fit_on_texts(data)
  return tokenizer

# %%
def get_data(tokenizer, MAX_LENGTH, input_data, input_labels):
  print('Loading data')
  
  assert len(input_data) == len(input_labels)
  sequences = tokenizer.texts_to_sequences(input_data)
  X = pad_sequences(sequences, maxlen=MAX_LENGTH)
  Y_bcc = np.array(input_labels)

  return X, Y_bcc

# %%
tokenizer = get_tokenizer(processed_list)

# %%
MAX_LENGTH = 50
# read ml data
X, Y_bcc = get_data(tokenizer, MAX_LENGTH, processed_list, input_labels)

# %%
# Creating one-hot encodings
y_bcc_labels = keras.utils.np_utils.to_categorical(Y_bcc)
print(y_bcc_labels)

# %%
# Splitting data into train, val and test
train1_X, test_X, train1_Y, test_Y = train_test_split(X, y_bcc_labels, test_size=0.2, random_state=43)
train_X, val_X, train_Y, val_Y = train_test_split(train1_X, train1_Y, test_size=0.1, random_state=43)

# %%
# Setting hyper-parameters
NUM_CLASSES = 5

MAX_SEQUENCE_LENGTH = MAX_LENGTH

MAX_NUM_WORDS = len(tokenizer.word_index) + 1

NUM_EMBEDDING_DIM = 100

# %%
# Defining the model
print('Getting Text FF NN')
input_layer = Input(
    shape=(MAX_SEQUENCE_LENGTH, ), 
    dtype='int32')
embedding_layer = Embedding(MAX_NUM_WORDS, NUM_EMBEDDING_DIM, trainable=True)
embedded_input = embedding_layer(input_layer)
flatten_input = Flatten()(embedded_input)
dense_layer_1 = Dense(2048, activation='relu')
drop_1 = Dropout(0.1)
dense_layer_2 = Dense(1024, activation='relu')
drop_2 = Dropout(0.1)
dense_layer_3 = Dense(512, activation='relu')
drop_3 = Dropout(0.1)
dense_layer_4 = Dense(5, activation='softmax')
output_layer = dense_layer_4(drop_3(dense_layer_3(drop_2(dense_layer_2(drop_1(dense_layer_1(flatten_input)))))))
model = Model(
    inputs=input_layer, 
    outputs=output_layer)
print(model.summary())

# %%
from tensorflow.keras.optimizers import Adam
lr = 1e-3
opt = Adam(learning_rate=lr, decay=lr/50)
model.compile(
    optimizer='adam',
    loss='categorical_crossentropy',
    metrics=['accuracy'])

# %%
BATCH_SIZE = 512
NUM_EPOCHS = 100
stop = [EarlyStopping(monitor='val_loss', patience=1)]
history = model.fit(x=train_X,
                    y=train_Y,
                    batch_size=BATCH_SIZE,
                    epochs=NUM_EPOCHS,
                    validation_data=(
                      val_X, 
                      val_Y
                    ),
                    shuffle=True,
                    callbacks=stop,
          )

# %%
# Getting predictions
from sklearn import metrics
from sklearn.metrics import classification_report
predictions = model.predict(test_X)

# %%
y_pred = [idx for idx in np.argmax(predictions, axis=1)]
y_true = [idx for idx in np.argmax(test_Y, axis=1)]
print('BBC News Accuracy is')
print(metrics.accuracy_score(y_true, y_pred)*100)
print(classification_report(y_true, y_pred, target_names = list(le_name_mapping.keys())))


