for i in `seq 1 30` ; 
do 
	./generate.sh highway.sumo.cfg.in > highway.$i.sumo.cfg 
done

for i in `seq 1 30` ; 
do 
	./generate.sh highway.standalone.sumo.cfg.in > highway.standalone.$i.sumo.cfg 
done

