#!/bin/bash

nEvts=5000

for i in `seq 2 4`;
do
	./simulation.sh $i $nEvts
done
