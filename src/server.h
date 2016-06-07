/****************************************************************************/
/// @file    server.h
/// @author  Julen Maneros
/// @date    10.10.2010
///
// iTETRIS Cooperative ITS demo server logic
/****************************************************************************/
// iTETRIS; see http://www.ict-itetris.eu/
// Copyright 2008-2010 iTETRIS consortium
/****************************************************************************/
#ifndef SERVER_H
#define SERVER_H

// ===========================================================================
// included modules
// ===========================================================================
#include <fstream>
#include "foreign/tcpip/socket.h"
#include "foreign/tcpip/storage.h"
#include "application-logic.h"

// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class Server
 * @brief Encapsulates the functions that communicate the App with the iCS using a socket
 */
class Server
{
public:

    /// @brief Starts the server listening for commands on the socket port of the parameter
    static void processCommands(int port);

private:
    /// @brief Server constructor
    Server(int port);

    /// @brief Server desstructor
    ~Server(void);

    /// @brief Redirects the received command from the iCS to the appropriate function
    int dispatchCommand();

    /// @brief Process the logic of the command to set new subscriptions in the iCS
    bool LookForNewSubscriptions();

    /// @brief Prepares data in the socket to set a "return cars in zone" subscription
    bool SubsReturnCarsInZone(tcpip::Storage& myOutputStorage, int nodeId, int timestep, Area area);

    /// @brief Prepares data in the socket to set a CAM area scheduling subscription
    bool SubsSetCamArea(tcpip::Storage& myOutputStorage, int nodeId, int timestep, Area area);

    /// @brief Prepares data in the socket to tell the iCS to stop asking for new subscriptions
    bool StopAskingForSubscription(tcpip::Storage& myOutputStorage, int nodeId, int timestep);

    /// @brief Process the logic of the command to drop subscriptions in the iCS. NOT USED
    bool CheckSubscriptionStatus();

    /// @brief Process the information to the "return cars in zone" subscription
    bool ReceiveSubscriptionCarsInZone();

    /// @brief Process the command of the messages carrying the result payload
    bool ReceiveAppMessageStatusNotification();

    /// @brief Process the command to tell the App it can execute the logic
    bool ExecuteApplication();

    /// @brief Requests Subscriptions as function of the Application logic 
   // bool RequestSubscriptions (tcpip::Storage& myOutputStorage, int nodeId, int timestep, tcpip::Storage& mySubsStorage);
    
    /// @brief Requests A recuring APP_MSG_RECEIVE Subscription 
   // bool SubAppMessageReceive (tcpip::Storage& myOutputStorage, int nodeId, int timestep, tcpip::Storage& mySubsStorage);
    
    /// @brief Process the reception of a APP_MSG_RECEIVE subscription
    bool ReceiveApplicationMessage();

    /// @brief Process the reception of the confirmation of an APP_MSG_SEND subscription (msg scheduled or not)
    bool ReceiveConfirmationMessage();

    /// @brief Process the reception of the confirmation of an TRAFF_SIM subscription (CMD executed or not)
    bool ReceiveConfirmationTrafficMessage();

    /// @brief Process the reception of a pull from the resultContainer of a different application
    bool ReceiveResponseResultContainer();

    /// @brief Places in the output the status of the received command to be sent back to the iCS
    void writeStatusCmd(int commandId, int status, const std::string description);

    /// @brief Static instance of the server
    static Server* instance_;

    /// @brief Communication socket
    tcpip::Socket* socket_;

    /// @brief Flag of the status of the connection, open or close
    static bool closeConnection_;

    /// @brief Flag of the processing status of the server, executing logic or waiting for commands
    bool myDoingSimStep;

    /// @brief Container containing the specifications of the subscriptions (APP_SUMO,  APP_MSG_SEND, APP_MSG_RECEIVE)
    tcpip::Storage mySubsStorage;

    /// @brief Container from iCS to App for the data transmitted over the socket.
    tcpip::Storage myInputStorage;

    /// @brief Container from App to iCS for the data transmitted over the socket.
    tcpip::Storage myOutputStorage;
};

#endif
