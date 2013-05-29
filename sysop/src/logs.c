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


/* mutex to handle cuncurrent logs */
pthread_mutex_t m_log;

char* date_time();

/* internal function that writes to the syslog socket */
void log_generic(char * which_log, char * log_message, int log_type)
{
    /* initializes string to write on log*/
    char * log_s = malloc(sizeof(char) * (LOG_MAX_SIZE+1));
    log_s[0] = '\0';

    /* gets formatted date time */
    char * date = date_time();

    /* prepares string to write on log */
    strcat(log_s, date);
    strcat(log_s, " ");
    strcat(log_s, log_message);
    log_s[strlen(log_s)] = '\0';

    /* opens log socket */
    openlog(which_log, 0, LOG_LOCAL0);

    /* writes on log */
    syslog(log_type, "%s", log_s);

    /* closes log socket */
    closelog();

    /* freeing up memory */
    free(log_s);
    free(date);
}

/* logs an error message for a thread */
void log_error(char * which_log,  char * log_message)
{
    /* locks access to the log socket */
    pthread_mutex_lock(&m_log);

    /* calls function that writes on socket */
    log_generic(which_log, log_message, LOG_ERR);

    /* releases log access */
    pthread_mutex_unlock(&m_log);
}

/* logs an information message for a thread */
void log_info(char * which_log,  char * log_message)
{
    pthread_mutex_lock(&m_log);
    log_generic(which_log, log_message, LOG_INFO);
    pthread_mutex_unlock(&m_log);
}

/* logs a debug message for a thread */
void log_debug(char * which_log,  char * log_message)
{
    pthread_mutex_lock(&m_log);
    log_generic(which_log, log_message, LOG_DEBUG);
    pthread_mutex_unlock(&m_log);
}

/* timestamp function; returns a formatted timestamp */
char* date_time()
{
    char * timestamp = malloc(sizeof(char)*23);
    time_t curr_time = time(NULL);
    struct tm *today_date;
    today_date = localtime(&curr_time);
    sprintf(timestamp, "%02d/%02d/%04d-%02d:%02d:%02d -> ",
            today_date->tm_mday, today_date->tm_mon+1,
            today_date->tm_year+1900, today_date->tm_hour,
            today_date->tm_min, today_date->tm_sec);
    timestamp[strlen(timestamp)] = '\0';
    return timestamp;
}
