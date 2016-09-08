#!/bin/bash

run(){
 D=$1
 mkdir -p log-$D/app log-$D/sumo
 echo "Start with $D"
 iCS -c itetris-config-file.$D.xml > log-$D/output.$D 2> log-$D/error.$D
}

for i in 500 1000 1500 3000 4500
do
	run $i
done
