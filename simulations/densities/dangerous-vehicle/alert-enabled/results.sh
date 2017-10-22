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
	echo $i `../../../tools/process_dangerous.py config/$i/log/sumo/netstate.dump` >> $1/TTC.txt
	echo $i `../../../tools/process_trip.py config/$i/log/sumo/tripinfo` >> $1/tripinfo.txt
done
cd $OLD

#for i in config/*/log ; do sudo umount $i ; done
