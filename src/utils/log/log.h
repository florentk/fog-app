/****************************************************************************/
/// @file    log.h
/// @author  Julen Maneros
/// @date
/// @version $Id:
///
/****************************************************************************/
#ifndef LOG_H
#define LOG_H

// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

// ===========================================================================
// enum definitions
// ===========================================================================
/**
 * @enum LogLevel
 * @brief The loggin levels
 */
enum LogLevel {

    ///@brief Maximum log level error, for critical simulation stopping situations.
    kLogLevelError = 2,

    ///@brief Medium log level error, for situations that could produce odd behaviours in the simulation. The simulation may continue.
    kLogLevelWarning = 1,

    ///@brief Lower log level error, just information.
    kLogLevelInfo = 0,
};

// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class Log
 * @brief To control the iCS debugging log file.
 */
class Log
{
public:

    /**
    * @brief Initializes the log file.
    * @param [in] path The path in which the messages will be written.
    * @return EXIT_SUCCESS if the file was correctly created, -1 otherwise.
    */
    static int StartLog(std::string path);

    /**
    * @brief Establishes the log level with the input of the user.
    * @param [in] logLevel The log level defined by the user in the iTETRIS config file.
    */
    static void SetLogLevel(LogLevel logLevel);

    /**
    * @brief
    * @param [in]
    */
    static void SetLogTimeThreshold(char* logThreshold);

    /**
    * @brief
    * @param [in]
    */
    static void SetLogTimeThreshold(std::string logThreshold);

    /// @brief Closes the log file.
    static void Close();

    /**
    * @brief
    * @param [in]
    */
    static bool WriteLog(const char* message);

    /**
    * @brief
    * @param [in]
    * @param [in]
    * @return
    */
    static bool WriteHeader(const char* message);

    /**
    * @brief
    * @param [in]
    * @param [in]
    * @return
    */
    static bool Write(const char* message, LogLevel messageLogLevel);

    /**
    * @brief
    * @param [in]
    */
    static std::string GetPath();

private:

    /**
    * @brief
    * @param [in]
    */
    Log(std::string path, long timeThreshold);

    /// @brief File for the iCS loggin.
    std::ofstream myfile_;

    /// @brief Path of the file
    std::string path_;

    /// @brief Singleton instance
    static Log* instance_;

    static long lineCounter_;

    /// @brief User defined number of timesteps for each log file.
    static long timeStepThreshold_;

    /// @brief Next timestep in which the log file has to be changed.
    static long nextTimeStepThreshold_;

    /// @brief The amount of created log files.
    static int currentNumberLogFiles_;

    /// @brief Defines the log level of the output.
    static LogLevel logLevel_;

    /// @brief Closes and opens a new log file.
    int StartNewFile();
};
#endif
