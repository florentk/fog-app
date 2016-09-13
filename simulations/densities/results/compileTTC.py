#!/usr/bin/python

import operator
import numpy

def compute_stats(data) :
	a = numpy.array(data)
	return  (a.mean(),a.std()/numpy.sqrt(a.size));


#read all result files and record it in a dict
results={500:([],[]),1000:([],[]),1500:([],[]),3000:([],[]),4500:([],[])}

for i in range(1,31):
	data_enabled = open('%d/TTC-enabled.txt' % (i),'r').readlines()
	data_disabled = open('%d/TTC-disabled.txt' % (i),'r').readlines()	
	data_enabled_str = map(lambda x:x.strip('\n').split(' '),data_enabled) 
	data_disabled_str = map(lambda x:x.strip('\n').split(' '),data_disabled) 
	for i in data_enabled_str : 
		(d,ttc) =  int(i[0]),int(i[1])
		results[d] = (results[d][0]+[ttc],results[d][1])
		
	for i in data_disabled_str : 
		(d,ttc) =  int(i[0]),int(i[1])
		results[d] = (results[d][0],results[d][1]+[ttc])



result_avg = dict([(d, (compute_stats(results[d][0]), compute_stats(results[d][1])) ) for d in results ])	

print "Densitie\twith alert\terr\twithout alert\terr"

for i in  sorted(result_avg.items(), key=operator.itemgetter(0))	:
	d = i[0]
	ttc_e = i[1][0]
	ttc_d = i[1][1]
	print "%d\t%0.5f\t%0.5f\t%0.5f\t%0.5f" % ( d , ttc_e[0], ttc_e[1],ttc_d[0],ttc_d[1])

#reorganise data
result_zip = dict([(d, zip(results[d][0],results[d][1])) for d in results ])
#compute the diff between result with alert enabled and alert disabled
result_dif = dict([(d, map( lambda x : x[1]-x[0], result_zip[d]) ) for d in result_zip ])
#compute average and variance	
result_avg = dict([(d, compute_stats(result_dif[d])) for d in result_dif ])	
	
#print result_dif

#print "diff !!"

for i in  sorted(result_avg.items(), key=operator.itemgetter(0))	:
	print "%d %0.5f %0.5f" % (i[0],i[1][0],i[1][1])
	

