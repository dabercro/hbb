#! /usr/bin/env python

import os
import sys

import tensorflow as tf


INVERSION = 0

INDIR = '/local/dabercro/files/tf_v%i' % INVERSION
if not os.path.exists(INDIR):
    INDIR = '/data/t3home000/dabercro/training/regression/tf_v%i' % INVERSION

INFILES = os.listdir(INDIR)

modelroot = '/home/dabercro/hbb/analysis/models/premade_v%i'
modelindex = 0
# Let's make a new directory each time
while os.path.exists(modelroot % modelindex):
    modelindex += 1

MODELDIR = modelroot % modelindex if not os.path.exists('checkpoint') else '.'

INPUTSFILE = '/home/dabercro/hbb/analysis/regression2.txt'
OUTPUTSFILE = '/home/dabercro/hbb/analysis/targets.txt'


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

        return parsed_inputs, {key: target for key in ['central', 'lower', 'upper']}

    return dataset.map(mapping).\
        shuffle(100000).\
        batch(10000).\
        repeat()


def my_loss(labels, logits, **kwargs):
    return tf.losses.huber_loss(labels=labels,
                                predictions=logits,
                                reduction=tf.losses.Reduction.NONE,
                                delta=0.05,
                                **kwargs)

def quantile(quant):

    def loss(labels, logits):
        diff = tf.math.subtract(labels, logits)
        return diff * (quant - tf.where(diff < 0,
                                        tf.ones_like(diff),
                                        tf.zeros_like(diff)))

    return loss


#estimator = tf.estimator.DNNLinearCombinedEstimator(
estimator = tf.estimator.DNNEstimator(
    feature_columns=[
        tf.feature_column.numeric_column(key=key)
        for key in inputs
    ],
    model_dir=MODELDIR,
    head=tf.contrib.estimator.multi_head(
        heads=[
            tf.contrib.estimator.regression_head(
                loss_fn=my_loss,
                label_dimension=len(outputs),
                name='central'),
            tf.contrib.estimator.regression_head(
                loss_fn=quantile(0.25),
                label_dimension=len(outputs),
                name='lower'),
            tf.contrib.estimator.regression_head(
                loss_fn=quantile(0.75),
                label_dimension=len(outputs),
                name='upper')
        ]
    ),
    hidden_units=[400, 400, 200, 300, 200]
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