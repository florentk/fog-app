#!/usr/bin/python

import operator
import numpy

def compute_stats(data) :
	a = numpy.array(data)
	return  (a.mean(),a.std()/numpy.sqrt(a.size));


#read all result files and record it in a dict
results={500:[],1000:[],1500:[],3000:[],4500:[]}

for i in range(1,31):
	data = open('%d/rate.txt' % (i),'r').readlines()
	data_str = map(lambda x:x.strip('\n').split(' '),data) 
	for i in data_str : 
		(d,rate) =  int(i[0]),float(i[3])
		results[d] += [rate]



result_avg = dict([(d, compute_stats(results[d])) for d in results ])	

print "Densitie\trate\terr"

for i in  sorted(result_avg.items(), key=operator.itemgetter(0))	:
	d = i[0]
	rate = i[1]
	print "%d\t%0.5f\t%0.5f" % ( d , rate[0], rate[1])


	

