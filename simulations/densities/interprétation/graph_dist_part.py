#!/usr/bin/env python
# -*- coding: utf-8 -*-

from pylab import plotfile, show, gca, figure, xlabel, ylabel, title, legend, xlim, ylim
from matplotlib.backends.backend_pdf import PdfPages
import matplotlib.pyplot as plt


data = open('dist.mean','r').readlines()
data_str = map(lambda x:x.strip('\n').split('\t'),data)

densities = map(lambda x: int(x[0])  , data_str[1:]  )
data_v2v = map(lambda x: float(x[1])  , data_str[1:]  )
data_v2v_err = map(lambda x: float(x[2])  , data_str[1:]  )
data_disabled = map(lambda x: float(x[3])  , data_str[1:]  )
data_disabled_err = map(lambda x: float(x[4])  , data_str[1:]  )
#data_baseline = map(lambda x: float(x[5])  , data_str[1:]  )
#data_baseline_err = map(lambda x: float(x[6])  , data_str[1:]  )
#data_v2i = map(lambda x: float(x[7])  , data_str[1:]  )
#data_v2i_err = map(lambda x: float(x[8])  , data_str[1:]  )




pp = PdfPages('dist_part.pdf')

fig = plt.figure()
ax = fig.add_subplot(111)

ax.errorbar(densities,data_v2v,yerr=data_v2v_err,ecolor='black', capsize = 10)	
v2v = ax.plot(densities,data_v2v,marker='+',mec='k',ls='solid',lw=2,color='green')	

#ax.errorbar(densities,data_v2i,yerr=data_v2i_err,ecolor='black', capsize = 10)	
#v2i = ax.plot(densities,data_v2i,marker='x',mec='k',ls='solid',lw=2,color='yellow')	

ax.errorbar(densities,data_disabled,yerr=data_disabled_err,ecolor='black', capsize = 10)	
disabled = ax.plot(densities,data_disabled,marker='*',mec='k',ls='solid',lw=2,color='red')	

#ax.errorbar(densities,data_baseline,yerr=data_baseline_err,ecolor='black', capsize = 10)	
#baseline = ax.plot(densities,data_baseline,marker='o',mec='k',ls='solid',lw=2,color='blue')	


		
ax.set_xlabel('Number of vehicles per hour')
ax.set_ylabel('Part of time with distance from dangerous vehicle < 100 m')
ax.set_xlim(0,5000)
#ax.set_ylim(0.0,0.006)
ax.set_ylim(0.0,0.3)

#legend(loc=0)
#ax.legend( (v2v[0],v2i[0],disabled[0],baseline[0]), ("Fog alert V2V","Fog alert V2I","No alert","Baseline"),loc='lower right' )
ax.legend( (v2v[0],disabled[0]), ("alert V2V","no alert"),loc='lower right' )
	
pp.savefig()
pp.close()
