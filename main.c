#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>

#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include "logs.h"
#include "queue.h"



void get_xor(char*,char*,char*);
void* tr_read();
void read_random(char *,int);
void* tw_function();
void* te_function();
void* td_function();

queue *input_queue, *se_queue, *sd_queue;
char text[MAX_LENGTH + 1] = "\0";
char r[MAX_LENGTH + 1] = "\0";
char se[MAX_LENGTH + 1] = "\0";
char sd[MAX_LENGTH + 1] = "\0";

pthread_t tr,te,td,tw;

sem_t fillCount, se_sem, sd_sem;
pthread_mutex_t mutex, se_mutex,sd_mutex;

int main()
{
	//char *x = get_xor("Testo di prova ","df546tgfg435trf");
	//printf("%s\n",x);
	printf("Alert:\n");
	printf("A string bigger than %i characters will overwrite itself!!!\n",MAX_LENGTH);

	input_queue = malloc(sizeof(queue));
	se_queue = malloc(sizeof(queue));
	sd_queue = malloc(sizeof(queue));
    //r = (char*)malloc((strlen(text)+1)*sizeof(char));
    //r[0] = "\0";
	pthread_mutex_init(&mutex,NULL);
	pthread_mutex_init(&se_mutex,NULL);
	pthread_mutex_init(&sd_mutex,NULL);

	sem_init(&fillCount,0,0);
	sem_init(&se_sem,0,0);
    sem_init(&sd_sem,0,0);



    int tr_status = pthread_create(&tr,NULL,tr_read,NULL);
    int te_status = pthread_create(&te,NULL,te_function,NULL);

    int td_status = pthread_create(&td,NULL,td_function,NULL);
    int tw_status = pthread_create(&tw,NULL,tw_function,NULL);

    //printf("text len:%s\n",strlen(text));
    //char t[200];
    //read_random(&t,200);
    //printf("%s\n", t);
    pthread_join(tr,NULL);
    //sem_destroy(&fillCount);
    sem_destroy(&se_sem);
    sem_destroy(&sd_sem);



    printf("That's all folks! ");
	return 0;
}

void *tr_read()
{
	pthread_t te;
	init(input_queue);
    int i = 0;

    //int status = sem_wait(&sem);
    //printf("Tr started successfully...\n");

	do
	{
		char c=fgetc(stdin);
		if(c != '\n')
		{
			text[i % MAX_LENGTH] = c;
			i++;
		}
		else
		{
            if( i < MAX_LENGTH)
                text[i % MAX_LENGTH] = '\0';


			//printf("testo: %s\n", text);
			pthread_mutex_lock(&mutex);
			if(!enqueue(text,input_queue))
				printf("Error while adding element to the queue!\n");
			pthread_mutex_unlock(&mutex);
			//Empty the string
			sem_post(&fillCount);
			text[0]='\0';
			i=0;

		}
	}
	while(strstr(text,"quit") == NULL);

	//pthread_join degli altri thread
	printf("TR EXTERMINATED\n");
	sem_post(&fillCount);
	pthread_join(te,NULL);
	pthread_join(td,NULL);
	pthread_join(tw,NULL);

}

void get_xor(char *r,char *s,char *value)
{

	int i;
	int len = strlen(s);
	char c;
	char string[MAX_LENGTH + 1];


	for(i = 0; i < len; i++)
		string[i] = s[i] ^ r[i];
    //string[i] = '\0';
	strncpy(value,string,len);
	value[i] = '\0';

}
void* tw_function()
{
    int queue_status = 1;
    char output[MAX_LENGTH +1];
	output[0] = '\0';

    while(queue_status)
    {
        sem_wait(&sd_sem);
        pthread_mutex_lock(&sd_mutex);
		queue_status = dequeue(&output,sd_queue);
		pthread_mutex_unlock(&sd_mutex);
        printf("SD:\t %s\n", sd);
    }
    printf("TW EXTERMINATED\n");
    //pthread_join(td,NULL);

}
void* td_function()
{
    int queue_status = 1;
    char input[MAX_LENGTH +1];
	input[0] = '\0';


    while(queue_status)
    {
        sem_wait(&se_sem);
        pthread_mutex_lock(&se_mutex);
		queue_status = dequeue(&input,se_queue);
		pthread_mutex_unlock(&se_mutex);
        get_xor(r,se,sd);

        pthread_mutex_lock(&sd_mutex);
        enqueue(&sd, sd_queue);
        pthread_mutex_unlock(&sd_mutex);

        sem_post(&sd_sem);
    }
    printf("TD EXTERMINATED\n");
    sem_post(&sd_sem);
    //pthread_join(tw,NULL);
	//free(r);
	//free(se);
}

void* te_function()
{
	//char *input = (char*)malloc(MAX_LENGTH * sizeof(char));
    init(se_queue);
	char input[MAX_LENGTH +1];

	input[0] = '\0';

	//printf("sizeof input %i\n",sizeof(input));
	//printf("Te thread started successfully!\n");
	int queue_status = 1;

	while(queue_status)
	{
		//printf("Te trying to take semaphore\n");
		int status = sem_wait(&fillCount);
		pthread_mutex_lock(&mutex);
		queue_status = dequeue(&input,input_queue);
		pthread_mutex_unlock(&mutex);

		//r = (char*)malloc(strlen(input) * sizeof(char));

		//printf("INPUT: %s\n",input );
		read_random(r,strlen(input));

		printf("R:\t %s\n",r);
		//se = (char*) malloc((abs(strlen(r))+1) * sizeof(char));
		get_xor(r,input,se);
		pthread_mutex_lock(&se_mutex);
        enqueue(&se,se_queue);
        pthread_mutex_unlock(&se_mutex);
		printf("SE:\t %s\n",se);
		sem_post(&se_sem);

		//free(r);
		//free(se);
	}
    //free(input);
    printf("TE EXTERMINATED\n");
    sem_post(&se_sem);
	pthread_join(td, NULL);
}

void read_random(char *s,int s_len)
{
	//char *tmp = (char*)malloc(s_len * sizeof(char));
	char tmpChar;
	int n_bytes = s_len,n_read = 0;


	int random_fd = open("/dev/random",O_RDONLY);
	//int random_fd = open("/dev/random",O_RDONLY);

	if(random_fd == -1)
	{
		perror("Error opening /dev/random file descriptor  ");
		exit(1);
	}

	printf("Reading from /dev/random... This may take a while!\n");
	//printf("n_bytes: %i\n",n_bytes );
	while(n_bytes > 0)
	{
		//n_read = read(random_fd,&tmpChar,1);

/*
		if(n_read > s_len)
			strncat(s,tmp,  n_read-s_len);
		else
			strcat(s,tmp);

		printf("char readed: %i\n",n_read);
		printf("...\n");
		n_bytes -= n_read;
*/
        if(read(random_fd,&tmpChar,1) != -1)
        {
            s[n_read] = tmpChar;
            n_bytes--;
            n_read++;
        }
	}
	s[n_read] = '\0';

	close(random_fd);
}


