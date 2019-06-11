#! /bin/bash

for base in /data/t3home000/dabercro/models/190607_*
do

    for modelnum in 2 3 4 6 8  # 40,000, 60,000, 80,000, 120,000, and 160,000 steps
    do

        model=$(ls -1 $base/savedmodel/ | sed -n ${modelnum}p)

        freeze_graph --input_saved_model_dir=$base/savedmodel/$model/ \
                     --output_graph=freeze_$(basename $base)_$(echo "$modelnum * 20000" | bc).pb \
                     --output_node_names=dnn/split_logits/Slice || exit $?

    done

done
