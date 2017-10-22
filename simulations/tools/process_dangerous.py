#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import xml.sax
import math

class VehicleHandler( xml.sax.ContentHandler ):
   def __init__(self):
      self.LENGHT = 6.0
      self.DIST_THRESHOLD = 100.0
      self.lane_id = ""
      self.time = ""
      self.last_pos = 0.0
      self.last_speed = 0.0
      self.last_id_vehic = ""
      self.first = True
      self.nb_dangerous = 0
      self.nb = 0;      

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
         else:
          dist = math.fabs(self.last_pos - pos)
          rel_speed = math.fabs(self.last_speed - speed)     
          if (rel_speed > 0.0 and id_vehic == "dangerous_car"):
            if (dist < self.DIST_THRESHOLD):
              self.nb_dangerous += 1 
	    self.nb += 1
          #print self.time, self.lane_id, id_vehic, pos, speed, self.last_id_vehic, self.last_pos, self.last_speed, dist, rel_speed      

          
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
    print float(Handler.nb_dangerous) / Handler.nb
   except Exception,e:
    print("Voici l'erreur :", e)

