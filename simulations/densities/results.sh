#!/bin/sh
OLD=`pwd`
mkdir $1
cd with-ap 
for i in * 
do
	echo $i `../../tools/process_dump.py $i/log/sumo/netstate.dump` >> ../$1/TTC-enabled.txt
	echo $i `../../tools/get_rate_slowed.sh $i/log/app/itsfogapplogfile.txt` >> ../$1/rate.txt
done
cd $OLD
