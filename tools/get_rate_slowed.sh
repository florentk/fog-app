N=`grep "enter slowed at -1" log/app/itsfogapplogfile.txt | wc -l`
SLOW=`grep "enter" log/app/itsfogapplogfile.txt | wc -l`
RATE=`echo 1.0 - $N/$SLOW | bc -l`
echo $N $SLOW $RATE
