#!/bin/zsh

rm -rf results_hw
mkdir results_hw

# load omnetpp.txt to omnetpp.ini
cp omnetpp.txt omnetpp.ini

for i in {5..100}; do
    MEAN_IAT=$(echo "scale=2; $i/5" | bc)
    echo "Current MEAN_IAT is: $MEAN_IAT"
    
    # Writing the value directly into the ini file
    echo "**.gen.sendIaTime = exponential(${MEAN_IAT}s)\noutput-vector-file = results_hw/${MEAN_IAT}_run_\${runnumber}.vec" >> omnetpp.ini

    touch results_hw/$MEAN_IAT.txt
    # Run the simulation
    echo "MEAN_IAT=$MEAN_IAT" >> results_hw/$MEAN_IAT.txt
    ./HW4 -u Cmdenv >> results_hw/$MEAN_IAT.txt

    # Restore the backup ini file
    sed -i '' -e '$ d' omnetpp.ini
    sed -i '' -e '$ d' omnetpp.ini
    rm results_hw/*.vci
done
