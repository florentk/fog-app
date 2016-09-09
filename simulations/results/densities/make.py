#!/usr/bin/env python
# -*- coding: utf-8 -*-

from pylab import plotfile, show, gca, figure, xlabel, ylabel, title, legend, xlim, ylim
from matplotlib.backends.backend_pdf import PdfPages



pp = PdfPages('reception_rate.pdf')

plotfile('reception_rate.txt', (0,1), delimiter='\t',newfig=True)			
xlabel('Number of vehicles per hour')
ylabel('Rate of vehicle which recept alert')
legend(loc=0)
	
pp.savefig()
pp.close()







