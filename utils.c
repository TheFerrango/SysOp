#include "utils.h"
#include <string.h>

void safe_copy(char *dst,char *src)
{
    int src_size = strlen(src);
    strncpy(dst,src,sizeof(dst));
    printf("src: %s size:%i\n",src,src_size);
    if(src_size == MAX_LENGTH)
        dst[MAX_LENGTH] = '\0';
    else
        dst[src_size] = '\0';

}
