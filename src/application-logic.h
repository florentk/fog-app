/****************************************************************************/
/// @file    application-logic.h
/// @author  Florent Kaisser
/// @date    29.09.2016
///
// iTETRIS Fog application related logic
#ifndef APPLICATIONLOGIC_H
#define APPLICATIONLOGIC_H

#define AP_MASK 0x10000
#define IsAP(i) (i & AP_MASK)

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
    /// @brief the current direction of the vehicle.
    float direction;   
    /// @brief Reference to behind vehicle's
    int behindId;
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
    static std::map<int,Vehicle> m_vehiclesInFog;

    /// @brief The collection of messages the results travel in
    static std::vector<AppMessage> m_messages;

    /// @brief The collection of vehicles, which subscribed to a RX subscription
    static std::map<int, bool> m_carRxSubs;
    
     /// @brief The collection of vehicles, which speed changed : pair of (nodeid,time) 
    static std::map<int, int> m_carLastSpeedChangeTime; 
    
     /// @brief The collection of vehicles, which slow start : pair of (nodeid,time) 
    static std::map<int, int> m_carLastSlowedTime;     

    /// @brief The collection of vehicles in the hazardous zone once : pair of (nodeid, time)
    static std::map<int, int> m_vehiclesInFogOnceTime;  
    
    /// @brief The time of last broadcast for each vehicle
    static std::map<int, int> m_lastBroadcast; 

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
    
    /// @brief Intializes where the vehicle break on arrival in fog   
    static int SetBreakInFog(bool actif);
    
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
    
    /// @brief Activ ou inactiv the  vehiclealert
    static int SetVehicleAlert(bool actif); 
    
    /// @brief Activ ou inactiv the RSU alert
    static int SetRSUAlert(bool actif); 
    
    /// @brief Activ ou inactiv the stopped vehicle alert
    static int SetStoppedVehicleAlert(bool actif);         

    /// @brief Intializes the Minimum interval between broadcast alert in second
    static int SetAlertInterval(int interval); 
        
     /// @brief Intializes the Duration of slowdown in second
    static int SetDurationOfSlowdown(int duration);   
    
    /// @brief enable or disable the resend alert when the vehicule is slowed
    static int SetNoAlertMessageIfIsSlowed(bool enable);     
    
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
    
    /// @brief return true if the vehicle has end slowed (current alert)
    static bool NodeEndingSlowed(int nodeId, int timestep);
    
    /// @brief return true if the vehicle is current slowed
    static bool NodeIsSlowed(int nodeId, int timestep);

    /// @brief return true if the fog is active
    static bool FogIsActive(int timestep);

    /// @brief compute the dist between idNode1 and idNode2
    static float ComputeDistance(int idNode1,int idNode2);
    
    /// @brief return the id of followed vehicle on same lane
    static int FirstVehicleOnSameLane(std::vector<Vehicle>& vehicles,std::vector<Vehicle>::const_iterator v);
    
    /// @brief return the distance with the behind vehicle's
    static float DistanceWithBehindVehicle(int idNode);    
    
    /// @brief return true if the node idNode is in a fog
    static bool IsInFog(int idNode);
    
    /// @brief return true if the vehicle is stopped
    static bool IsStoppedInFog(int idNode);  

    /// @brief return the vehicle of node
    static Vehicle GetVehicle(int idNode);
    
    /// @brief return true if the two nodes go in the same direction
    static bool IsSameDirection(int idNode1, int idNode2);
    
    /// @brief return true if node 2 is behind idNode1
    static bool IsBehind(int idNode1,int idNode2);
    
    /// @brief return true if the node idNode has received the message msg
    static bool msgIsReceivedByNode(AppMessage& msg, int idNode);
    
    /// @brief return true if time interval is expired 
    static bool IsTimeToSendAlert(int nodeId, int timestep);
    
    /// @brief return a initialised subscription storage
    /// @input subsType type of subscription
    /// @input cmdLength size of other data in subscription   
    /// @output mySubsStorage    
    static void InitSubscription(unsigned char subsType, unsigned char cmdLength, tcpip::Storage& mySubsStorage);

    /// @brief return a subscription storage for reception of geobroadcast messages by nodeId
    /// @input nodeId timestep
    /// @output mySubsStorage
    static void CreateGeobroadcastReceiveSubscription(int nodeId, int timestep, tcpip::Storage& mySubsStorage);
    
    /// @brief return a subscription storage for reception of AP messages by nodeId
    /// @input nodeId timestep
    /// @output mySubsStorage
    static void CreateAPReceiveSubscription(int nodeId, int timestep, tcpip::Storage& mySubsStorage);
        
	/// @brief update the status of a message
	static void UpdateAppMessageStatus(AppMessage& msg, TrafficApplicationResultMessageState newStatus);

	/// @brief Add a receiver of a message 
	static void AddReceiverAppMessage(AppMessage& msg, int receiverNodeId);
	
	/// @brief create a subscription to slow down vehicle
    static void CreateSlowDownTrafficSimSubscription(int timestep, int nodeId, float speedLimit, int durationOfSlowdown, tcpip::Storage& mySubsStorage);
    
    /// @brief create a subscription to set speed of a vehicle
    static void CreateSetSpeedTrafficSimSubscription(int timestep, int nodeId, float speedLimit, tcpip::Storage& mySubsStorage);
    
    /// @brief create a subscription to geobroadcast effectivelly the message identified by messageId 
    static void CreateGeobroadcastSendSubscription(int timestep, int senderId, int messageId, tcpip::Storage& mySubsStorage);	
    
    /// @brief create a subscription to AP send effectivelly the message identified by messageId 
    static void CreateAPSendSubscription(int timestep, int senderId, int messageId, tcpip::Storage& mySubsStorage);	    
      
    /// @brief process for break action
    static void Break(int timestep, int destinationId,  tcpip::Storage& mySubsStorage);
    
    /// @brief process for slowdown action
    static void StartSlowDown(int timestep, int destinationId, tcpip::Storage& mySubsStorage);
    
    /// @brief process when end of slowdown
    static void EndSlowDown(int timestep, int destinationId,  tcpip::Storage& mySubsStorage);
    
    /// @brief process when is alert expired
    static void ResetSpeed(int timestep, int destinationId, tcpip::Storage& mySubsStorage);
    
    /// @brief process action
    static bool LauchAction(int timestep, int action, int senderId , int destinationId, tcpip::Storage& mySubsStorage);
    
    /// @brief create a new message with initial status initStatus
    static void NewAppMessage(int timestep, int senderId, int action, TrafficApplicationResultMessageState initStatus);
    
    /// @brief update statistics
    static void ProcessStatistic(int timestep, int senderId, bool isSlowed);

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
    
    /// @brief Sending fog alert actif or not
    static bool m_alertFog;   
    
    /// @brief Sending RSU alert actif or not
    static bool m_alertRSU;   
    
    /// @brief Sending stopped vehicle alert actif or not
    static bool m_alertStoppedVehicle;           
    
    /// @brief Minimum interval between broadcast alert
    static int m_alertInterval; 
        
    /// @brief Duration of slowdown in second
    static int m_durationOfSlowdown;   
    
    /// @brief if the vehicule is slowed, no resend alert
    static bool m_noAlertMessageIfIsSlowed;  
    
    /// @brief if true, the vehicle break on arrival in fog   
    static bool m_vehicleBreakInFog;   
            
};

#endif
