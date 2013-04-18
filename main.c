#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <pthread.h>
#include <fcntl.h>

#define MAX_LENGTH 256

char* get_xor(char*,char*);
void* tr_read();
void read_random(char *s);


char text[MAX_LENGTH] = "\0";
pthread_mutex_t lock;

int main()
{
	//char *x = get_xor("Testo di prova ","df546tgfg435trf");
	//printf("%s\n",x);
	pthread_t tw,tr,td,te;

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    //tr = pthread_create(&tr,NULL,tr_read,NULL);
    char t[2];
    read_random(t);
    printf("%s\n", t);
	pthread_mutex_destroy(&lock);
	return 0;
}
void *tr_read()
{
	
    int i = 0;
	do
	{
		pthread_mutex_lock(&lock);
		char c =getchar();

		if(c != '\n')
		{
			text[i % MAX_LENGTH] = c;
			i++;
		}
		else
		{
			printf("%s\n", text);
			pthread_mutex_unlock(&lock);
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
void read_random(char *s)
{
	char *tmp = (char*)malloc(strlen(s) * sizeof(char));
	int n_bytes = strlen(s),n_read = 0;

	int random_fd = open("/dev/random",O_RDONLY);

	if(random_fd == -1)
	{
		perror("Error opening /dev/random file descriptor");
		exit(1);
	}

	while(n_read < n_bytes)
	{
		n_read += read(random_fd,tmp,n_bytes);
		printf("i read %i bytes\n",n_read);
	}

	close(random_fd);
	strcpy(s,tmp);
	free(tmp);
}


