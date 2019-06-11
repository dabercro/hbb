#! /bin/bash

./validate_regression.py 190607_nopf_narrowloss_120000 || exit 1
./validate_regression.py 190607_nopf_narrowloss_160000 || exit 1
./validate_regression.py 190607_nopf_narrowloss_40000 || exit 1
./validate_regression.py 190607_nopf_narrowloss_60000 || exit 1
./validate_regression.py 190607_nopf_narrowloss_80000 || exit 1
./validate_regression.py 190607_nopf_wideloss_120000 || exit 1
./validate_regression.py 190607_nopf_wideloss_160000 || exit 1
./validate_regression.py 190607_nopf_wideloss_40000 || exit 1
./validate_regression.py 190607_nopf_wideloss_60000 || exit 1
./validate_regression.py 190607_nopf_wideloss_80000 || exit 1
./validate_regression.py 190607_pf_narrowloss_120000 || exit 1
./validate_regression.py 190607_pf_narrowloss_160000 || exit 1
./validate_regression.py 190607_pf_narrowloss_40000 || exit 1
./validate_regression.py 190607_pf_narrowloss_60000 || exit 1
./validate_regression.py 190607_pf_narrowloss_80000 || exit 1
./validate_regression.py 190607_pf_wideloss_120000 || exit 1
./validate_regression.py 190607_pf_wideloss_160000 || exit 1
./validate_regression.py 190607_pf_wideloss_40000 || exit 1
./validate_regression.py 190607_pf_wideloss_60000 || exit 1
./validate_regression.py 190607_pf_wideloss_80000 || exit 1
./validate_regression.py 190603_v6 || exit 1
