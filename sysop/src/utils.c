#include "utils.h"
#include <string.h>

/*
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
*/

size_t safe_copy(char *dst, const char *src, size_t siz)
{
        char *d = dst;
        const char *s = src;
        size_t n = siz;

        /* Copy as many bytes as will fit */
        if (n != 0 && --n != 0) {
                do {
                        if ((*d++ = *s++) == 0)
                                break;
                } while (--n != 0);
        }

        /* Not enough room in dst, add NUL and traverse rest of src */
        if (n == 0) {
                if (siz != 0)
                        *d = '\0';                /* NUL-terminate dst */
                while (*s++)
                        ;
        }

        return(s - src - 1);        /* count does not include NUL */
}
