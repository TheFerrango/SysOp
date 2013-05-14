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

queue *input_queue;
char text[MAX_LENGTH + 1] = "\0";
char r[MAX_LENGTH + 1] = "\0";
char*se,*sd;
sem_t sem;

sem_t emptyCount,fillCount;
pthread_mutex_t mutex;

int main()
{
	//char *x = get_xor("Testo di prova ","df546tgfg435trf");
	//printf("%s\n",x);
	printf("Alert:\n");
	printf("A string bigger than %i characters will overwrite itself!!!\n",MAX_LENGTH);

	pthread_t tr,te;
	input_queue = malloc(sizeof(queue));
    //r = (char*)malloc((strlen(text)+1)*sizeof(char));
    //r[0] = "\0";
	pthread_mutex_init(&mutex,NULL);

	sem_init(&fillCount,0,0);
	sem_init(&emptyCount,0,BUFFER_SIZE);


    int tr_status = pthread_create(&tr,NULL,tr_read,NULL);
    /**td = pthread_create(&td,NULL,td_function,NULL);
    tw = pthread_create(&tw,NULL,tw_function,NULL);**/

    //printf("text len:%s\n",strlen(text));
    //char t[200];
    //read_random(&t,200);
    //printf("%s\n", t);
    pthread_join(tr,NULL);
    sem_destroy(&fillCount);
    sem_destroy(&emptyCount);


    print_queue(input_queue);
	return 0;
}
void init_semaphore(custom_sem sem)
{
	//Now this does nothing, maybe i'll use it in future
}

void *tr_read()
{
	pthread_t te;
	init(input_queue);
    int i = 0;
    int te_status = pthread_create(&te,NULL,te_function,NULL);

    //int status = sem_wait(&sem);
    printf("Tr started successfully...\n");


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


			sem_wait(&emptyCount);
			printf("Releasing semaphore!\n");
			printf("testo: %s\n", text);
			pthread_mutex_lock(&mutex);
			if(!enqueue(text,input_queue))
				printf("Error while adding element to the queue!\n");
			pthread_mutex_unlock(&mutex);
			//Empty the string
			sem_post(&fillCount);
			printf("Retaking semaphore!\n");

			text[0]='\0';
			i=0;

		}
	}
	while(strstr(text,"quit") == NULL);

	//pthread_join degli altri thread
	pthread_join(te,NULL);
}

void get_xor(char *r,char *s,char *value)
{
	int i;
	int len = strlen(s);
	char c;
	char string[MAX_LENGTH + 1];
	string[MAX_LENGTH] = '\0';

	for(i = 0; i < len; i++)
		string[i] = s[i] ^ r[i];

	strncpy(value,string,len);

}
void* tw_function()
{
	printf("Sd: %s\n", sd);
	free(sd);
}
void* td_function()
{
	get_xor(r,se,sd);
	//free(r);
	free(se);
}

void* te_function()
{
	//char *input = (char*)malloc(MAX_LENGTH * sizeof(char));
	char input[MAX_LENGTH +1];
	input[0] = '\0';

	printf("sizeof input %i\n",sizeof(input));
	printf("Te thread started successfully!\n");
	int queue_status = 1;

	while(queue_status)
	{
		printf("Te trying to take semaphore\n");
		int status = sem_wait(&fillCount);
		pthread_mutex_lock(&mutex);
		queue_status = dequeue(&input,input_queue);
		pthread_mutex_unlock(&mutex);

		//r = (char*)malloc(strlen(input) * sizeof(char));

		printf("%s\n",input );
		read_random(r,strlen(input));

		printf("R: %s\n",r);
		se = (char*) malloc(strlen(r) * sizeof(char));
		get_xor(r,input,se);
		sem_post(&emptyCount);
		printf("Se: %s\n",se);

		//free(r);
		//free(se);
	}


}

void read_random(char *s,int s_len)
{
	//char *tmp = (char*)malloc(s_len * sizeof(char));
	char tmpChar;
	int n_bytes = s_len,n_read = 0;

	int random_fd = open("/dev/urandom",O_RDONLY);

	if(random_fd == -1)
	{
		perror("Error opening /dev/random file descriptor  ");
		exit(1);
	}

	printf("Reading from /dev/random... This may take a while!\n");
	//TODO provare a leggere un carattere alla volta
	printf("n_bytes: %i\n",n_bytes );
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

	close(random_fd);
}


