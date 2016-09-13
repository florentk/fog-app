#!/bin/sh
for i in `seq 11 30` 
do
  cp common/highway.$i.sumo.cfg common/highway.sumo.cfg
  ../tools/simu-run.sh alert-*/*
  ./results.sh results/$i
done
