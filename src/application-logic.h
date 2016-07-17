/****************************************************************************/
/// @file    application-logic.h
/// @author  Julen Maneros
/// @date    10.10.2010
///
// iTETRIS Cooperative ITS demo application related logic
/****************************************************************************/
// iTETRIS; see http://www.ict-itetris.eu/
// Copyright 2008-2010 iTETRIS consortium
/****************************************************************************/
#ifndef APPLICATIONLOGIC_H
#define APPLICATIONLOGIC_H

#define SENDER_ID 2
#define SPEED_LIMIT 33

#include "foreign/tcpip/storage.h"
#include <vector>
#include <map>
#include <algorithm>

// ===========================================================================
// enumerations
// ===========================================================================
/**
 * @enum TrafficApplicationResultMessageState
 * @brief The status of the message in the simulation.
 */
enum TrafficApplicationResultMessageState {
    kToBeScheduled = 0,
    kScheduled,
    kArrived,
    kToBeApplied,
    kApplied,
    kToBeDiscarded,
    kMissed,
};

// ===========================================================================
// struct definitions
// ===========================================================================
/**
 * @struct AppMessage
 * @brief Information of a message carrying a result of the App
 */
struct AppMessage {
    /// @brief Unique message identifier
    int messageId;

    /// @brief Sender iCS station ID
    int senderId;

    /// @brief Destination iCS station ID
    int destinationId;

    /// @brief Message creation time step
    int createdTimeStep;

    /// @brief The action
    int action;
    
    /// @brief station IDs which received this message
    std::vector<int> receivedIds;

    /// @brief Message status
    TrafficApplicationResultMessageState status;
};

/**
 * @struct Area
 * @brief Definition of an 2D area
 */
struct Area {
    float x;
    float y;
    float radius;
};

/**
 * @struct Vehicle
 * @brief Definition of a vehicle
 */
struct Vehicle {
    int id;
    /// @brief x axis value of the position of the vehicle
    float x;
    /// @brief y axis value of the position of the vehicle
    float y;
    /// @brief current vehicle speed
    float speed;
};

// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class ApplicationLogic
 * @brief Encapsulates the logic of the ITS Application
 */
class ApplicationLogic
{
public:

    /// @brief The collection of vehicles in the CAM area
    static std::vector<Vehicle> m_vehiclesInFog;

    /// @brief The collection of messages the results travel in
    static std::vector<AppMessage> m_messages;

    /// @brief The collection of vehicles, which subscribed to a RX subscription
    static std::map<int, bool> m_carRxSubs;
    
     /// @brief The collection of vehicles, which speed changed : pair of (nodeid,time) 
    static std::map<int, int> m_carLastSpeedChangeTime; 

    /// @brief The collection of vehicles in the hazardous zone once : pair of (nodeid, time)
    static std::map<int, int> m_vehiclesInFogOnceTime;  

    /// @brief Counter to create different message IDs
    static int m_messageCounter;

    /// @brief Flag to indicate whether the CAM area subscription is set or not
    static bool m_camAreaIsSet;

    /// @brief Flag to indicate whether the returning car subscription is set or not
    static bool m_returnCarAreaIsSet;

    /// @brief Intializes CAM area values
    static int SetCamArea(float x, float y, float radius);

    /// @brief Intializes returning car area values
    static int SetCarArea(float x, float y, float radius);
    
    /// @brief Intializes returning fog area values
    static int SetFogArea(float x, float y, float radius);   
    
    /// @brief Intializes where the alert is propaging
    static int SetAlertRadius(float radius);
    
    /// @brief Intializes the time out of a alert
    static int SetAlertTimeOut(int alerttimeout); 
    
    /// @brief Intializes the speed limit on alert
    static int SetAlertSpeedlimit(float alertspeedlimit);         
     
    /// @brief Returns a CAM area
    static Area GetCamArea();

    /// @brief Returns the car area
    static Area GetReturningCarArea();  

    /// @brief Assigns a time step for the app to start returning values
    static int SetApplicationStartTimeStep(int timestep);

    /// @brief Assigns a time step for the fog is active returning values
    static int SetFogStartTimeStep(int timestep);
    
    /// @brief Assigns a time step for the fog is over returning values
    static int SetFogEndTimeStep(int timestep);   
    
    /// @brief Activ ou inactiv the alert
    static int SetAlertActif(bool actif); 

    /// @brief Skeleton to unsubcription. Demo app never requests it
    static bool DropSubscription(int subscriptionType);

    /// @brief Updates the current knowledge of vehicles in the area
    static bool ProcessSubscriptionCarsInZone(std::vector<Vehicle>& vehicles);

    /// @brief Sets sucessufuly received messages to be apply the payload in the next time step
    static bool ProcessMessageNotifications(std::vector<AppMessage>& messages);

    static float PartOfVehicleInFogSlowed(int idNode);

    /// @brief For every vehicle in the area assigns a result value
    static std::vector<AppMessage> SendBackExecutionResults(int senderId, int timestep);

    /// @brief Configure an APP_MSG_RECEIVE Subscription
    static tcpip::Storage RequestReceiveSubscription(int nodeId, int timestep);
    
    /// @brief Checks if within the application life_cycle, a specific subscription became required
    static tcpip::Storage CheckForRequiredSubscriptions (int nodeId, int timestep);
    
    /// @brief Checks if within the application life_cycle, a specific APP_MSG_SEND subscription became required
    //static tcpip::Storage CheckForAppMsgSendSubscriptions(int nodeId, int timestep);
    
    /// @brief Checks if within the application life_cycle, a specific APP_CMD_TRAFFIC_SIM subscription became required
   // static tcpip::Storage  CheckForAppTraffSimSubscriptions(int nodeId, int timestep);

    /// @brief Specific method to handle the reception of APP level message from the APP_MSG_RECEIVE subscription 
    static bool ProcessReceptionOfApplicationMessage(std::vector<AppMessage>& messages);


private:

    /// @brief return true if the alert of msg is expired
    static bool AlertIsExpired(int nodeId, int timestep);
    
    /// @brief return true if the vehicle is already slowed (current alert)
    static bool NodeIsSlowed(int nodeId, int timestep);
    
    /// @brief return true if the fog is active
    static bool FogIsActive(int timestep);
    
    /// @brief return true if the node idNode is in a fog
    static bool IsInFog(int idNode);
    
    /// @brief return true if the node idNode has received the message msg
    static bool msgIsReceivedByNode(AppMessage& msg, int idNode);

    /// @brief CAM area definition
    static Area m_camArea;

    /// @brief Returning car area definition
    static Area m_carArea;

    /// @brief The time step in which the app will send back results
    static int m_appStartTimeStep;
    
    /// @brief Returning fog area definition
    static Area m_fogArea;        
    
    /// @brief The time step in which the fog is active
    static int m_fogStartTimeStep;
    
    /// @brief The time step in which the fog is over
    static int m_fogEndTimeStep;    
    
    /// @brief Radius where the alert is propaging
    static float m_alertRadius;
    
    /// @brief The time out of a alert
    static int m_alertTimeOut; 
    
    /// @brief Speed limit on alert
    static float m_alertSpeedlimit;    
    
    /// @brief Sending alert actif or not
    static bool m_alertActif;               
};

#endif
