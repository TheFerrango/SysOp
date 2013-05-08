#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <pthread.h>
#include <fcntl.h>
#include "logs.h"
#include "queue.h"


char* get_xor(char*,char*);
void* tr_read();
void read_random(char *,int);
void* tw_function();
void* te_function();
void* td_function();


queue input_queue;
char text[MAX_LENGTH] = "\0";
char* r,se,sd;

int main()
{
	//char *x = get_xor("Testo di prova ","df546tgfg435trf");
	//printf("%s\n",x);
	pthread_t tw,tr,td,te;

    /**tr = pthread_create(&tr,NULL,tr_read,NULL);
    te = pthread_create(&te,NULL,te_function,NULL);
    td = pthread_create(&td,NULL,td_function,NULL);
    tw = pthread_create(&tw,NULL,tw_function,NULL);**/


    printf("%s\n",strlen(text));
    char t[200];
    read_random(&t,200);
    printf("%s\n", t);
    
    //pthread_join(&tr,NULL);
	return 0;
}


void *tr_read()
{
	init(&input_queue);
    int i = 0;
	do
	{
		char c =getchar();

		if(c != '\n')
		{
			text[i % MAX_LENGTH] = c;
			i++;
		}
		else
		{
			printf("%s\n", text);
			enqueue(text,&input_queue);
		}
	}
	while(strstr(text,"quit") == NULL);
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
	char input[MAX_LENGTH];
	do
	{
		r = (char*)malloc(strlen(input) * sizeof(char));
		read_random(r,strlen(r));
		printf("R: %s\n",r);
		se = get_xor(r,text);
		printf("Se: %s\n",se);
	}
	while(dequeue(input,&input_queue));
	

}
void read_random(char *s,int s_len)
{
	//char *tmp = (char*)malloc(s_len * sizeof(char));
	char tmp;
	int n_bytes = s_len,n_read = 0;

	int random_fd = open("/dev/random",O_RDONLY);

	if(random_fd == -1)
	{
		perror("Error opening /dev/random file descriptor");
		exit(1);
	}

	printf("Reading from /dev/random... This may take a while!\n");
	//TODO provare a leggere un carattere alla volta
	/**while(n_read < n_bytes)
	{
		read(random_fd,tmp,n_bytes);
		n_read = strlen(tmp);
		strcat(s,tmp);
		printf("char readed: %i\n",n_read);
		printf("...\n");

	}**/
	int i=0;
	for(; i < s_len;i++)
	{
		read(random_fd,&tmp,sizeof(char));
		s[i] = tmp;
	}
	close(random_fd);
}


