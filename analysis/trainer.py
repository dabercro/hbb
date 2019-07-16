#! /usr/bin/env python

import os
import sys
import time
import datetime

import tensorflow as tf


INVERSION = 7

INDIR = '/local/dabercro/files/tf_v%i' % INVERSION
if not os.path.exists(INDIR):
    INDIR = INDIR.replace('/local/dabercro/files/',
                          '/data/t3home000/dabercro/training/regression/')

INFILES = os.listdir(INDIR)

modelroot = '/data/t3home000/dabercro/models/{0}_%s'.format(
    datetime.datetime.fromtimestamp(time.time()).strftime('%y%m%d')
)
modelindex = 0 if len(sys.argv) < 3 else sys.argv[2]
# Let's make a new directory each time
while os.path.exists(modelroot % modelindex):
    modelindex += 1

MODELDIR = modelroot % modelindex if not os.path.exists('checkpoint') else '.'

INPUTSFILE = '/home/dabercro/hbb/analysis/regression%i.txt' % 9
OUTPUTSFILE = '/home/dabercro/hbb/analysis/targets%i.txt' % 7


# Logging
tf.logging.set_verbosity(tf.logging.INFO)

# Read the keys from the txt file containing inputs
get_branches = lambda x: [name.strip().split(' = ')[0] for name in open(x, 'r') if name.strip()]
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

        target = tf.stack([parsed_outputs[key] for key in outputs])

        # Return stuff

        return parsed_inputs, target


    return dataset.map(mapping).\
        shuffle(100000).\
        batch(10000).\
        repeat()


def my_loss(labels, logits, **kwargs):

    return tf.losses.huber_loss(labels=labels,
                                predictions=logits,
#                                reduction=tf.losses.Reduction.NONE,
                                delta=0.25,
                                **kwargs)


def model_fn(features, labels, mode):
    # Set up the PFCandidates info

    n_pfcands = 30
    pf_features = [
        'pt', 'eta', 'phi', 'm',
        'dxy', 'dz', 'q'
    ]

    net = tf.feature_column.input_layer(features, [
        tf.feature_column.numeric_column(key=key)
        for key in inputs
    ])

    for units in [50, 50, 50]:
        net = tf.layers.dense(net, units=units, activation=tf.nn.relu)

    out_layer = tf.layers.dense(net, len(outputs), activation=None)

    if mode == tf.estimator.ModeKeys.PREDICT:

        return tf.estimator.EstimatorSpec(
            mode, predictions={
                'output': out_layer
            })

    loss = my_loss(labels, out_layer)

    if mode == tf.estimator.ModeKeys.EVAL:
        return tf.estimator.EstimatorSpec(
            mode, loss=loss)

    optimizer = tf.train.AdamOptimizer()

    train_op = optimizer.minimize(loss, global_step=tf.train.get_global_step())

    return tf.estimator.EstimatorSpec(mode, loss=loss, train_op=train_op)


estimator = tf.estimator.Estimator(
    model_fn=model_fn,
    model_dir=MODELDIR
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
