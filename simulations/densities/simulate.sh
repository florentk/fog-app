#!/bin/sh
S=stopped-vehicle/alert-enabled
F=11
T=30
for i in `seq $F $T` 
do
  echo "------------------------"
  echo "Serie de simulations $i"
  echo "------------------------"
  cp common/highway.$i.sumo.cfg common/highway.sumo.cfg
  ../tools/simu-run.sh $S/config/* 
  ./results.sh $S $i 
done
