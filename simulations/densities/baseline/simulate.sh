#!/bin/sh
for i in `seq 1 10` 
do
  pwd
  echo "------------------------"
  echo "Serie de simulations $i"
  echo "------------------------"
  cp ../common/highway.standalone.$i.sumo.cfg ../common/highway.standalone.sumo.cfg 
  ./run.sh config/* 
  ./results.sh result/$i
done
