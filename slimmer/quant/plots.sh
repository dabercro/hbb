#! /bin/bash

./validate_regression.py 190723_origin || exit 1
./validate_regression.py 190723_puppi || exit 1
./validate_regression.py 190725_lstm_pf || exit 1
./validate_regression.py 190904_0_2 || exit 1
./validate_regression.py 190924_0 || exit 1
