#! /usr/bin/env python

import os
import sys
import time
import datetime

import tensorflow as tf


DO_DNN = True
DO_LSTM = False

INVERSION = 'vae'

INDIR = '/data/t3home000/dabercro/training/classification/tf_%s' % INVERSION

INFILES = os.listdir(INDIR)

modelroot = '/data/t3home000/dabercro/models/{0}_%s'.format(
    datetime.datetime.fromtimestamp(time.time()).strftime('%y%m%d')
)
modelindex = 0 if len(sys.argv) < 3 else sys.argv[2]
# Let's make a new directory each time
while os.path.exists(modelroot % modelindex):
    modelindex += 1

MODELDIR = modelroot % modelindex if not os.path.exists('checkpoint') else '.'

INPUTSFILE = '/home/dabercro/hbb/analysis/regression%s.txt' % '_rawpuppi'
OUTPUTSFILE = '/home/dabercro/hbb/analysis/targets%i.txt' % 9


# Logging
tf.logging.set_verbosity(tf.logging.INFO)

# Read the keys from the txt file containing inputs
get_branches = lambda x: [name.strip().split(' = ')[0]
                          for name in open(x, 'r') if name.strip()]
inputs = get_branches(INPUTSFILE)
outputs = get_branches(OUTPUTSFILE)


def input_fn():

    dataset = tf.data.TFRecordDataset([os.path.join(INDIR, infile) for infile in INFILES],
                                      compression_type='GZIP')

    def mapping(record):

        parsed = lambda labels: tf.parse_single_example(record, {
            key: tf.FixedLenFeature((), tf.float32)
            for key in labels
        })
                
        parsed_inputs = parsed(inputs)
        parsed_outputs = parsed(outputs)

        return parsed_inputs, {
            key: parsed_outputs[key]
            for key in outputs + inputs
            }


    return dataset.map(mapping).\
        shuffle(1000000).\
        batch(100000).\
        repeat()


def my_loss(labels, logits, **kwargs):

    return tf.losses.huber_loss(labels=labels,
                                predictions=logits,
                                reduction=tf.losses.Reduction.NONE,
                                delta=0.05,
                                **kwargs)


def model_fn(features, labels, mode, params):

    head = params['head']

    # Encoding

    net = tf.feature_column.input_layer(features, [
            tf.feature_column.numeric_column(key=key)
            for key in inputs
            ])

    net = tf.layers.batch_normalization(net)

    for units in [512, 256, 128]:
        net = tf.layers.dense(net, units=units, activation=tf.nn.relu)

    # Encoded layer

    # Decoding

    logits = tf.layers.dense(net, len(outputs) + len(inputs), activation=None)

    def train_op_fn(loss):
        optimizer = tf.train.AdamOptimizer()
        train_op = optimizer.minimize(
            loss,
            global_step=tf.train.get_global_step()
            )
        return train_op

    return head.create_estimator_spec(
        features=features,
        mode=mode,
        labels=labels,
        logits=logits,
        train_op_fn=train_op_fn
        )


estimator = tf.estimator.Estimator(
    model_fn=model_fn,
    model_dir=MODELDIR,
    params={
        'head': tf.contrib.estimator.multi_head(
            heads=[
                tf.contrib.estimator.regression_head(
                    loss_fn=my_loss,
                    label_dimension=1,
                    name=label)
                for label in outputs + inputs
                ]
            )
        }
    )

estimator.train(input_fn=input_fn, steps=int(sys.argv[1]))


def receiver_fn():
    receiver_tensors = {
        key: tf.placeholder(dtype=tf.float32, shape=None, name='%s' % key)
        for key in inputs
        }

    # Just pass the tensors straight through
    return tf.estimator.export.ServingInputReceiver(receiver_tensors, receiver_tensors)

estimator.export_savedmodel(
    os.path.join(MODELDIR, 'savedmodel'),
    receiver_fn
    )

if MODELDIR != '.':
    os.system('cp %s %s' % (__file__, MODELDIR))
