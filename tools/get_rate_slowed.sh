FILE=$1
N=`grep "enter in hazard zone at -1" $FILE | wc -l`
SLOW=`grep "enter in hazard zone" $FILE | wc -l`
RATE=`echo 1.0 - $N/$SLOW | bc -l`
echo $N $SLOW $RATE
