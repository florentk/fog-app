#!/usr/bin/python

import operator
import numpy

def compute_stats(data) :
	a = numpy.array(data)
	return  (a.mean(),a.std()/numpy.sqrt(a.size));


#read all result files and record it in a dict
results={500:([],[],[]),1000:([],[],[]),1500:([],[],[]),3000:([],[],[]),4500:([],[],[])}

for i in range(1,31):
	data_enabled = open('../alert-enabled/result/%d/TTC.txt' % (i),'r').readlines()
	data_enabled_str = map(lambda x:x.strip('\n').split(' '),data_enabled) 
	data_disabled = open('../alert-disabled/result/%d/TTC.txt' % (i),'r').readlines()	
	data_disabled_str = map(lambda x:x.strip('\n').split(' '),data_disabled) 
	data_control = open('../control/result/%d/TTC.txt' % (i),'r').readlines()	
	data_control_str = map(lambda x:x.strip('\n').split(' '),data_control) 

	for i in data_enabled_str : 
		(d,ttc) =  int(i[0]),int(i[1])
		results[d] = (results[d][0]+[ttc],results[d][1],results[d][2])

	for i in data_disabled_str : 
		(d,ttc) =  int(i[0]),int(i[1])
		results[d] = (results[d][0],results[d][1]+[ttc],results[d][2])
	
	for i in data_control_str : 
		(d,ttc) =  int(i[0]),int(i[1])
		results[d] = (results[d][0],results[d][1],results[d][2]+[ttc])


result_avg = dict([(d, (compute_stats(results[d][0]), compute_stats(results[d][1]), compute_stats(results[d][2])) ) for d in results ])	

print "Densitie\tone stopped vehicle with alert\terr\tone stopped vehicle without alert\terr\tnot stopped vehicle\terr"

for i in  sorted(result_avg.items(), key=operator.itemgetter(0))	:
	d = i[0]
	ttc_e = i[1][0]
	ttc_d = i[1][1]
	ttc_c = i[1][2]
	print "%d\t%0.5f\t%0.5f\t%0.5f\t%0.5f\t%0.5f\t%0.5f" % ( d , ttc_e[0], ttc_e[1],ttc_d[0],ttc_d[1],ttc_c[0],ttc_c[1])

