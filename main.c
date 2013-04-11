#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LENGTH 256

char* get_xor(char*,char*);
void tr_read();

int main()
{
	//char *x = get_xor("Testo di prova ","df546tgfg435trf");
	//printf("%s\n",x);
	tr_read();
	return 0;
}
void tr_read()
{
	char text[MAX_LENGTH] = "\0";
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
			printf("%s\n", text);

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

