#!/usr/bin/env python
# -*- coding: utf-8 -*-

from pylab import plotfile, show, gca, figure, xlabel, ylabel, title, legend, xlim, ylim
from matplotlib.backends.backend_pdf import PdfPages
import matplotlib.pyplot as plt


data = open('rate.mean','r').readlines()
data_str = map(lambda x:x.strip('\n').split('\t'),data)
densities = map(lambda x: int(x[0])  , data_str[1:]  )

rate = map(lambda x: float(x[1])  , data_str[1:]  )
rate_err = map(lambda x: float(x[2])  , data_str[1:]  )


data = open('rate-v2i.mean','r').readlines()
data_str = map(lambda x:x.strip('\n').split('\t'),data)
rate_v2i = map(lambda x: float(x[1])  , data_str[1:]  )
rate_v2i_err = map(lambda x: float(x[2])  , data_str[1:]  )


pp = PdfPages('rate.pdf')

fig = plt.figure()
ax = fig.add_subplot(111)

ax.errorbar(densities,rate,yerr=rate_err,ecolor='black', capsize = 10)	
v2v = ax.plot(densities,rate,marker='+',mec='k',ls='solid',lw=2,color='green')	
ax.errorbar(densities,rate_v2i,yerr=rate_v2i_err,ecolor='black', capsize = 10)	
v2i = ax.plot(densities,rate_v2i,marker='x',mec='k',ls='solid',lw=2,color='yellow')	
		
ax.set_xlabel('Number of vehicles per hour')
ax.set_ylabel('Reception Rate')
ax.set_xlim(0,5000)
ax.set_ylim(0.0,1.2)
#legend(loc=0)
ax.legend( (v2v[0],v2i[0]), ("Fog alert V2V","Fog alert V2I"),loc='lower right' )
	
pp.savefig()
pp.close()
