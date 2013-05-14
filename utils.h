#include <string.h>
#include <semaphore.h>
#define MAX_LENGTH 256
#define BUFFER_SIZE 20

typedef struct custom_sem
{
	sem_t fillCount;
	sem_t emptyCount;
}custom_sem;