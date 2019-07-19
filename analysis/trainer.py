#! /usr/bin/env python

import os
import sys
import time
import datetime

import tensorflow as tf


DO_LSTM = True

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

INPUTSFILE = '/home/dabercro/hbb/analysis/regression%i.txt' % (10 if DO_LSTM else 9)
OUTPUTSFILE = '/home/dabercro/hbb/analysis/targets%i.txt' % 7

SORT_TYPE = 'fastjet'


# Logging
tf.logging.set_verbosity(tf.logging.INFO)

# Read the keys from the txt file containing inputs
get_branches = lambda x: [name.strip().split(' = ')[0].replace('fastjet', SORT_TYPE)
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
        'ptfrac', 'deta', 'dphi',
        'dxy', 'dz', 'q'
    ]

    lstm_inputs = ['Jet_pf_%s_%i_%s' % (SORT_TYPE, n_pfcands - 1 - i_cand, pf_feature)
                   for i_cand in xrange(n_pfcands)
                   for pf_feature in pf_features]

    reg_net = tf.feature_column.input_layer(features, [
        tf.feature_column.numeric_column(key=key)
        for key in inputs if key not in lstm_inputs
    ])

    reg_net = tf.layers.batch_normalization(reg_net)

    for units in [500, 500, 500]:
        reg_net = tf.layers.dense(reg_net, units=units, activation=tf.nn.relu)

    if DO_LSTM:

        lstm_net = tf.feature_column.input_layer(features, [
            tf.feature_column.numeric_column(key=key)
            for key in lstm_inputs
        ])

        lstm_net = tf.layers.batch_normalization(lstm_net)

        lstm_net = tf.split(lstm_net, n_pfcands, 1)

        rnn_cell = tf.nn.rnn_cell.MultiRNNCell([
                tf.nn.rnn_cell.BasicLSTMCell(n_hidden)
                for n_hidden in [24, 24, 24]
            ])

        lstm_net, states = tf.nn.static_rnn(rnn_cell, lstm_net, dtype=tf.float32)

        net = tf.concat([reg_net, lstm_net[-1]], 1)

        for units in [128, 128]:
            net = tf.layers.dense(net, units=units, activation=tf.nn.relu)

    else:

        net = reg_net

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
