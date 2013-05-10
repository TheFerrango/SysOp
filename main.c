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
sem_t *sem;

int val;
int main()
{
	//char *x = get_xor("Testo di prova ","df546tgfg435trf");
	//printf("%s\n",x);
	printf("Alert:\n");
	printf("A string bigger than %i characters will overwrite itself!!!\n",MAX_LENGTH);
	
	pthread_t tr;
	input_queue = malloc(sizeof(queue));
	//sem_init(&sem,0,0);
	sem_unlink("tmp");
	sem = sem_open("tmp",O_CREAT,0,0);
	if(sem == SEM_FAILED)
	{
		printf("Error creating semaphore\n");//but why?
		return 1;
	}
    int tr_status = pthread_create(&tr,NULL,tr_read,NULL);
    /**td = pthread_create(&td,NULL,td_function,NULL);
    tw = pthread_create(&tw,NULL,tw_function,NULL);**/

    //printf("text len:%s\n",strlen(text));
    //char t[200];
    //read_random(&t,200);
    //printf("%s\n", t);
    sem_close(sem);
    pthread_join(tr,NULL);
   
    print_queue(input_queue);
	return 0;
}


void *tr_read()
{
	pthread_t te;
	init(input_queue);
    int i = 0;
    printf("Tr started successfully...\n");

    int status = sem_wait(sem);
    perror("Error:  ");
    int te_status = pthread_create(&te,NULL,te_function,NULL);

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
			sem_post(sem);
			printf("%s\n", text);
			if(!enqueue(text,input_queue))
				printf("Error while adding element to the queue!\n");

			//Empty the string
			sem_wait(sem);
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
	do
	{
		int status = sem_wait(sem);
    	printf("%i\n",status );
		r = (char*)malloc(strlen(text) * sizeof(char));
		read_random(r,strlen(r));
		printf("R: %s\n",r);
		se = get_xor(r,text);
		sem_post(sem);
		printf("Se: %s\n",se);
	}
	while(dequeue(input,input_queue));
	

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


