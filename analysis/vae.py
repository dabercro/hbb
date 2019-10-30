#! /usr/bin/env python

import os
import sys
import time
import datetime
import glob
import random

import tensorflow as tf
import numpy as np


DO_DNN = True
DO_LSTM = False

INVERSION = '191022'

INDIR = '/data/t3home000/dabercro/training/classify/%s' % INVERSION

INFILES = glob.glob('%s/*/*.tfrecord' % INDIR)
random.shuffle(INFILES)

modelroot = '/data/t3home000/dabercro/models/{0}_%s'.format(
    datetime.datetime.fromtimestamp(time.time()).strftime('%y%m%d')
)
modelindex = 0 if len(sys.argv) < 3 else sys.argv[2]
# Let's make a new directory each time
while os.path.exists(modelroot % modelindex):
    modelindex += 1

MODELDIR = modelroot % modelindex if not os.path.exists('checkpoint') else '.'

INPUTSFILE = '/home/dabercro/hbb/analysis/classify3.txt'
OUTPUTSFILE = '/home/dabercro/hbb/analysis/class_targets.txt'
DECORRFILE = '/home/dabercro/hbb/analysis/class_decorrelate.txt'


# Logging
tf.logging.set_verbosity(tf.logging.INFO)

# Read the keys from the txt file containing inputs
get_branches = lambda x: [name.strip().split(' = ')[0]
                          for name in open(x, 'r') if name.strip()]
inputs = get_branches(INPUTSFILE)
outputs = get_branches(OUTPUTSFILE)
decorr = get_branches(DECORRFILE)


def input_fn():

    dataset = tf.data.TFRecordDataset(INFILES,
                                      compression_type='GZIP')

    def mapping(record):

        parsed = lambda labels: tf.parse_single_example(record, {
            key: tf.FixedLenFeature((), tf.float32)
            for key in labels
        })
                
        parsed_inputs = parsed(inputs)
        parsed_outputs = parsed(outputs + decorr + inputs)

        output = {
            '%s_%i' % (key, index): parsed_outputs[key]
            for key in outputs
            for index in range(2)
            }
        output.update({
                key: parsed_outputs[key]
                for key in decorr + inputs
                })

        return parsed_inputs, output


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


def my_class_loss(labels, logits, **kwargs):
    return tf.losses.sigmoid_cross_entropy(multi_class_labels=labels,
                                           logits=logits,
                                           reduction=tf.losses.Reduction.NONE,
                                           **kwargs)


# Some stuff from https://www.tensorflow.org/tutorials/generative/cvae
def log_normal_pdf(sample, mean, logvar, raxis=1):
  log2pi = tf.math.log(2. * np.pi)
  return tf.reduce_sum(-0.5 * ((sample - mean) ** 2. * tf.exp(-logvar) + logvar + log2pi),
                       axis=raxis)



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

    # Classifier + decorrelated variables + other bits
    additional_bits = 3
    encoded_num = 1 + len(decorr) + additional_bits

    # This will be used to reconstruct event
    encoded_mean = tf.layers.dense(net, units=encoded_num)
    encoded_sigma = tf.layers.dense(net, units=encoded_num)

    random_layer = tf.random.normal(shape=tf.shape(encoded_sigma))
    encoded_result = encoded_mean + encoded_sigma * random_layer

    # Recontruct class and decorrelated stuff
    class_mean, decorr_mean, _ = tf.split(encoded_mean, [1, len(decorr), additional_bits], 1)
    class_sigma, decorr_sigma, _ = tf.split(encoded_sigma, [1, len(decorr), additional_bits], 1)

    random_class = tf.random.normal(shape=tf.shape(class_sigma))
    random_decorr = tf.random.normal(shape=tf.shape(decorr_sigma))

    # Used to reconstruct decorrelated variables
    decorr_result = decorr_mean + decorr_sigma * random_decorr
    # Added to decorr_result to make classifier training
    class_result = class_mean + class_sigma * random_class
    class_result = tf.concat([decorr_result, class_result], 1)

    # Decoding

    # Re-make the event content
    net = encoded_result

    for units in [128, 256, 512]:
        net = tf.layers.dense(net, units=units, activation=tf.nn.relu)

    reconstruction = tf.layers.dense(net, len(inputs), activation=None)

    # Get the decorrelated variables
    net = decorr_result

    decorr_out = None

    for var in tf.split(net, [1] * len(decorr), 1):
        for units in [3, 3]:
            var = tf.layers.dense(var, units=units, activation=tf.nn.relu)
        var = tf.layers.dense(var, 1, activation=None)
        decorr_out = tf.concat([decorr_out, var], 1) if decorr_out is not None else var

    net = class_result

    for units in [10, 10, 10]:
        net = tf.layers.dense(net, units=units, activation=tf.nn.relu)

    class_out = tf.layers.dense(net, len(outputs), activation=None)

    # Non-sampled classifier
    net = tf.concat([class_mean, class_sigma], 1)

    for units in [10, 10, 10]:
        net = tf.layers.dense(net, units=units, activation=tf.nn.relu)

    non_sampled = tf.layers.dense(net, len(outputs), activation=None)

    logits = tf.concat([non_sampled, class_out, decorr_out, reconstruction], 1)

    def train_op_fn(loss):
        optimizer = tf.train.AdamOptimizer()

        # From https://www.tensorflow.org/tutorials/generative/cvae again
        logpx_z = -1. * loss #?
        logpz = log_normal_pdf(encoded_result, 0., 0.)
        logqz_x = log_normal_pdf(encoded_result, encoded_mean, encoded_sigma)
        loss = -1.* (logpx_z + logpz - logqz_x)

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
            heads=
            [
                tf.contrib.estimator.binary_classification_head(
                    loss_fn=(lambda labels, logits, **y: weight * my_class_loss(labels, logits, **y)),
                    name='%s_%i' % (label, index)
                    )
                for index, weight in enumerate([0.1, 1.0])
                for label in outputs
                ] +
            [
                tf.contrib.estimator.regression_head(
                    loss_fn=my_loss,
                    label_dimension=1,
                    name=label)
                for label in decorr + inputs
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
