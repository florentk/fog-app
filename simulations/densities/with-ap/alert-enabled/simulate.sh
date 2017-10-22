#!/bin/sh
for i in `seq 1 10` 
do
  pwd
  echo "------------------------"
  echo "Serie de simulations $i"
  echo "------------------------"
  cp ../../common/highway.$i.sumo.cfg ../../common/highway.sumo.cfg
  ./run.sh config/* 
  ./results.sh result/$i
done
