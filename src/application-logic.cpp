/****************************************************************************/
/// @file    application-logic.cpp
/// @author  Jerome Haerri, with help of Panos Matzakos and Remi Barbier 
/// @date    10.05.2012
///
// iTETRIS Cooperative ITS demo application related logic
/****************************************************************************/
// iTETRIS; see http://www.ict-itetris.eu/
// Copyright 2008-2010 iTETRIS consortium
/****************************************************************************/

// ===========================================================================
// included modules
// ===========================================================================
#include "application-logic.h"
#include "app-commands-subscriptions-constants.h"

#include <time.h>

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include "utils/log/log.h"

#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#include <unistd.h>
#endif

// ===========================================================================
// used namespaces
// ===========================================================================
using namespace std;

// ===========================================================================
// static member variables
// ===========================================================================
bool ApplicationLogic::m_camAreaIsSet = false;
bool ApplicationLogic::m_returnCarAreaIsSet = false;

vector<Vehicle> ApplicationLogic::m_vehiclesInArea;
vector<AppMessage> ApplicationLogic::m_messages;
map<int,bool> ApplicationLogic::m_carRxSubs;
int ApplicationLogic::m_messageCounter = 0;
Area ApplicationLogic::m_camArea;
Area ApplicationLogic::m_carArea;
int ApplicationLogic::m_appStartTimeStep;
Area ApplicationLogic::m_fogArea;        
int ApplicationLogic::m_fogStartTimeStep;
int ApplicationLogic::m_fogEndTimeStep;    
float ApplicationLogic::m_alertRadius;
int ApplicationLogic::m_alertTimeOut; 
float ApplicationLogic::m_alertSpeedlimit;    

// ===========================================================================
// method definitions
// ===========================================================================
Area
ApplicationLogic::GetCamArea()
{
    m_camAreaIsSet = true;
    return m_camArea;
}

Area
ApplicationLogic::GetReturningCarArea()
{
    m_returnCarAreaIsSet = true;
    return m_carArea;
}

bool
ApplicationLogic::DropSubscription(int subscriptionType)
{
   switch(subscriptionType) {
     case (SUB_SET_CAM_AREA):
     case (SUB_RETURNS_CARS_IN_ZONE):
     case (SUB_RECEIVED_CAM_INFO):
     case (SUB_APP_MSG_RECEIVE): {
       return false;
     }
     case (SUB_X_APPLICATION_DATA):
     case (SUB_APP_MSG_SEND):
     case (SUB_APP_CMD_TRAFF_SIM):
     case (SUB_APP_RESULT_TRAFF_SIM):
     case (SUB_FACILITIES_INFORMATION): {
       return true;
     }
     default: {
       return false;
     }
   } 
}

bool
ApplicationLogic::ProcessSubscriptionCarsInZone(vector<Vehicle>& vehicles)
{
    // Erase old data
    m_vehiclesInArea.clear();
    // Assign new data
    m_vehiclesInArea = vehicles;

    return true;
}

tcpip::Storage
ApplicationLogic::RequestReceiveSubscription(int nodeId, int timestep) {

    tcpip::Storage mySubsStorage;
    
    std::map<int,bool>::iterator it = m_carRxSubs.find(nodeId);
    if ((it == m_carRxSubs.end()) || ((*it).second == false)) { // means that car with ID nodeId has not registered a RX subscription yet
      if(it == m_carRxSubs.end()) {
        m_carRxSubs.insert(std::pair<int,bool>(nodeId,true)); 
      }        
      else {
        (*it).second == true;  // will not ask twice...
      }
    // preparing anwer...
    stringstream log;
    log << "APP --> [ApplicationLogic] RequestReceiveSubscription() Node ID " << nodeId << " subscribing to an APP_MSG_RECEIVE subscription at time "<<timestep<< "";
    Log::Write((log.str()).c_str(), kLogLevelInfo);
    
    // command length    
    mySubsStorage.writeUnsignedByte(1 + 1 + 1 + 1 + 1 + 4 + 2 + 1 + 1 + 2 + 4);
    // command type
    mySubsStorage.writeUnsignedByte(CMD_ASK_FOR_SUBSCRIPTION);
    // subscription type
    mySubsStorage.writeUnsignedByte(SUB_APP_MSG_RECEIVE);
    // HEADER_APP_MSG_TYPE
   // This demo-app only sends one type of message (to stop a vehicles): the code is 0x01 ; if we need to send another type, please change to a different number
    mySubsStorage.writeUnsignedByte(0x01);
    // Only destID
    mySubsStorage.writeUnsignedByte(0x04); 
    //Target Node id for unicast reception
    mySubsStorage.writeInt(nodeId);	// the UNICAST target/dest ID ; if not present, it would mean: ubiquitous mode
    //Length of rest of the command (payload length). 
    mySubsStorage.writeShort(500);
    //Unicast transmissions
    mySubsStorage.writeUnsignedByte(EXT_HEADER_TYPE_GEOBROADCAST);
    //Additional header specifying the sources
    mySubsStorage.writeUnsignedByte(EXT_HEADER__VALUE_BLOCK_IDs);
    //Number of Sources (currently only RSU) // if not present, it would mean: we listen to packets from any source node
    mySubsStorage.writeShort(1);
    //RSU ID is SENDER_ID
    mySubsStorage.writeInt(SENDER_ID); // TODO remove the hard coded RSU ID as source of the UNICAST reception	
   
  }
  return mySubsStorage;
}

bool
ApplicationLogic::ProcessReceptionOfApplicationMessage(vector<AppMessage>& messages){



    // Loop messages received by the APP_MSG_RECEIVE Subscription
    for (vector<AppMessage>::iterator it = messages.begin(); it != messages.end() ; ++it) {
        stringstream log;
        
        int destinationId = (*it).destinationId;
        log << "APP --> [ApplicationLogic] ProcessReceptionOfApplicationMessage() Message " << (*it).messageId << " received by " <<  destinationId;
        Log::Write((log.str()).c_str(), kLogLevelInfo); 
        
        
        
        // Match message with the registered by the App
        for (vector<AppMessage>::iterator it_ = m_messages.begin(); it_ != m_messages.end() ; ++it_) {
            int inputId = (*it).messageId; // received from the subscription
            int registeredId = (*it_).messageId; // id registered by App
            //TrafficApplicationResultMessageState status =  (*it_).status;
            
            if ((inputId == registeredId)/* && (status == kScheduled || status == kToBeApplied)*/) {
                stringstream log;
                
                (*it_).status = kToBeApplied;  // JHNOTE: changed from kScheduled to kToBeApplied -> registrering for a traf_sim subs
                (*it_).receivedIds.push_back(destinationId);
                
                log << "APP --> [ApplicationLogic] ProcessReceptionOfApplicationMessage() Message " << registeredId << " is registered and changed status from kScheduled to kToBeApplied";
                Log::Write((log.str()).c_str(), kLogLevelInfo); 
            } 
            else {
                 stringstream log;
                log << "APP --> [ApplicationLogic]  ProcessReceptionOfApplicationMessage() Message " << registeredId << " not registered.";
                Log::Write((log.str()).c_str(), kLogLevelWarning);
            }
        }
    }
    return  true;
}

// This method is kept here to keep the global framework of iTETRIS APPs, but it is never called, as not required by this version of the DEMO APP
bool
ApplicationLogic::ProcessMessageNotifications(vector<AppMessage>& messages)
{
    // Show registered messages (logging purposes only)
    if (m_messages.size() == 0) {
        Log::Write("ProcessMessageNotifications() There are no registered messages", kLogLevelInfo);
        return true;
    } else {
        stringstream messagesOut;
        for (vector<AppMessage>::iterator it = m_messages.begin(); it != m_messages.end() ; it++) {
            AppMessage message = *it;
            if (it == m_messages.begin()) {
                messagesOut << "ProcessMessageNotifications() Registered Messages: " << message.messageId << ", ";
            } else if (it == messages.end()) {
                messagesOut << message.messageId;
            } else {
                messagesOut << message.messageId << ", ";
            }
        }
        Log::Write((messagesOut.str()).c_str(), kLogLevelInfo);
    }

    return  true;
}

/* We cycle here at each request for new subscription and see if we have messages that are:
 *       - in stage kToBeScheduled. If so,
 *           we change to kScheduled and request to send a APP_MSG_SEND subscription.
 *       - in stage kToBeApplied, If so,
 *           we change to kApplied and request to send a APP_CMD_TRAFFIC_SIM subscription
*/
tcpip::Storage 
ApplicationLogic::CheckForRequiredSubscriptions (int nodeId, int timestep){
  
  tcpip::Storage mySubsStorage;

  for (vector<AppMessage>::iterator it_ = m_messages.begin(); it_ != m_messages.end() ; ++it_) {
       //if no message received by nodeId => not process the message
       if ( ! msgIsReceivedByNode(*it_, nodeId) ) continue;
  
       if (((*it_).status == kToBeScheduled)/*  ((*it_).senderId == nodeId)*/) { // we need to send an APP_MSG_SEND and it should be a subscription created by me
           
           (*it_).status = kScheduled;  // changed from kToBeScheduled to kScheduled
          
           stringstream log;
           log << "APP --> [ApplicationLogic] CheckForRequiredSubscriptions() Message " << (*it_).messageId << " is changed status from kToBeScheduled to kScheduled";
           Log::Write((log.str()).c_str(), kLogLevelInfo); 

           //Command length
	       mySubsStorage.writeUnsignedByte(1 + 1 + 1 + 1 + 1 + 1 + 1 + 4 + 1 + 2 + 4 + 1 + 1 + 1 + 1 + 4+ 4 + 4);
	       //Command type
	       mySubsStorage.writeUnsignedByte(CMD_ASK_FOR_SUBSCRIPTION);
	       mySubsStorage.writeUnsignedByte(SUB_APP_MSG_SEND);
	       mySubsStorage.writeUnsignedByte(0x01); //HEADER_APP_MSG_TYPE 
               // This demo-app only sends one type of message (to stop a vehicles): the code is 0x01 ; if we need to send another type, please change to a different number
	       mySubsStorage.writeUnsignedByte(0x0F);  // in bits, it is: 1111 : comm profile, the prefered techno and a senderID and the message lifetime
	       mySubsStorage.writeUnsignedByte(0xFF);  // unsigned char preferredRATs = 0xFF;
	       mySubsStorage.writeUnsignedByte(0xFF);  // unsigned char commProfile = 0xFF;
	       mySubsStorage.writeInt((*it_).senderId);
	       mySubsStorage.writeUnsignedByte(2);     // unsigned int msgLifetime = 2;
               mySubsStorage.writeShort(1024); // m_app_Msg_length
	       mySubsStorage.writeInt((*it_).messageId); //sequence number
	       mySubsStorage.writeUnsignedByte(EXT_HEADER_TYPE_GEOBROADCAST); // CommMode
	       mySubsStorage.writeUnsignedByte(EXT_HEADER__VALUE_BLOCK_AREAs); // CommMode
	       mySubsStorage.writeUnsignedByte(1);
	       /*mySubsStorage.writeInt((*it_).destinationId);*/
	       
	       mySubsStorage.writeUnsignedByte(EXT_HEADER__CIRCLE); //Area type 
	       mySubsStorage.writeFloat(m_fogArea.x); //x
	       mySubsStorage.writeFloat(m_fogArea.y); //y
	       mySubsStorage.writeFloat(m_fogArea.radius + m_alertRadius);	//radius   		           

           break;  // we break on each successful occurence, as the iCS can only read ONE subscription at a time..we will be called again by the iCS as long as we have something to request
       }
       else if (((*it_).status == kToBeApplied)/* && ((*it_).senderId == nodeId)*/) { // we need to send an APP_CMD_TRAFFIC_SIM subscription
           (*it_).status = kApplied;  // changed from kToBeApplied to kApplied 
          
           stringstream log;
           log << "APP --> [ApplicationLogic] CheckForRequiredSubscriptions() Message " << (*it_).messageId << " on " << nodeId << " is changed status from kToBeApplied to kApplied";
           Log::Write((log.str()).c_str(), kLogLevelInfo); 
           
           for (vector<int>::iterator itDestIds = (*it_).receivedIds.begin(); itDestIds != (*it_).receivedIds.end() ; ++itDestIds) {
                   int destinationId = (*itDestIds);
                   //itDestIds=(*it_).receivedIds.erase(itDestIds);
           
                   //Command length
	           mySubsStorage.writeUnsignedByte(1 + 1 + 1 + 1 + 1 + 4 + 4);
	           //Command type
	           mySubsStorage.writeUnsignedByte(CMD_ASK_FOR_SUBSCRIPTION);
	           mySubsStorage.writeUnsignedByte(SUB_APP_CMD_TRAFF_SIM);
	           mySubsStorage.writeUnsignedByte(0x01); //HEADER_APP_MSG_TYPE
	           mySubsStorage.writeUnsignedByte(VALUE_SET_SPEED); //to change the speed
	           mySubsStorage.writeInt(destinationId); //Destination node to set its maximum speed.
	           mySubsStorage.writeFloat(m_alertSpeedlimit); //Set Maximum speed
	           
	           stringstream log;
                   log << "APP --> [ApplicationLogic] CheckForRequiredSubscriptions() Node Vehicle " << destinationId << " slowed !";
                   Log::Write((log.str()).c_str(), kLogLevelInfo); 
            }
	   }else if ( (*it_).status == kApplied && AlertIsExpired(*it_,timestep)){
	       //Restore initial speed limit
           for (vector<int>::iterator itDestIds = (*it_).receivedIds.begin(); itDestIds != (*it_).receivedIds.end() ; ) {
                   int destinationId = (*itDestIds);
                   itDestIds=(*it_).receivedIds.erase(itDestIds);
           
                   //Command length
	           mySubsStorage.writeUnsignedByte(1 + 1 + 1 + 1 + 1 + 4 + 4);
	           //Command type
	           mySubsStorage.writeUnsignedByte(CMD_ASK_FOR_SUBSCRIPTION);
	           mySubsStorage.writeUnsignedByte(SUB_APP_CMD_TRAFF_SIM);
	           mySubsStorage.writeUnsignedByte(0x01); //HEADER_APP_MSG_TYPE
	           mySubsStorage.writeUnsignedByte(VALUE_SET_SPEED); //to change the speed
	           mySubsStorage.writeInt(destinationId); //Destination node to set its maximum speed.
	           mySubsStorage.writeFloat(SPEED_LIMIT); //Set Maximum speed
	           
	           stringstream log;
                   log << "APP --> [ApplicationLogic] CheckForRequiredSubscriptions() Node Vehicle " << destinationId << " speed restored !";
                   Log::Write((log.str()).c_str(), kLogLevelInfo); 
            }	   
	   }
  }
  return  mySubsStorage;
}

// even though we do not need to return a value to the iCS result container (we use the VOID_RESULT_CONTAINER),  
// we still need to initialize the procedure...
vector<AppMessage>
ApplicationLogic::SendBackExecutionResults(int senderId, int timestep)
{
   vector<AppMessage> results;
   if (     timestep >= m_appStartTimeStep 
       &&   FogIsActive(timestep) 
       &&   IsInFog(senderId)) { 
        // Loop vehicles in the area, one message per vehicle
        stringstream log;  
        
        log << "APP --> [ApplicationLogic] SendBackExecutionResults() creating new messages for ID "<< senderId << " for vehicles..." ;
           

        AppMessage message;
        message.messageId = ++m_messageCounter;
        message.status = kToBeScheduled;  // JHNOTE: registers a APP_MSG_Sends subscription 
        message.senderId = senderId;
        message.destinationId = 0; //broadcast !
        message.createdTimeStep = timestep;
        message.payloadValue = m_alertSpeedlimit; 
        log<< " ID " << message.destinationId << " with new speed " << message.payloadValue<< " Message is now kToBeScheduled ";
        m_messages.push_back(message);

        Log::Write((log.str()).c_str(), kLogLevelInfo); 

        // Keep in safe place all the results to send back to the iCS
        results = m_messages;
    }
    
    // Loop current messages to find those to be applied and erase from the result record
    for (vector<AppMessage>::iterator it = m_messages.begin() ; it != m_messages.end() ; it++) {
        //if ((*it).status == kApplied) {  
        if ((*it).status == kToBeDiscarded) {
            m_messages.erase(it);
            stringstream log;
           	log << "APP --> [ApplicationLogic] CheckForRequiredSubscriptions() Message " << (*it).messageId << " was kApplied...has been removed, as the cycle is now complete.";
          	Log::Write((log.str()).c_str(), kLogLevelInfo); 
        }
    }

    return results;
}

bool 
ApplicationLogic::AlertIsExpired(AppMessage& msg, int timestep){
    return msg.status == kApplied && (timestep > msg.createdTimeStep+m_alertTimeOut);
}

bool 
ApplicationLogic::FogIsActive(int timestep){
    return (m_fogStartTimeStep <= timestep) && (timestep < m_fogEndTimeStep);
}

bool
ApplicationLogic::IsInFog(int idNode){

    for (vector<Vehicle>::const_iterator it = m_vehiclesInArea.begin() ; it != m_vehiclesInArea.end() ; it++) {
        if ((*it).id == idNode)
            return true;
    }
    
    return false;
}

bool
ApplicationLogic::msgIsReceivedByNode(AppMessage& msg, int idNode){

    for (vector<int>::const_iterator itDestIds = msg.receivedIds.begin() ; itDestIds != msg.receivedIds.end() ; itDestIds++) {
        if ((*itDestIds) == idNode)
            return true;
    }
    
    return false;
}

int
ApplicationLogic::SetCamArea(float x, float y, float radius)
{

stringstream log;
    log << "CAM Area : " << "(" << x << "," << y << "," << radius << ")";
    Log::Write((log.str()).c_str(), kLogLevelInfo); 

    if (radius <= 0) {
        Log::Write("Radius of the CAM area is 0 or negative.", kLogLevelError);
        return EXIT_FAILURE;
    }

    m_camArea.x = x;
    m_camArea.y = y;
    m_camArea.radius = radius;

    return EXIT_SUCCESS;
}

int
ApplicationLogic::SetCarArea(float x, float y, float radius)
{
    stringstream log;
    log << "Car Area : " << "(" << x << "," << y << "," << radius << ")";
    Log::Write((log.str()).c_str(), kLogLevelInfo); 

    if (radius <= 0) {
        Log::Write("Radius of the returning car area is 0 or negative.", kLogLevelError);
        return EXIT_FAILURE;
    }

    m_carArea.x = x;
    m_carArea.y = y;
    m_carArea.radius = radius;

    return EXIT_SUCCESS;
}

int
ApplicationLogic::SetApplicationStartTimeStep(int timestep)
{
    stringstream log;
    log << "Start time : " << timestep;
    Log::Write((log.str()).c_str(), kLogLevelInfo); 

    if (timestep < 0) {
        Log::Write("Application start time step is negative", kLogLevelError);
        return EXIT_FAILURE;
    }

    m_appStartTimeStep = timestep;

    return EXIT_SUCCESS;
}


int ApplicationLogic::SetFogArea(float x, float y, float radius)
{
    stringstream log;
    log << "Fog Area : " << "(" << x << "," << y << "," << radius << ")";
    Log::Write((log.str()).c_str(), kLogLevelInfo); 

    if (radius <= 0) {
        Log::Write("Radius of the returning fog area is 0 or negative.", kLogLevelError);
        return EXIT_FAILURE;
    }

    m_fogArea.x = x;
    m_fogArea.y = y;
    m_fogArea.radius = radius;
    return EXIT_SUCCESS;     
}

int ApplicationLogic::SetFogStartTimeStep(int timestep)
{
    stringstream log;
    log << "Start fog time : " << timestep;
    Log::Write((log.str()).c_str(), kLogLevelInfo); 

    if (timestep < 0) {
        Log::Write("Fog start time step is negative", kLogLevelError);
        return EXIT_FAILURE;
    }

    m_fogStartTimeStep = timestep;
    return EXIT_SUCCESS;     
}    

int ApplicationLogic::SetFogEndTimeStep(int timestep)
{
    stringstream log;
    log << "End fog time : " << timestep;
    Log::Write((log.str()).c_str(), kLogLevelInfo); 

    if (timestep < 0) {
        Log::Write("Fog over time step is negative", kLogLevelError);
        return EXIT_FAILURE;
    }
    
    m_fogEndTimeStep = timestep;
    return EXIT_SUCCESS;     
}        

int ApplicationLogic::SetAlertRadius(float radius)
{
    stringstream log;
    log << "Alert radius : " << radius;
    Log::Write((log.str()).c_str(), kLogLevelInfo); 

    if (radius <= 0) {
        Log::Write("Radius of alert is 0 or negative.", kLogLevelError);
        return EXIT_FAILURE;
    }
    
    m_alertRadius = radius;

    return EXIT_SUCCESS;     
}    

int ApplicationLogic::SetAlertTimeOut(int alerttimeout)
{
    stringstream log;
    log << "Alert timeout : " << alerttimeout;
    Log::Write((log.str()).c_str(), kLogLevelInfo); 

    if (alerttimeout < 0) {
        Log::Write("Alert time out is negative", kLogLevelError);
        return EXIT_FAILURE;
    }
    
    m_alertTimeOut = alerttimeout;

    return EXIT_SUCCESS;     
}     

int ApplicationLogic::SetAlertSpeedlimit(float alertspeedlimit)
{
    stringstream log;
    log << "Alert speed limit : " << alertspeedlimit;
    Log::Write((log.str()).c_str(), kLogLevelInfo); 

    if (alertspeedlimit < 0) {
        Log::Write("Alert speed limit is negative", kLogLevelError);
        return EXIT_FAILURE;
    }
    
    m_alertSpeedlimit = alertspeedlimit;

    return EXIT_SUCCESS;     
}       


