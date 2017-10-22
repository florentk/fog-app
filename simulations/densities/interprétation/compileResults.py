#!/usr/bin/python

import operator
import numpy
import sys

def compute_stats(data) :
	a = numpy.array(data)
	return  (a.mean(),a.std()/numpy.sqrt(a.size));

def compile(name,col,nb):
  #read all result files and record it in a dict
  results={500:([],[],[],[]),1000:([],[],[],[]),1500:([],[],[],[]),3000:([],[],[],[]),4500:([],[],[],[])}

  for i in range(1,nb+1):
	  data_enabled = open('%d/%s-enabled.txt' % (i,name),'r').readlines()
	  data_disabled = open('%d/%s-disabled.txt' % (i,name),'r').readlines()	
	  data_baseline = open('%d/%s-baseline.txt' % (i,name),'r').readlines()
#	  data_v2i = open('%d/%s-v2i.txt' % (i,name),'r').readlines()	

	  data_enabled_str = map(lambda x:x.strip('\n').split(' '),data_enabled) 
	  data_disabled_str = map(lambda x:x.strip('\n').split(' '),data_disabled) 
	  data_baseline_str = map(lambda x:x.strip('\n').split(' '),data_baseline) 
#	  data_v2i_str = map(lambda x:x.strip('\n').split(' '),data_v2i) 

	  for i in data_enabled_str : 
		  (d,ttc) =  int(i[0]),float(i[col])
		  results[d] = (results[d][0]+[ttc],results[d][1],results[d][2],results[d][3])
		
	  for i in data_disabled_str : 
		  (d,ttc) =  int(i[0]),float(i[col])
		  results[d] = (results[d][0],results[d][1]+[ttc],results[d][2],results[d][3])

	  for i in data_baseline_str : 
		  (d,ttc) =  int(i[0]),float(i[col])
		  results[d] = (results[d][0],results[d][1],results[d][2]+[ttc],results[d][3])

#	  for i in data_v2i_str : 
#		  (d,ttc) =  int(i[0]),float(i[col])
#		  results[d] = (results[d][0],results[d][1],results[d][2],results[d][3]+[ttc])


#  result_avg = dict([(d, (compute_stats(results[d][0]), compute_stats(results[d][1]),compute_stats(results[d][2]), compute_stats(results[d][3])) ) for d in results ])	
  result_avg = dict([(d, (compute_stats(results[d][0]), compute_stats(results[d][1]),compute_stats(results[d][2])) ) for d in results ])	

  print "Densitie\twith alert V2V\terr\twithout alert\terr\tbase line\terr"
#  print "Densitie\twith alert V2V\terr\twithout alert\terr\tbase line\terr\twith alert v2i\terr"

  for i in  sorted(result_avg.items(), key=operator.itemgetter(0))	:
	  d = i[0]
	  ttc_e = i[1][0]
	  ttc_d = i[1][1]
	  ttc_b = i[1][2]
#	  ttc_i = i[1][3]
	  print "%d\t%0.5f\t%0.5f\t%0.5f\t%0.5f\t%0.5f\t%0.5f" % ( d , ttc_e[0], ttc_e[1],ttc_d[0],ttc_d[1],ttc_b[0],ttc_b[1])
#	  print "%d\t%0.5f\t%0.5f\t%0.5f\t%0.5f\t%0.5f\t%0.5f\t%0.5f\t%0.5f" % ( d , ttc_e[0], ttc_e[1],ttc_d[0],ttc_d[1],ttc_b[0],ttc_b[1],ttc_i[0],ttc_i[1])

  #reorganise data
#  result_zip = dict([(d, zip(results[d][0],results[d][1])) for d in results ])
  #compute the diff between result with alert enabled and alert disabled
#  result_dif = dict([(d, map( lambda x : x[1]-x[0], result_zip[d]) ) for d in result_zip ])
  #compute average and variance	
#  result_avg = dict([(d, compute_stats(result_dif[d])) for d in result_dif ])	
	
  #print result_dif

  #print "diff !!"

  #for i in  sorted(result_avg.items(), key=operator.itemgetter(0))	:
	#  print "%d %0.5f %0.5f" % (i[0],i[1][0],i[1][1])
	

if ( __name__ == "__main__"):
  compile(sys.argv[1],int(sys.argv[2]),int(sys.argv[3]))

