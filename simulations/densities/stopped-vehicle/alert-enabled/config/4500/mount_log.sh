#!/bin/bash
mkdir -p  log
if mount -t ramfs -o size=512M,maxsize=512M /dev/ram0 log 
then
	mkdir  log/ics log/ns log/app log/sumo
	chmod -R 777 log
fi
