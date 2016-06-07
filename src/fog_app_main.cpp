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
    oc.addCopyrightNotice("(c) iTETRIS consortium 2008-2010; http://www.ict-itetris.eu/");
    oc.addCopyrightNotice("   (c) DLR 2001-2010; http://sumo.sf.net");
    oc.addCallExample("-c <CONFIGURATION>");

    // insert options sub-topics
    SystemFrame::addConfigurationOptions(oc);
    oc.addOptionSubTopic("General");
    oc.addOptionSubTopic("CamAreaSubscription");
    oc.addOptionSubTopic("ReturningCarAreaSubscription");
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
}

/* -------------------------------------------------------------------------
 * options checking
 * ----------------------------------------------------------------------- */
bool
checkOptions()
{
    bool ret = true;
    OptionsCont &oc = OptionsCont::getOptions();

    // check last time step
    if (!oc.isSet("start")) {
        MsgHandler::getErrorInstance()->inform("Missing ITS start time step");
        ret = false;
    }

    // check last time step
    if (!oc.isSet("socket")) {
        MsgHandler::getErrorInstance()->inform("Missing socket port number");
        ret = false;
    }

    // check last time step
    if (!oc.isSet("logfile")) {
        MsgHandler::getErrorInstance()->inform("Missing log file path");
        ret = false;
    }

    // check last time step
    if (!oc.isSet("xcam")) {
        MsgHandler::getErrorInstance()->inform("Missing X base point for the CAM area subscription");
        ret = false;
    }

    // check last time step
    if (!oc.isSet("ycam")) {
        MsgHandler::getErrorInstance()->inform("Missing Y base point for the CAM area subscription");
        ret = false;
    }

    // check last time step
    if (!oc.isSet("radiuscam")) {
        MsgHandler::getErrorInstance()->inform("Missing radius for the CAM area subscription.");
        ret = false;
    }

    // check last time step
    if (!oc.isSet("xcar")) {
        MsgHandler::getErrorInstance()->inform("Missing X base point for the returning car area subscription.");
        ret = false;
    }

    // check last time step
    if (!oc.isSet("ycar")) {
        MsgHandler::getErrorInstance()->inform("Missing Y base point for the returning car area subscription.");
        ret = false;
    }

    // check last time step
    if (!oc.isSet("radiuscar")) {
        MsgHandler::getErrorInstance()->inform("Missing radius for the returning car area subscription.");
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
    oc.setApplicationDescription("iTETRIS Community ITS Cooperative Demo App");
#ifdef WIN32
    oc.setApplicationName("demoapp.exe", "ITS Cooperative Demo App");
#else
    oc.setApplicationName("demoapp", "ITS Cooperative Demo App " + (string)PACKAGE_VERSION);
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

        MsgHandler::getMessageInstance()->inform("Starting iTETRIS ITS Cooperative Demo Application");

        //  options verification
        if (!checkOptions()) throw ProcessError();

        // Initialize log file
        std::string logfile =  oc.getString("logfile");
        Log::StartLog(logfile.c_str());
        Log::WriteHeader("iTETRIS ITS COOPERATIVE DEMO APP LOG FILE");

        // Set Application start time step
        if (ApplicationLogic::SetApplicationStartTimeStep(oc.getInt("start")) == EXIT_FAILURE) throw ProcessError("Start is negative");

        // Set CAM area the user values
        if (ApplicationLogic::SetCamArea(oc.getFloat("xcam"), oc.getFloat("ycam"), oc.getFloat("radiuscam")) == EXIT_FAILURE) throw ProcessError();

        // Set desired returning car area user values
        if (ApplicationLogic::SetCarArea(oc.getFloat("xcar"), oc.getFloat("ycar"), oc.getFloat("radiuscar")) == EXIT_FAILURE) throw ProcessError();

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