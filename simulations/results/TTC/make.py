#!/usr/bin/env python
# -*- coding: utf-8 -*-

#from pylab import plotfile, show, gca, figure, xlabel, ylabel, title, legend, xlim, ylim


import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages


data = open('TTC.txt','r').readlines()
data_str = map(lambda x:x.strip('\n').split('\t'),data)
densities = map(lambda x: int(x[0])  , data_str[1:]  )
ttc_whith_alert = map(lambda x: int(x[1])  , data_str[1:]  )
ttc_without_alert = map(lambda x: int(x[2])  , data_str[1:]  )


print densities
print ttc_whith_alert
print ttc_without_alert


ind = np.arange(len(ttc_whith_alert))                # the x locations for the groups
width = 0.35                      # the width of the bars


pp = PdfPages('Cumul of TTC.pdf')

fig = plt.figure()
ax = fig.add_subplot(111)

rects1 = ax.bar(ind, ttc_without_alert, width, color='red')
rects2 = ax.bar(ind+width, ttc_whith_alert , width, color='green')

ax.set_xlim(-width,len(ind)+width)
ax.set_ylim(0,17)
ax.set_xlabel('Number of vehicles per hour')
ax.set_ylabel('Cumul of TTC < 3 s')


ax.set_xticks(ind+width)
xtickNames = ax.set_xticklabels(densities)
plt.setp(xtickNames, fontsize=10)

## add a legend
ax.legend( (rects1[0], rects2[0]), (data_str[0][2], data_str[0][1]) )

#plt.show()


	
pp.savefig()
pp.close()







