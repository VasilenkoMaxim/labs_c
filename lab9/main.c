#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/msg.h>

#define MaxLenStr 4096
#define MaxChildProc 1024

struct vector{
	float x;
	float y;
};
typedef struct vector vectors;
struct  
{
	
};
//к вектору v1 добавляет вектор v2
void add_vectors(vectors *v1, vectors *v2){
	v1->x+=v2->x;
	v1->y+=v2->y;	
}


//Входные аргументы main: 1) Размер карты по x; 2) Размер карты по y;
//3) Количество целей; 4) Количество разведчиков
int main(int argc, char **argv){
	int max_x;
	int max_y;
	if ( argc < 5 ){
		fprintf( stderr, "%s\n", "Нужно четыре целочисленных входных аргумента");
		return 1;
	}
	int x = atoi(argv[1]);
	int y = atoi(argv[2]);
	int Naim = atoi(argv[3]);
	int Nscout = atoi(argv[4]);
	if ( Nscout > MaxChildProc ){
		fprintf(stderr, "%s\n", "Слишком много разведчиков, <= 1024");
		return 1;
	}



	pid_t pid[Nscout];
	int parent_pid=getpid();
	int chaild_pid;
	for (int i = 0; i < N; ++i)
	{
		pid[i]=fork();
		if (!pid[i]){
			exit(0);
		}	
	}
	
	return(0);
}
