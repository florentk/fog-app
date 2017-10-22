#!/bin/bash

run(){
 D=$1
 OLD=`pwd`
 cd $D
 echo "Start with $D"
# sudo ./mount_log.sh
 mkdir -p   log/sumo
 sumo -c network/highway.standalone.sumo.cfg > log/output 2> log/error
 cd $OLD
}

for i in $*
do
        run $i
done

