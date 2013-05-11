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



char* get_xor(char*,char*);
void* tr_read();
void read_random(char *,int);
void* tw_function();
void* te_function();
void* td_function();

queue *input_queue;
char text[MAX_LENGTH + 1] = "\0";
char* r,se,sd;
sem_t sem;

sem_t emptyCount,fillCount;
pthread_mutex_t mutex;

int val;
int main()
{
	//char *x = get_xor("Testo di prova ","df546tgfg435trf");
	//printf("%s\n",x);
	printf("Alert:\n");
	printf("A string bigger than %i characters will overwrite itself!!!\n",MAX_LENGTH);

	pthread_t tr,te;
	input_queue = malloc(sizeof(queue));
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
    sem_destroy(&fillCount);
    sem_destroy(&emptyCount);
    pthread_join(tr,NULL);

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
			printf("%s\n", text);
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

char* get_xor(char *r,char *s)
{
	char *value = (char* ) malloc(strlen(s) * sizeof(char));

	int i;
	for(i = 0; i < strlen(s); i++)
		value[i] = s[i] ^ r[i];

	return value;
}
void* tw_function()
{
	printf("Sd: %s\n", sd);
	free(sd);
}
void* td_function()
{
	sd = get_xor(r,se);
	free(r);
	free(se);
}

void* te_function()
{
	char input[MAX_LENGTH + 1];
	input[0] = '\0';
	printf("Te thread started successfully!\n");
	int queue_status = 1;
	while(queue_status)
	{
		printf("Te trying to take semaphore\n");
		int status = sem_wait(&fillCount);
		pthread_mutex_lock(&mutex);
		queue_status = dequeue(input,input_queue);
		pthread_mutex_unlock(&mutex);
		printf("Te takes semaphore\n");
		r = (char*)malloc(strlen(input) * sizeof(char));
		read_random(r,strlen(r));
		printf("R: %s\n",r);
		se = get_xor(r,input);
		sem_post(&emptyCount);
		printf("Se: %s\n",se);
	}


}

void read_random(char *s,int s_len)
{
	char *tmp = (char*)malloc(s_len * sizeof(char));
	int n_bytes = s_len,n_read = 0;

	int random_fd = open("/dev/random",O_RDONLY);

	if(random_fd == -1)
	{
		perror("Error opening /dev/random file descriptor");
		exit(1);
	}

	printf("Reading from /dev/random... This may take a while!\n");
	//TODO provare a leggere un carattere alla volta
	while(n_read < n_bytes)
	{
		read(random_fd,tmp,n_bytes);
		n_read = strlen(tmp);
		strcat(s,tmp);
		printf("char readed: %i\n",n_read);
		printf("...\n");

	}

	close(random_fd);
}


