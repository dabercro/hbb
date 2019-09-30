#! /usr/bin/env python

import os
import sys
import time
import datetime

import tensorflow as tf


DO_DNN = True
DO_LSTM = False

INVERSION = '190903_chs'

INDIR = '/local/dabercro/files/tf_%s' % INVERSION
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

INPUTSFILE = '/home/dabercro/hbb/analysis/regression%s.txt' % '_rawpuppi'
OUTPUTSFILE = '/home/dabercro/hbb/analysis/targets%i.txt' % 9


# Logging
tf.logging.set_verbosity(tf.logging.INFO)

# Read the keys from the txt file containing inputs
get_branches = lambda x: [name.strip().split(' = ')[0]
                          for name in open(x, 'r') if name.strip()]
inputs = get_branches(INPUTSFILE)
#outputs = get_branches(OUTPUTSFILE)
outputs = ['Jet_gen_ptratio']


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
            for key in outputs
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

    # Set up the PFCandidates info
    n_pfcands = 30
    pf_features = [
        '',
        '_dxy',
        '_dz',
        '_is_chhadron',
        '_is_ele',
        '_is_muon',
        '_is_nhadron',
        '_is_photon',
        '_puppiwt',
        '_q',
        '_transformed_e',
        '_transformed_px',
        '_transformed_py',
        '_transformed_pz',
    ]

    lstm_inputs = ['Jet_pf_%i%s' % (i_cand, pf_feature)
                   for i_cand in xrange(n_pfcands - 1, -1, -1)
                   for pf_feature in pf_features]

    if DO_DNN:

        reg_net = tf.feature_column.input_layer(features, [
            tf.feature_column.numeric_column(key=key)
            for key in inputs if '_pf_' not in key
        ])

        reg_net = tf.layers.batch_normalization(reg_net)

        for units in [1024, 1024, 1024,
                      512, 256, 128]:
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
                for n_hidden in [12, 12]
            ])

        lstm_net, states = tf.nn.static_rnn(rnn_cell, lstm_net, dtype=tf.float32)


    if DO_DNN and DO_LSTM:
        net = tf.concat([reg_net, lstm_net[-1]], 1)
    elif DO_DNN:
        net = reg_net
    else:
        net = lstm_net[-1]


    for units in []:
        net = tf.layers.dense(net, units=units, activation=tf.nn.relu)

    logits = tf.layers.dense(net, len(outputs), activation=None)

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
                for label in outputs
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
