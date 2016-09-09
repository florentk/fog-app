#!/bin/sh
SRC=/home/florent/Projets/fog-app/simulations/defaults
DIR=$1
mkdir $DIR
for i in $SRC/*
do
	if [ -L $i  ]
	then 
		ln -s $i $DIR
	fi
done

mkdir $DIR/network
for i in $SRC/network/*
do
	if [ -L $i  ]
	then 
		ln -s $i $DIR/network
	fi
done
