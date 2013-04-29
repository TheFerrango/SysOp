#include <stdlib.h>
#include <syslog.h>

void logTr(char * logMsg)
{
    logGen("tr", logMsg);
}

void logTd(char * logMsg)
{
    logGen("td", logMsg);
}

void logTe(char * logMsg)
{
    logGen("te", logMsg);
}

void logTw(char * logMsg)
{
    logGen("tw", logMsg);
}

void logGen(char * whichLog, char * logMsg)
{
    openlog(whichLog, 0, LOG_LOCAL0);
    syslog(LOG_INFO, logMsg);
    closelog();
}
