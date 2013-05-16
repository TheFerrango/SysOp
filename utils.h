#include <string.h>
#include <semaphore.h>
#define MAX_LENGTH 256
#define BUFFER_SIZE 20
size_t safe_copy(char *dst, const char *src, size_t siz);
typedef struct custom_sem
{
	sem_t fillCount;
	sem_t emptyCount;
}custom_sem;
