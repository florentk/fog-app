#!/bin/sh
OLD=`pwd`
mkdir -p $1
cd config
dirs=`ls`
cd ..
rm -f $1/TTC.txt  
rm -f $1/tripinfo.txt
for i in $dirs  
do
	echo $i `../../../tools/process_dump.py config/$i/log/sumo/netstate.dump` >> $1/TTC.txt
	echo $i `../../../tools/process_trip.py config/$i/log/sumo/tripinfo` >> $1/tripinfo.txt
	echo $i `../../../tools/get_rate_slowed.sh config/$i/log/app/itsfogapplogfile.txt` >> $1/rate.txt
done
cd $OLD

#for i in config/*/log ; do sudo umount $i ; done
