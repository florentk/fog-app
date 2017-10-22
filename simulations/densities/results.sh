#!/bin/sh
OLD=`pwd`
D=$1/result/$2
mkdir  $D
cd $1/config
for i in * 
do
	echo $i `../../../../tools/process_dump.py $i/log/sumo/netstate.dump` >> ../../../$D/TTC.txt
	#echo $i `../../tools/get_rate_slowed.sh $i/log/app/itsfogapplogfile.txt` >> ../$1/rate.txt
done
cd $OLD
