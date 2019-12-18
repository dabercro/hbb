#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190417_breg_new%2Fhbb_m_tf_v10__hbb_m_tf_v9__hbb_m_tf_v2__hbb_m%2C190417_breg_old%2Fhbb_m_tf_v10__hbb_m_tf_v9__hbb_m_tf_v2__hbb_m"

for f in */*.pdf
do
    pdfcrop $f $f
done
