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


pp = PdfPages('rate.pdf')

fig = plt.figure()
ax = fig.add_subplot(111)

ax.errorbar(densities,rate,yerr=rate_err,ecolor='black', capsize = 10)	
		
ax.set_xlabel('Number of vehicles per hour')
ax.set_ylabel('Rate of vehicle which recept alert')
ax.set_xlim(0,5000)
ax.set_ylim(0.8,1.0)
#legend(loc=0)
	
pp.savefig()
pp.close()
