#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import xml.sax
import math

class VehicleHandler( xml.sax.ContentHandler ):
   def __init__(self):
      self.LENGHT = 6.0
      self.THRESHOLD = 3.0
      self.lane_id = ""
      self.time = ""
      self.last_pos = 0.0
      self.last_speed = 0.0
      self.last_id_vehic = ""
      self.first = True
      self.nb_no_safety = 0
      self.nb_ttc = 0      
      self.nb_speed = 0
      self.cumul_speed = 0.0

   # Call when an element starts
   def startElement(self, tag, attributes):
      self.CurrentData = tag

      if tag == "timestep":
	 self.time = attributes["time"]   
 
      if tag == "lane":
         self.lane_id = attributes["id"]
         #print "*****", self.lane_id, "*****"
         self.last_pos = 0.0
         self.last_speed = 0.0
         self.first = True
         
      if tag == "vehicle":
	 id_vehic = attributes["id"]
         pos = float(attributes["pos"])
         speed = float(attributes["speed"])
         
         
         if(self.first):
          self.first = False
	 #elif (pos > 3000.0 and pos < 5000.0):
         elif(not ((self.lane_id == "gneE0_0" or self.lane_id == "gneE0_1") and  pos < 4000.0)):
          dist = math.fabs(self.last_pos - pos)
          rel_speed = math.fabs(self.last_speed - speed)     
          if (rel_speed == 0.0):
            ttc = 1000000.0    
          else:
            ttc = (dist - self.LENGHT) / rel_speed
            
          self.nb_ttc += 1
          if (ttc < self.THRESHOLD): 
            #print self.time, self.lane_id, id_vehic, pos, speed, self.last_id_vehic, self.last_pos, self.last_speed, dist, rel_speed, ttc
            self.nb_no_safety += 1
          if (speed > 0.0):
            self.nb_speed += 1
            self.cumul_speed += speed
          
         self.last_pos = pos
         self.last_speed = speed
         self.last_id_vehic = id_vehic

  
if ( __name__ == "__main__"):
   
   #print "self.time, self.lane_id, id_vehic, pos, speed, self.last_id_vehic, self.last_pos, self.last_speed, dist, rel_speed, ttc"
   # create an XMLReader
   parser = xml.sax.make_parser()
   # turn off namepsaces
   parser.setFeature(xml.sax.handler.feature_namespaces, 0)

   # override the default ContextHandler
   Handler = VehicleHandler()
   parser.setContentHandler( Handler )
   
   try:
    #parser.parse("log/sumo/netstate.dump")
    parser.parse(sys.argv[1])
    print Handler.nb_ttc, Handler.nb_no_safety, Handler.nb_no_safety / float(Handler.nb_ttc), Handler.cumul_speed / Handler.nb_speed
   except:
    print Handler.nb_ttc, Handler.nb_no_safety, Handler.nb_no_safety / float(Handler.nb_ttc), Handler.cumul_speed / Handler.nb_speed


