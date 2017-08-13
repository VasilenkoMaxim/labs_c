//#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
//#include <string.h>
//#include <signal.h>
#include <fcntl.h>
//#include <sys/wait.h>
//#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
//#include <ctype.h>
#include <math.h>


#define MaxLenStr 4096
#define MaxChildProc 1024

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct semiinfo *__buf;
}


struct vector{
	float x;
	float y;
};
typedef struct vector vectors;

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

	//Чтение входных параметров
	if ( argc < 5 ){
		fprintf( stderr, "%s\n", "Нужно четыре целочисленных входных аргумента");
		return 1;
	}
  	srand( (unsigned) (time(NULL)/2) );
	int x = atoi(argv[1]);
	int y = atoi(argv[2]);
	int Nmap=x*y; // количесво клеток на карте
	int Naim=rand()%Nmap+1;
	int Nscout = atoi(argv[4]);
	if ( Nscout > MaxChildProc ){
		fprintf(stderr, "%s\n", "Слишком много разведчиков, <= 1024");
		return 1;
	}

	//Создаем сегмент разделяемой памяти для поля с целями
	if ( (int shmid_map = shmget(IPC_PRIVATE, sizeof(char)*Nmap, 0444 | IPC_CREAT)) == -1 ){
		perror("shmget fuck up");
		exit(1);
	}
	//Создаем сегмент разделяемой памяти для поля с информацией о просканированых полях
	if ( (int shmid_scan = shmget(IPC_PRIVATE, sizeof(int)*Nmap, 0666 | IPC_CREAT)) == -1 ){
		perror("shmget fuck up");
		exit(1);
	}
	//Инициализируем сегменты разделяемой памяти
	if ( ((char *shm_map = (char *) shmat(shmid_map, NULL, 0)) == (char *) -1) ){
		perror("shmat fuck up");
		exit(1);	
	}
	if ( ((int *shm_scan = (int *) shmat(shmid_scan, NULL, 0)) == (int *) -1) ){
		perror("shmat fuck up");
		exit(1);	
	}
	for (int i = 0; i < Nmap; ++i)
	{
		shm_map[i]=0;
		shm_scan[i]=0;
	}
	int j;
	for (int i = 0; i < Naim; ++i)
	{	
		while ( shm_map[( j=rand()%Nmap )] ){};
		shmid_map[j]=1;
	}
	if ( shmdt(shm_map) == -1 || shmdt(shm_scan) == -1){
		perror("shmdt fuck up");
		exit(1);	
	}
	//Главный родительский процесс инициализировал карту

	//Создаем семафор для контроля доступа к shmid_scan
	if ( (int semid_scan = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1 ){
		perror("semget fuck up");
		exit(1);
	}
	//Инициализация семафора
	struct sembuf lock_res = { 0, -1, 0};
	struct sembuf relock_res = { 0, 1, 0};
	union semun arg;
	arg.val=1;
	if ( semctl( semid_scan, 0, SETVAL, arg) == -1 ){
		perror("semctl fuck up");
		exit(1);
	}

	pid_t pid[Nscout];
	int check1[Nscout];
	int check2[Nscout];
	for (int i = 0; i < Nscout; ++i)
	{
		check1[i]=0;
		check2[i]=0;
	}
	int parent_pid=getpid();
	int chaild_pid;

	//Определяем точку старта
	vectors cur_pos={ (float) (rand()%(x-2)+1), (float) (rand()%(y-2)+1)};
	float velocity = 1.0;
	float pi = 3.14;
	vectors course = { 0.0, 0.0}; 
	for (int i = 0; i < N; ++i)
	{
		pid[i]=fork();
		if (pid[i]==-1){
			perror("fork fuck up");
			exit(1);
		}else if (!pid[i]){
			chaild_pid=getpid();
			//Дочерний процесс определяет свой курс
			course = { velocity*((float) sin(2*pi*i/N)), velocity*((float) sin(2*pi*i/N))};
			if ( ((char *shm_map = (char *) shmat(shmid_map, NULL, 0)) == (char *) -1) ){
				perror("shmat fuck up");
				exit(1);	
			}
			if ( ((int *shm_scan = (int *) shmat(shmid_scan, NULL, 0)) == (int *) -1) ){
				perror("shmat fuck up");
				exit(1);	
			}		
			do{
				if ( semop( semid_scan, &lock_res, 1) == -1 ){
					perror("semop fuck up");
					exit(1);
				}
				j=((int) cur_pos.x)+(x-1)*((int) cur_pos.y);
				if ( shm_map[j] ){
					check1[i]++;
					if ( !shm_scan[j] ){
						check2[i]++;
						
					}
				}
			}while ( ((int) cur_pos.x > 0) && ((int) cur_pos.x < x-1) && ((int) cur_pos.y > 0) && ((int) cur_pos.y < y-1));
			exit(0);
		}	
	}
	
	return(0);
}
