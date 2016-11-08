#! /bin/bash

#cd /home/angelo/git/inet_ext/simulations/test

#for ((i=1; $i<3; i++)); do
for i in {0..2160}
do
	echo "RUN $i"
	opp_run -r $i -u Cmdenv -c TestMobMassMobility -n ../../src:..:../../../../Programs/omnetpp-5.0/samples/inet/examples:../../../../Programs/omnetpp-5.0/samples/inet/src:../../../../Programs/omnetpp-5.0/samples/inet/tutorials -l ../../src/inet_ext -l ../../../../Programs/omnetpp-5.0/samples/inet/src/INET --debug-on-errors=false omnetpp.ini
	#opp_run -r $i -u Cmdenv -c TestMobGaussMarkovMobility -n ../../src:..:../../../../Programs/omnetpp-5.0/samples/inet/examples:../../../../Programs/omnetpp-5.0/samples/inet/src:../../../../Programs/omnetpp-5.0/samples/inet/tutorials -l ../../src/inet_ext -l ../../../../Programs/omnetpp-5.0/samples/inet/src/INET --debug-on-errors=false omnetpp.ini
done
