#! /usr/bin/env python

import os
import sys
import logging
import pickle
import math
import bisect
import collections

from argparse import ArgumentParser

import numpy

import tensorflow
tensorflow.enable_eager_execution()

from tensorflow import keras
from tensorflow.keras.losses import categorical_crossentropy
from tensorflow.keras import backend as K

from tensorflow.python.framework import graph_util
from tensorflow.python.framework import graph_io

argv = sys.argv[:]
sys.argv = sys.argv[:1]
from ROOT import TFile, TTreeFormula
sys.argv = argv

logging.basicConfig(level=logging.DEBUG if os.environ.get('debug') else logging.INFO,
                    format='%(asctime)s:%(levelname)s: %(message)s')


class Reweighter(object):
    def __init__(self, smooth_data):
        self.data = [tuple(row) for row in smooth_data]

        num_data = len(self.data)
        num_per_bin = num_data/(num_data/1000)

        ndim = len(self.data[0])

        save = int(math.pow(num_per_bin, ndim))

        chunks = [self.data[i:i + save] for i in range(0, num_data, save)]

        self.bin = [row[0][0] for row in chunks]
        self.bin.append(self.data[-1][0])

        for index, row in enumerate(chunks):
            self.bin[index + 1] = (self.bin[index + 1] + row[-1][0])/2

        self.children = [] if ndim == 1 else \
            [Reweighter([point[1:] for point in row]) for row in chunks]

    def __str__(self):        
        return '\n'.join([str(item) for item in [self.bin] + self.children])

    def get_weight(self, point):
        index = bisect.bisect(self.bin, point[0])

        if index == len(self.bin):
            index -= 1

        if index:
            index -= 1

        size = self.bin[index + 1] - self.bin[index]

        if self.children:
            size *= self.children[index].get_weight(point[1:])

        return size


class Trainer(object):
    def __init__(self, inputs, targets, layers, endsoft, adversary=None, **kwargs):

        num_inputs = len(inputs)
        num_targets = len(targets[0])

        logging.info('Using %i variables', num_inputs)
        logging.info('Training for %i targets', num_targets)

        nodes_in_layer = num_inputs

        self.my_model = keras.models.Sequential(
            [keras.layers.Dense(nodes_in_layer, input_shape=(num_inputs, ))] + 
            [keras.layers.Dense(nodes_in_layer, activation='tanh') for _ in xrange(layers)] +
            [keras.layers.Dense(num_targets, activation='softmax' if endsoft else'linear')]
            )

        if adversary:
            adversary.model = self.my_model
            kwargs['loss'] = adversary.loss

        self.my_model.compile(**kwargs)


def load(inputfile, target, inputs, adversary, weight, reweight):
    """
    Parameters:
      inputfile: Name of the ROOT file that contains all our data for training
      target: Expression that yields class number
      inputs: List of expressions to input into the classifier
      adversary: Expressions that the adversary should not be able to guess
        from the classifier output
      weight: Expressions to get the sample weights
      reweight: Bool to decide if should reweight

    Returns:
      Numpy Arrays that can be used in fitting with the following info
      - Labels that are used to classify
      - Raw data to do the classification with
      - Data that should not be predictable based on the predicted label
    """

    in_file = TFile(inputfile)
    tree = in_file.events

    target_form = TTreeFormula(target, target, tree)
    val_forms = [TTreeFormula(v, v, tree) for v in inputs]
    adversary_forms = [TTreeFormula(a, a, tree) for a in adversary]
    weight_form = TTreeFormula(weight, weight, tree) if weight else None

    reserve = tree.GetEntries()

    data = numpy.zeros((reserve, len(inputs)))
    smooths = numpy.zeros((reserve, len(adversary)))
    labels = numpy.zeros((reserve, 1))
    weights = numpy.zeros(reserve) if weight else None

    logging.info('Reading %i events', reserve)

    # Set up the inputs
    for event, _ in enumerate(tree):
        if event == reserve:
            break

        if event % 10000 == 0:
            logging.info('Filling %s', str(float(event * 100)/reserve) + '%')

        labels[event][0] = target_form.EvalInstance()
        if weight:
            weights[event] = weight_form.EvalInstance()

        for jndex, val in enumerate(val_forms):
            data[event][jndex] = val.EvalInstance()

        for jndex, adv in enumerate(adversary_forms):
            smooths[event][jndex] = adv.EvalInstance()

    if reweight:
        # Want to reweight each class separately
        smooth_dict = collections.defaultdict(list)

        for label, point in zip(labels, smooths):
            smooth_dict[label[0]].append(point)

        reweighters = {key: Reweighter(row) for key, row in smooth_dict.iteritems()}

        for index, point in enumerate(data):
            weights[index] *= reweighters[labels[index][0]].get_weight(point)

    return keras.utils.to_categorical(labels), data, smooths, weights


def parse():
    parser = ArgumentParser(
        prog='crombie kerasreg',
        description='Reads some configuration file, takes some arguments and does Keras stuff'
        )

    parser.add_argument('-c', '--config', dest='config', metavar='FILE', required=True,
                        help='The config file that contains the names of training variables')
    parser.add_argument('-i', '--input', dest='input', metavar='FILE', required=True,
                        help='The name of the input file for tree')
    parser.add_argument('-t', '--target', dest='target', metavar='EXPR', required=True,
                        help='The branch that numbers the class')
    parser.add_argument('-o', '--output', dest='output', metavar='FILE', required=True,
                        help='The name of the output file, which will be overwritten by training')
    parser.add_argument('-e', '--epochs', dest='epochs', metavar='NUM', type=int,
                        help='Number of epochs', default=10)
    parser.add_argument('-b', '--batch', dest='batch', metavar='NUM', type=int,
                        help='Batch size', default=1000)
    parser.add_argument('-l', '--layers', dest='layers', metavar='NUM', type=int,
                        help='Number of hidden layers', default=5)
    parser.add_argument('-s', '--seed', dest='seed', metavar='NUM', type=int, required=True,
                        help='Starting seed')
    parser.add_argument('-a', '--adversary', dest='adversary', metavar='EXPR', required=True,
                        help='The branches that we want the adversary layer to try to predict. Separate multiple with ","')
    parser.add_argument('-w', '--weight', dest='weight', metavar='EXPR', required=True,
                        help='The branches that give the sample weight')
    parser.add_argument('-r', '--reweight', dest='reweight', action='store_true',
                        help='Reweight to make adversary layers flat')

    return parser.parse_args()


def min_error (y_true, y_pred):
    return K.min(K.square(y_true - y_pred), axis=-1)


class Adversary(keras.callbacks.Callback, Trainer):

    def __init__(self, data, smooths, **kwargs):
        Trainer.__init__(self, **kwargs)
        self.data = data
        self.smooths = smooths
        self.evaluated = None
        self.model = None
        self.num_epochs = 100

        K.set_value(self.my_model.optimizer.lr, 10.)

    def on_epoch_end(self, epoch, *_):
        if not epoch or epoch % 100:
            return

        # This is the classifier model
        self.evaluated = self.model.predict(self.data)
        # my_model is the adversary
        self.my_model.fit(self.evaluated, self.smooths,
                          validation_split=0.2,
                          epochs=self.num_epochs, batch_size=200000)
        # Drop after intial training
        self.num_epochs = 10
        K.set_value(self.my_model.optimizer.lr, 1.)

    def loss(self, y_true, y_pred):
        # Get loss from adversary
        if self.model is None:
            LOG.error('Model is None...')

#        classifier_result = self.model.evaluate(self.data)
#        adversary_score = self.my_model.evaluate(classifier_result, self.smooths)

        return categorical_crossentropy(y_true, y_pred)
#            adversary_score


if __name__ == '__main__':

    logging.debug('Args: %s', sys.argv)

    args = parse()

    numpy.random.seed(args.seed)

    logging.debug('Config: %s', args.config)

    inputs = [line.strip() for line in open(args.config, 'r') if not line.startswith('#')]
    smooth = args.adversary.split(',')

    labels, data, smooths, weights = load(args.input, args.target, inputs, smooth, args.weight, args.reweight)

    # Adversarial
    adversarial = Adversary(data=data,
                            smooths=smooths,
                            inputs=labels[0],
                            targets=smooths,
                            layers=20,
                            endsoft=False,
                            optimizer='adam',
                            loss=min_error)

    # Initial
    model = Trainer(inputs=inputs,
                    targets=labels,
                    adversary=adversarial,
                    layers=args.layers,
                    endsoft=True,
                    optimizer='rmsprop',
#                    loss='categorical_crossentropy',
                    metrics=['accuracy']                                 
                    )

    initial = model.my_model.fit(data, labels, validation_split=0.5,
                                 epochs=args.epochs, batch_size=args.batch,
                                 callbacks=[adversarial],
                                 )

    output_node = [n.op.name for n in model.my_model.outputs]
    logging.info('Output node: %s', output_node)

#    graph = graph_util.convert_variables_to_constants(sess, sess.graph.as_graph_def(), output_node)
#    graph_io.write_graph(graph, 'weights', args.output, as_text=False)

    argfilename = 'args.txt'
    with open(argfilename, 'w') as argfile:
        argfile.write('\n'.join(sys.argv))

    os.system('tar -czf weights/%s.tgz weights/%s %s %s' % (args.output, args.output, __file__, argfilename))