#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	char *str;
	str=(char *)calloc(10,sizeof(char));
	for (int i = 0; i < 10; ++i)
	{
		printf("%c\n",  str[i] );
	}
	sprintf(str, "%s\n", "abc\nefgh");
	printf("%d\n", strlen(str));
	for (int i = 0; i < 10; ++i)
	{
		printf("%d\n", ( str[i] == '\0'));
	}
	for (int i = 0; i < 10; ++i)
	{
		printf("%c\n",  str[i] );
	}
	return 0;
}