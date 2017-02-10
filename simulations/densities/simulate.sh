#!/bin/sh
for i in `seq 1 30` 
do
  echo "------------------------"
  echo "Serie de simulations $i"
  echo "------------------------"
  cp common/highway.$i.sumo.cfg common/highway.sumo.cfg
  ../tools/simu-run.sh with-ap/*
  ./results.sh results.withap/$i
done
