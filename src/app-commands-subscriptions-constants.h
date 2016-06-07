/****************************************************************************/
/// @file    app-commands-subscriptions-constants.h
/// @author
/// Constant values to be shared between the iCS and the Applications.
/****************************************************************************/
// iTETRIS, see http://www.ict-itetris.eu
// Copyright © 2008 iTetris Project Consortium - All rights reserved
/****************************************************************************/

#ifndef APP_COMM_SUBS_CONSTANTS_H
#define APP_COMM_SUBS_CONSTANTS_H

// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

// ****************************************
// COMMANDS & SUBSCRIPTIONS
// ****************************************

// command: close connection
#define CMD_APP_CLOSE 0xFF

// subscription: Return cars in a zone
#define SUB_RETURNS_CARS_IN_ZONE 0x02

// command: Send a message to a station.
#define CMD_SEND_MESSAGE_TO_STATION 0x03

// subscription: Request received messages status
#define SUB_REQ_RECEIVED_MESSAGES_STATUS 0x05

// command: Notify Application to execute
#define CMD_NOTIFY_APP_EXECUTE 0x06

// command: End of Application Execution
#define CMD_NOTIFY_APP_END_EXECUTE 0x07

// command End of Subscription
#define CMD_END_SUBSCRIPTION 0x08

// command Checks Applications readiness
#define CMD_APP_READY 0x09

// command Ask for subscriptions
#define CMD_ASK_FOR_SUBSCRIPTION 0x0A

// command End of subscription requests
#define CMD_END_SUBSCRIPTION_REQUEST 0x0B

// command Drop the subscription
#define CMD_DROP_SUBSCRIPTION 0x0C

// command Renew the subscription
#define CMD_RENEW_SUBSCRIPTION 0x0D

// command Sends returns cars in zone subscription data
#define CMD_CARS_IN_ZONE 0x0E

// subscription Set area to send CAM
#define SUB_SET_CAM_AREA 0x11

// notify app of arrived messages
#define CMD_NOTIFY_APP_MESSAGE_STATUS 0x14

// subscription start travel time estimation
#define SUB_TRAVEL_TIME_ESTIMATION_START 0x12

// subscription end travel time estimation
#define SUB_TRAVEL_TIME_ESTIMATION_END 0x13

// subscription to monitor the travel time estimation
#define SUB_TRAVEL_TIME_ESTIMATION 0x15

// subscription to monitor the travel time estimation
#define CMD_TRAVEL_TIME_ESTIMATION 0x20

// subscription get received cam info
#define SUB_RECEIVED_CAM_INFO 0x18

// command Sends returns received cam info data
#define CMD_RECEIVED_CAM_INFO 0x19

// subscription to get the facilities information about a station
#define SUB_FACILITIES_INFORMATION 0x1C

// send to the application the facilities info data
#define CMD_FACILITIES_INFORMATION 0x1D

// subscription to send an application message
#define SUB_APP_MSG_SEND 0x1E

// send to the application the status of the application message scheduling process
#define CMD_APP_MSG_SEND 0x1F

// subscription to push an application cmd to the traffic simulator
#define SUB_APP_CMD_TRAFF_SIM 0x2A

// send to the application the traffic simulator data
#define CMD_APP_CMD_TRAFF_SIM 0x2B

// subscription to an application pull result from the traffic simulator
#define SUB_APP_RESULT_TRAFF_SIM 0x2C

// send to the application the status of the traffic simulator application CMD process
#define CMD_APP_RESULT_TRAFF_SIM 0x2D

// subscription to cross-application data
#define SUB_X_APPLICATION_DATA 0x2E

// send to the application the cross application data
#define CMD_X_APPLICATION_DATA 0x2F

// subscription to send an application message
#define SUB_APP_MSG_RECEIVE 0x3A

// send to the application the status of the application message scheduling process
#define CMD_APP_MSG_RECEIVE 0x3B


// ****************************************
// OUTPUT OF APPLICATIONS
// Where the result of the application should be applied
// ****************************************

// Set maximum speed for a vehicle
#define OUTPUT_SET_VEHICLE_MAX_SPEED 0x04

// Calculate the travel time
#define OUTPUT_TRAVEL_TIME_ESTIMATION 0x21

// Traffic jam detection
#define OUTPUT_TRAFFIC_JAM_DETECTION 0x23

// Application without result
#define OUTPUT_VOID 0x22

// reply: The application executed its algorithm //15
#define APP_RESULT_ON 0x0F

// reply: The application DID NOT execute //16
#define APP_RESULT_OFF 0x10

// ****************************************
// RESULT TYPES
// ****************************************

// result type: Ok
#define APP_RTYPE_OK    0x01
// result type: not implemented
#define APP_RTYPE_NOTIMPLEMENTED  0x02
// result type: error
#define APP_RTYPE_ERR   0xFF

// ****************************************
// MESSAGE STATUS
// ****************************************

// result : to schedule in ns-3
#define APP_RESULT_TO_SCHEDULE    0x15
// result : to be applied in traffic simulator
#define APP_RESULT_TO_APPLY  0x16
// result : discard it (do not apply)
#define APP_RESULT_TO_DISCARD   0x17

// ****************************************
// APPLICATION-BASED SUBSCRIPTIONS
// ****************************************

// APP_MSG_SEND and APP_MSG_RECEIVE Subscriptions
#define EXT_HEADER_TYPE_TOPOBROADCAST       0x00
#define EXT_HEADER_TYPE_UNICAST             0x01
#define EXT_HEADER_TYPE_MULTICAST           0x02
#define EXT_HEADER_TYPE_GEOBROADCAST        0x03
#define EXT_HEADER_TYPE_GEOUNICAST          0x04
#define EXT_HEADER_TYPE_GEOANYCAST          0x05

#define EXT_HEADER__VALUE_BLOCK_IDs         0x00
#define EXT_HEADER__VALUE_BLOCK_AREAs       0x01
#define EXT_HEADER__VALUE_BLOCK_HOPS_No     0x02

#define EXT_HEADER__CIRCLE                  0x00
#define EXT_HEADER__ELLIPSE                 0x01
#define EXT_HEADER__CONV_POLYGON            0x02
#define EXT_HEADER__LANE_ID                 0x03
#define EXT_HEADER__EDGE_ID                 0x04
#define EXT_HEADER__JUNCTION_ID             0x05

// CMD_Traffic_SIM Subscriptions
#define VALUE_SET_EDGE_TRAVELTIME       0x01
#define VALUE_GET_EDGE_TRAVELTIME       0x02
#define VALUE_RE_ROUTE   	    	0x03
#define VALUE_GET_ROUTE_VARIABLE	0x04
#define VALUE_SET_SPEED			0x05

#define TYPE_ERROR			0x00
#define TYPE_EDGE_ID			0x01
#define	TYPE_STATION_ID			0x02
#define TYPE_EDGE_TRAVELTIME       	0x03
#define TYPE_EDGES_ID_FROM_ROAD_ID      0x04

// ****************************************
// CROSS_APPLICATION SUBSCRIPTION TYPE
// ****************************************
#define TTE				0x01
#define JAM				0x02
#define EDGE_ID				0x03
#define ROUTE_ID			0x04
#define SPEED				0x05

#define VALUE_GET_DATA_BY_RESULT_CONTAINER_ID      0x01
#endif
