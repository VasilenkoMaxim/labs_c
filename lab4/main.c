#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>
#include "masstr.h"
#define MaxCharStr 1024
#define MaxStr 1024

int myfread(FILE *fp, masstr *M, int maxbufferlen){
	char buffer0[maxbufferlen];
	char buffer1[maxbufferlen];
	int nread=0;
	int check=0;
	int j=0;
	int n=0;
	while ( !feof(fp) ){
		nread=fread( buffer0, sizeof(char), maxbufferlen, fp);
		for (int i = 0; i < nread; ++i)
		{
			buffer1[j]=buffer0[i];
			if ( buffer0[i] == '\n' ){
				buffer1[j+1]='\0';
				j=0;
				add_str_masstr( M, n, buffer1);
				n++;
				check=1;
			}
			j++;
		}
		if (!check){
			return -1;
		}
	}
	return n;
}
/*
int cpytodot(char *s1, char *s2){
	char a=0;
	for (int i = lenstr(s1)-1; i >= 0; --i)
	{
		if ( a || s1[i]=='.' ){

		}else{

		}
	}
}
*/
int main( int argc, char **argv[]){
	mtrace();
	FILE *fp;
	masstr M;
	char pathstr1[MaxCharStr];
	char *pathstr0;
	char *ch1;
	int lennamefile;


	if ( argc < 3 ){
		fprintf(stderr, "%s\n", "Мало аргументов. Необходимо <имя файла> <символ>");
		exit(1);
	}
	ch1=argv[2];
	char ch=ch1[0];

	if ( (fp=fopen(argv[1],"r")) == NULL ){
		fprintf(stderr, "%s%s\n", "Не удалось открыть файл: ", argv[1]);
		exit(2);
	}
	
	init_masstr( &M, MaxStr);
	int countstr=myfread( fp, &M, MaxCharStr);

	if( fclose(fp) ){
		fprintf(stderr, "%s%s\n", "Не удалось закрыть файл: ", argv[1]);
		exit(3);
	}

	pathstr0=argv[1];
	lennamefile=strlen(pathstr0);
	while ( pathstr0[lennamefile]!='.' ){
		lennamefile--;
	}
	for (int i = 0; i < lennamefile; ++i)
	{
		pathstr1[i]=pathstr0[i];
	}
	pathstr1[lennamefile]='.';
	pathstr1[lennamefile+1]='n';
	pathstr1[lennamefile+2]='e';
	pathstr1[lennamefile+3]='w';
	pathstr1[lennamefile+4]='1';
	pathstr1[lennamefile+5]='\0';
	
	
	if ( (fp=fopen(pathstr1,"w")) == NULL ){
		fprintf(stderr, "%s%s\n", "Не удалось открыть файл: ", argv[1]);
		exit(2);
	}

	int check;
	int i=0;
	while ( M.nchar[i] )
	{	
		check=1;
		for (int j = 0; j < M.nchar[i]; ++j)
		{
			if ( *(M.str[i] + j) == ch){
				check=0;
				break;
			}
		}
		if (check){
			fwrite( M.str[i], sizeof(char), M.nchar[i]-1, fp);
		}
		i++;
	}
	if( fclose(fp) ){
		fprintf(stderr, "%s%s\n", "Не удалось закрыть файл: ", argv[1]);
		exit(3);
	}
	
	destroy_massstr( &M);
	return 0;
}