#!/bin/bash

sed  "s/SEED_NUMBER_INPUT/$RANDOM/g"   highway.sumo.cfg.in
