#include "ErrorLog.h"

static FILE *log;

bool initErrorLog(){
    //Opens the file for logging, if we can we terminate by force.
    if(!(log = fopen(ERROR_LOG_FILE,"w"))){
        perror("Couldn't init the error log file\n");
        exit(1);
    }
    //First line of the error log
    logError("--%s V:%s Initializing error log--\n",APP_NAME,APP_VERSION);

    return true;
}

bool closeErrorLog(){
    //Ending line of the log
    logError("\n--Closing error log--\n");
    //Close the log
    if(log)fclose(log);

    return true;
}

bool logError(const char *format,...){
    //Same as used by printfx
    va_list arg;
    //Start the list
    va_start(arg,format);
    
    //Print the error, and flush it to ensure its written
    if(log){
        vfprintf(log,format,arg);
        vfprintf(log,"\n",arg);
        fflush(log);
    }
    //End the list
    va_end(arg);

    return true;
}
