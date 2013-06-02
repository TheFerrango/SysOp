/*
A. A 2012/2013
Corso di laurea di informatica

Buratti Alberto (145552), 
Lotto Lorenzo (151775), 
Morandi Mirko(151778)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <syslog.h>
#include <pthread.h>
#include "logs.h"


/* mutex to handle cuncurrent logs */
pthread_mutex_t m_log;


/* internal function that writes to the syslog socket */
void log_generic(char * which_log, char * log_message, int log_type)
{
    /* initializes string to write on log*/
    char * log_s = malloc(sizeof(char) * (LOG_MAX_SIZE+1));
    log_s[0] = '\0';

    /* gets formatted date time */
//    char * date = date_time();

    /* prepares string to write on log */
  //  strcat(log_s, date);
    //strcat(log_s, " ");
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
    //free(date);
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