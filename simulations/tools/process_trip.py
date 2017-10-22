#!/usr/bin/python2

from bs4 import BeautifulSoup
import sys

def process_stat(vtype,soup):
  trips = soup.find_all('tripinfo',{"vtype":vtype})
  return "%f %f" % (sum([ float(t.get("duration"))  for t in trips]) / len(trips) , sum([ float(t.findChildren("emissions")[0].get("co2_abs"))  for t in trips]) / len(trips))


def main(filename):
  f = open(filename, "r" )
  xml = f.read()
  soup = BeautifulSoup(xml, "lxml") 
  
  
  
  print  process_stat("car1",soup), process_stat("car2",soup), process_stat("truck",soup)
  

if ( __name__ == "__main__"):
   try:
    main(sys.argv[1])
   except:
    print "bad args"
