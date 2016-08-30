/****************************************************************************/
/// @file    its_demo_app_main.cpp
/// @author  Julen Maneros
/// @date    10.10.2010
///
// Main for the iTETRIS Cooperative ITS demo
/****************************************************************************/
// iTETRIS; see http://www.ict-itetris.eu/
// Copyright 2008-2010 iTETRIS consortium
/****************************************************************************/

// ===========================================================================
// included modules
// ===========================================================================
#include "server.h"
#include "utils/log/log.h"
#include <utils/common/SystemFrame.h>
#include <utils/common/MsgHandler.h>
#include <utils/options/OptionsCont.h>
#include <utils/options/OptionsIO.h>
#include <utils/xml/XMLSubSys.h>

#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

// ===========================================================================
// used namespaces
// ===========================================================================
using namespace std;

// ===========================================================================
// functions
// ===========================================================================
/* -------------------------------------------------------------------------
 * options initialisation
 * ----------------------------------------------------------------------- */
void
fillOptions()
{
    OptionsCont &oc = OptionsCont::getOptions();
    oc.clearCopyrightNotices();
    oc.addCopyrightNotice("(c) Florent Kaisser 2016");
    oc.addCallExample("-c <CONFIGURATION>");

    // insert options sub-topics
    SystemFrame::addConfigurationOptions(oc);
    oc.addOptionSubTopic("General");
    oc.addOptionSubTopic("CamAreaSubscription");
    oc.addOptionSubTopic("ReturningCarAreaSubscription");
    oc.addOptionSubTopic("FogArea");
    oc.addOptionSubTopic("Alert");         
    SystemFrame::addReportOptions(oc);

    oc.doRegister("start", 's' , new Option_Integer(0));
    oc.addDescription("start", "General", "Defines the begin time of the ITS App");

    oc.doRegister("socket", new Option_Integer());
    oc.addDescription("socket", "General", "Socket port number in which the App expects commands from the iCS");

    oc.doRegister("logfile", new Option_FileName());
    oc.addDescription("logfile", "General", "Path of the logging file");

    oc.doRegister("xcam", new Option_Float());
    oc.addDescription("xcam", "CamAreaSubscription", "Defines X base point of the CAM Area Subscription");

    oc.doRegister("ycam", new Option_Float());
    oc.addDescription("ycam", "CamAreaSubscription", "Defines Y base point of the CAM Area Subscription");

    oc.doRegister("radiuscam", new Option_Float());
    oc.addDescription("radiuscam", "CamAreaSubscription", "Defines radius of the CAM Area Subscription");

    oc.doRegister("xcar", new Option_Float());
    oc.addDescription("xcar", "ReturningCarAreaSubscription", "Defines X base point of the area to return cars Subscription");

    oc.doRegister("ycar", new Option_Float());
    oc.addDescription("ycar", "ReturningCarAreaSubscription", "Defines Y base point of the area to return cars Subscription");

    oc.doRegister("radiuscar", new Option_Float());
    oc.addDescription("radiuscar", "ReturningCarAreaSubscription", "Defines radius of the area to return cars Subscription");
    
    //Specific Fog application
    oc.doRegister("xfog", new Option_Float());
    oc.addDescription("xfog", "FogArea", "Defines X base point of the area of fog");

    oc.doRegister("yfog", new Option_Float());
    oc.addDescription("yfog", "FogArea", "Defines Y base point of the area of fog");

    oc.doRegister("radiusfog", new Option_Float());
    oc.addDescription("radiusfog", "FogArea", "Defines radius of the area of fog"); 
    
    oc.doRegister("startfog", new Option_Integer());
    oc.addDescription("startfog", "FogArea", "Defines the begin time of fog");  
    
    oc.doRegister("endfog", new Option_Integer());
    oc.addDescription("endfog", "FogArea", "Defines the end time of fog");           
    
    oc.doRegister("radiusalert", new Option_Float());
    oc.addDescription("radiusalert", "Alert", "Defines radius of the alert");  
     
    oc.doRegister("timeoutalert", new Option_Integer());
    oc.addDescription("timeoutalert", "Alert", "Defines the timeout of a alert");       
    
    oc.doRegister("alertspeedlimit", new Option_Float());
    oc.addDescription("alertspeedlimit", "Alert", "Defines the speed limit on alert");    
   
    oc.doRegister("alertactif", new Option_Bool());
    oc.addDescription("alertactif", "Alert", "Defines if the alert is activ");          
    
    oc.doRegister("alertinterval", new Option_Integer());
    oc.addDescription("alertinterval", "Alert", "Defines the Minimum interval between broadcast alert in second");   
    
    oc.doRegister("durationofslowdown", new Option_Integer());
    oc.addDescription("durationofslowdown", "Alert", "Defines the Duration of slowdown in second");           
    
    oc.doRegister("noalertmessageifisslowed", new Option_Bool());
    oc.addDescription("noalertmessageifisslowed", "Alert", "Defines the resend alert when the vehicule is slowed");        
    
              
}

/* -------------------------------------------------------------------------
 * options checking
 * ----------------------------------------------------------------------- */
bool
checkOptions()
{
    bool ret = true;
    OptionsCont &oc = OptionsCont::getOptions();

    
    if (!oc.isSet("start")) {
        MsgHandler::getErrorInstance()->inform("Missing ITS start time step");
        ret = false;
    }

    
    if (!oc.isSet("socket")) {
        MsgHandler::getErrorInstance()->inform("Missing socket port number");
        ret = false;
    }

    
    if (!oc.isSet("logfile")) {
        MsgHandler::getErrorInstance()->inform("Missing log file path");
        ret = false;
    }

    
    if (!oc.isSet("xcam")) {
        MsgHandler::getErrorInstance()->inform("Missing X base point for the CAM area subscription");
        ret = false;
    }

    
    if (!oc.isSet("ycam")) {
        MsgHandler::getErrorInstance()->inform("Missing Y base point for the CAM area subscription");
        ret = false;
    }

    
    if (!oc.isSet("radiuscam")) {
        MsgHandler::getErrorInstance()->inform("Missing radius for the CAM area subscription.");
        ret = false;
    }

    
    if (!oc.isSet("xcar")) {
        MsgHandler::getErrorInstance()->inform("Missing X base point for the returning car area subscription.");
        ret = false;
    }

    
    if (!oc.isSet("ycar")) {
        MsgHandler::getErrorInstance()->inform("Missing Y base point for the returning car area subscription.");
        ret = false;
    }

    
    if (!oc.isSet("radiuscar")) {
        MsgHandler::getErrorInstance()->inform("Missing radius for the returning car area subscription.");
        ret = false;
    }   
    
    if (!oc.isSet("xfog")) {
        MsgHandler::getErrorInstance()->inform("Missing X base point of the area of fog.");
        ret = false;
    }
    
    if (!oc.isSet("yfog")) {
        MsgHandler::getErrorInstance()->inform("Missing Y base point of the area of fog.");
        ret = false;
    }
    
    if (!oc.isSet("radiusfog")) {
        MsgHandler::getErrorInstance()->inform("Missing radius of the area of fog.");
        ret = false;
    }
    
    if (!oc.isSet("startfog")) {
        MsgHandler::getErrorInstance()->inform("Missing the begin time of fog.");
        ret = false;
    }
    
    if (!oc.isSet("endfog")) {
        MsgHandler::getErrorInstance()->inform("Missing the end time of fog.");
        ret = false;
    }
    
    if (!oc.isSet("radiusalert")) {
        MsgHandler::getErrorInstance()->inform("Missing radius of the alert.");
        ret = false;
    }
    
    if (!oc.isSet("timeoutalert")) {
        MsgHandler::getErrorInstance()->inform("Missing the timeout of a alert.");
        ret = false;
    }
    
    if (!oc.isSet("alertspeedlimit")) {
        MsgHandler::getErrorInstance()->inform("Missing the speed limit on alert.");
        ret = false;
    }
    if (!oc.isSet("alertactif")) {
        MsgHandler::getErrorInstance()->inform("Missing the activity alert");
        ret = false;
    }   
  
    if (!oc.isSet("alertinterval")) {
        MsgHandler::getErrorInstance()->inform("Missing the Minimum interval between broadcast alert in second");
        ret = false;
    }
    
    if (!oc.isSet("durationofslowdown")) {
        MsgHandler::getErrorInstance()->inform("Missing the Duration of slowdown in second");
        ret = false;
    }
    if (!oc.isSet("noalertmessageifisslowed")) {
        MsgHandler::getErrorInstance()->inform("Missing the resend alert when the vehicule is slowed");
        ret = false;
    }     
    
    return ret;
}

/* -------------------------------------------------------------------------
 * main
 * ----------------------------------------------------------------------- */
int main(int argc, char **argv)
{
    OptionsCont &oc = OptionsCont::getOptions();
    oc.setApplicationDescription("Fog app for iTetris");
#ifdef WIN32
    oc.setApplicationName("itsfogapp.exe", "Fog app for iTetris");
#else
    oc.setApplicationName("itsfogapp", "Fog app for iTetris " + (string)PACKAGE_VERSION);
#endif

    int ret = 0;

    try {
        // start-up
        XMLSubSys::init(false); // xml-initialization
        //  options parsing
        fillOptions();
        // Get and process the command options
        OptionsIO::getOptions(true, argc, argv);
        if (oc.processMetaOptions(argc < 2)) { // Close if less of two arguments are provided (one is the app name)
            SystemFrame::close();
            return 0;
        }
        //  message handler start-up
        MsgHandler::initOutputOptions(true);

        MsgHandler::getMessageInstance()->inform("Starting Fog application for iTetris");

        //  options verification
        if (!checkOptions()) throw ProcessError();

        // Initialize log file
        std::string logfile =  oc.getString("logfile");
        Log::StartLog(logfile.c_str());
        Log::WriteHeader("FOG APPLICATION LOG FILE");

        // Set Application start time step
        if (ApplicationLogic::SetApplicationStartTimeStep(oc.getInt("start")) == EXIT_FAILURE) throw ProcessError("Start is negative");

        // Set CAM area the user values
        if (ApplicationLogic::SetCamArea(oc.getFloat("xcam"), oc.getFloat("ycam"), oc.getFloat("radiuscam")) == EXIT_FAILURE) throw ProcessError();

        // Set desired returning car area user values
        if (ApplicationLogic::SetCarArea(oc.getFloat("xcar"), oc.getFloat("ycar"), oc.getFloat("radiuscar")) == EXIT_FAILURE) throw ProcessError();

        // Set desired returning values for fog application specific 
        if (ApplicationLogic::SetFogArea(oc.getFloat("xfog"), oc.getFloat("yfog"), oc.getFloat("radiusfog")) == EXIT_FAILURE) throw ProcessError();
        if (ApplicationLogic::SetFogStartTimeStep(oc.getInt("startfog")) == EXIT_FAILURE) throw ProcessError();        
        if (ApplicationLogic::SetFogEndTimeStep(oc.getInt("endfog")) == EXIT_FAILURE) throw ProcessError();
        if (ApplicationLogic::SetAlertRadius(oc.getFloat("radiusalert")) == EXIT_FAILURE) throw ProcessError();
        if (ApplicationLogic::SetAlertTimeOut(oc.getInt("timeoutalert")) == EXIT_FAILURE) throw ProcessError();
        if (ApplicationLogic::SetAlertSpeedlimit(oc.getFloat("alertspeedlimit")) == EXIT_FAILURE) throw ProcessError();
        if (ApplicationLogic::SetAlertActif(oc.getBool("alertactif")) == EXIT_FAILURE) throw ProcessError();        
        if (ApplicationLogic::SetAlertInterval(oc.getInt("alertinterval")) == EXIT_FAILURE) throw ProcessError();
        if (ApplicationLogic::SetDurationOfSlowdown(oc.getInt("durationofslowdown")) == EXIT_FAILURE) throw ProcessError();       
        if (ApplicationLogic::SetNoAlertMessageIfIsSlowed(oc.getBool("noalertmessageifisslowed")) == EXIT_FAILURE) throw ProcessError();     
       
        // Start the server
        Server::processCommands(oc.getInt("socket"));

        return 0;

    } catch (std::runtime_error &e) {
        if (std::string(e.what())!=std::string("Process Error") && std::string(e.what())!=std::string("")) {
            MsgHandler::getErrorInstance()->inform(e.what());
        }
        MsgHandler::getErrorInstance()->inform("Quitting (on error).", false);
        ret = 1;
    } catch (...) {
        MsgHandler::getErrorInstance()->inform("Quitting (on unknown error).", false);
        ret = 1;
    }

    SystemFrame::close();
    Log::Close();
    return ret;
}
