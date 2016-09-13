#!/usr/bin/env python
# -*- coding: utf-8 -*-

from pylab import plotfile, show, gca, figure, xlabel, ylabel, title, legend, xlim, ylim
from matplotlib.backends.backend_pdf import PdfPages
import matplotlib.pyplot as plt


data = open('TTC_diff.mean','r').readlines()
data_str = map(lambda x:x.strip('\n').split(' '),data)
densities = map(lambda x: int(x[0])  , data_str  )
ttc_diff = map(lambda x: float(x[1])  , data_str  )
ttc_diff_err = map(lambda x: float(x[2])  , data_str  )


pp = PdfPages('TTC_diff.pdf')

fig = plt.figure()
ax = fig.add_subplot(111)

ax.errorbar(densities,ttc_diff,yerr=ttc_diff_err,ecolor='black', capsize = 10)	
		
ax.set_xlabel('Number of vehicles per hour')
ax.set_ylabel('Difference of cumul of TTC with et without alert')
#legend(loc=0)
	
pp.savefig()
pp.close()
