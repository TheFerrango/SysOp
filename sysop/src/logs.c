#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <pthread.h>
#include "logs.h"


pthread_mutex_t m_log;

char* time_stamp();

void log_generic(char * which_log, char * log_message, int log_type)
{
    char * log_s = malloc(sizeof(char) * (LOG_MAX_SIZE+1));
    log_s[0] = '\0';
    char * date = time_stamp();
    strcat(log_s, date);
    strcat(log_s, " ");
    strcat(log_s, log_message);
    log_s[strlen(log_s)] = '\0';
    openlog(which_log, 0, LOG_LOCAL0);
    syslog(log_type, "%s", log_s);
    closelog();
    free(log_s);
    free(date);
}

void log_error(char * which_log,  char * log_message)
{
    pthread_mutex_lock(&m_log);
    log_generic(which_log, log_message, LOG_ERR);
    pthread_mutex_unlock(&m_log);
}

void log_info(char * which_log,  char * log_message)
{
    pthread_mutex_lock(&m_log);
    log_generic(which_log, log_message, LOG_INFO);
    pthread_mutex_unlock(&m_log);
}

void log_debug(char * which_log,  char * log_message)
{
    pthread_mutex_lock(&m_log);
    log_generic(which_log, log_message, LOG_DEBUG);
    pthread_mutex_unlock(&m_log);
}

/* timestamp function; returns a formatted timestamp */
char* time_stamp()
{
    char * timestamp = malloc(sizeof(char)*23);
    time_t curr_time = time(NULL);
    struct tm *today_date;
    today_date = localtime(&curr_time);
    sprintf(timestamp, "%02d/%02d/%04d-%02d:%02d:%02d -> ", today_date->tm_mday,
            today_date->tm_mon+1, today_date->tm_year+1900, today_date->tm_hour, today_date->tm_min, today_date->tm_sec);
    timestamp[strlen(timestamp)] = '\0';
    return timestamp;
}
