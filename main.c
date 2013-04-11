#include <stdlib.h>
#include <unistd.h>
#include <string.h>


char* get_xor(char*,char*);

int main()
{
	char *x = get_xor("Testo di prova ","df546tgfg435trf");

	printf("%s\n",x);
	return 0;
}
char* get_xor(char *r,char *s)
{
	char *value = (char* ) malloc(strlen(s) * sizeof(char));

	int i;
	for(i = 0; i < strlen(s); i++)
		value[i] = s[i] ^ r[i];

	return value;
	
}

