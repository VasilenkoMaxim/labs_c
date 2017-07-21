#include <stdio.h>
#include <stdlib.h>
//#include "masstr.h"
#define MaxCharStr 1024
#define MaxStr 1024


struct masstr{
	char **str;
	int *nchar;
	int nstr;
};
typedef struct masstr masstr;
int init_masstr(masstr *M, int nstr);
int destroy_massstr(masstr *M);
int add_str_masstr(masstr *M, int n, char *str);
int free_str_masstr(masstr *M, int n);

int init_masstr(masstr *M, int nstr){
	if ( ((M->str=(char **)calloc(nstr,sizeof(char *))) != NULL)&&((M->nchar=(int *)calloc(nstr,sizeof(int))) != NULL) ){
		M->nstr=nstr;
		//Вроде бы calloc обнуляет значения в выделенной памяти
		/*for (int i = 0; i < M->nmax; ++i)
		{
			M->str[i]=NULL;
			M->n[i]=0;
		}*/
		return 0;
	}
	return 1;
}
int destroy_massstr(masstr *M){
	for (int i = 0; i < M->nstr; ++i)
	{
		free_str_masstr( M, i);
	}
	free(M->nchar);
	M->nchar=NULL;
	free(M->str);
	M->str=NULL;
	M->nstr=0;
	return 0;
}
int add_str_masstr(masstr *M, int n, char *str){
	if ( n>=0 && n<M->nstr ){
		if ( M->str[n]!=NULL ){
			free(M->str[n]);
			M->str[n]=NULL;
			M->nchar[n]=0;
		}
	}else{
		return 1;
	}
	M->nchar[n]=strlen(str)+1;
	if ( (M->str[n] = (char *)calloc( M->nchar[n], sizeof(char))) != NULL ){
		strcpy( M->str[n], str);
		return 0;
	}else{
		M->nchar[n]=0;
		return 2;
	}
}
//n - номер строки которая освобождается
int free_str_masstr(masstr *M, int n){
	if ( n>=0 && n<M->nstr ){
		if ( M->str[n] ){
			free(M->str[n]);
			M->str[n]=NULL;
		}
		M->nchar[n]=0;
		return 0;
	}
	else{
		return 1;
	}
}

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

int main( int argc, char **argv[]){
	FILE *fpr, *fpw;
	char pathstr1[MaxCharStr];
	char *pathstr0;
	int lennamefile;
	char symbol;

	if ( argc < 3 ){
		fprintf(stderr, "%s\n", "Мало аргументов. Необходимо <имя файла> <символ>");
		exit(1);
	}
	char ch=*(argv[2]);
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
	pathstr1[lennamefile+4]='2';
	pathstr1[lennamefile+5]='\0';

	if ( (fpr=fopen(argv[1],"r")) == NULL ){
		fprintf(stderr, "%s%s\n", "Не удалось открыть файл: ", argv[1]);
		exit(2);
	}
	if ( (fpw=fopen(pathstr1,"w")) == NULL ){
		fprintf(stderr, "%s%s\n", "Не удалось открыть файл: ", argv[1]);
		exit(2);
	}
	
	while ( (symbol=fgetc( fpr)) != EOF ){
		if (symbol == ch){
			fputc( ' ', fpw);
		}else{
			fputc( symbol, fpw);
		}
	}

	if( fclose(fpr) ){
		fprintf(stderr, "%s%s\n", "Не удалось закрыть файл: ", argv[1]);
		exit(3);
	}
	if( fclose(fpw) ){
		fprintf(stderr, "%s%s\n", "Не удалось закрыть файл: ", argv[1]);
		exit(3);
	}
	
	return 0;
}