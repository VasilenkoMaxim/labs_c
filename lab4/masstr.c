#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>
#include "masstr.h"

int init_masstr(masstr *M, int nstr){
	if ( (M->nchar=(int *)calloc(nstr,sizeof(int))) != NULL )  {
		if ( (M->str=(char **)calloc(nstr,sizeof(char *))) != NULL){
			M->nstr=nstr;
			return 0;
		}
		free(M->nchar);
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
