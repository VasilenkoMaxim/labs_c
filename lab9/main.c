#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <math.h>
#include <ncursesw/ncurses.h>

#define MaxLenStr 4096
#define MaxChildProc 1024
#define VELOCITY 1.0
#define PI 3.14

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct semiinfo *__buf;
};

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

void int_vectors(int *x, int *y, vectors *v){
	*x=(int) round(v->x);
	*y=(int) round(v->y);
}

int read_input_scouts( int argc, char **argv, int *x, int *y, int *Nscout){
	if ( argc < 4 ){
		fprintf( stderr, "%s\n", "Нужно три целочисленных входных аргумента");
		return 101;
	}
	*x = atoi(argv[1]);
	*y = atoi(argv[2]);
	*Nscout = atoi(argv[3]);
	if ( *Nscout > MaxChildProc ){
		fprintf(stderr, "%s\n", "Слишком много разведчиков, <= 1024");
		return 102;
	}
	return 0;
}

int init_map_scouts( char *shm_map, vectors *shm_scout_pos, int Naim, int Nmap, int Nscout, vectors start_pos){
	for (int i = 0; i < Nscout; ++i){
		shm_scout_pos[i]=start_pos;
	}
	for (int i = 0; i < Nmap; ++i){
		shm_map[i]=0;
	}
	int j;
	for (int i = 0; i < Naim; ++i){	
		while ( shm_map[ j=rand()%Nmap ] ){};
		shm_map[j]=1;
	}
	return 0;
}

int print_array_ncurses(char *arr, int x, int y, int max_x, int max_y, chtype *ch){
	if ( max_y<y || ((int) (max_x/2))<x ){
		return -1;
	}
	int dx=(int) ( ((int) (max_x/2))-x )/2;
	int dy=(int) (max_y-y)/2;
	for (int i = 0; i < y; ++i)
	{
		move(dy+i,2*dx);
		for (int j = 0; j < x; ++j)
		{
			addch(ch[arr[i*x+j]]);
			addch(ch[arr[i*x+j]]);			
		}
	}
	curs_set(0);
	refresh();
}

//Входные аргументы main: 1) Размер карты по x; 2) Размер карты по y; 3) Количество разведчиков
int main(int argc, char **argv){
	int max_x, max_y;
	initscr();                   // Переход в curses-режим
	getmaxyx(stdscr, max_y, max_x);
	if (!has_colors())
	{
	  endwin();
	  printf("Цвета не поддерживаются");
	  exit(1);
	}
	start_color();
	// 1 цвет в палитре - красные символы на чёрном фоне
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	init_pair(2, COLOR_WHITE, COLOR_RED);
	init_pair(3, COLOR_WHITE, COLOR_BLUE);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);
	chtype ch[3];
	chtype ch_scout;
	ch[0] = ' ' | COLOR_PAIR(1);
	ch[1] = ' ' | COLOR_PAIR(2);
	ch[2] = ' ' | COLOR_PAIR(3);

	int err;
	srand( (unsigned) (time(NULL)/2) );
	
	//Чтение входных параметров: 3 целых числа
	int x, y, Nscout;
	if ( err = read_input_scouts( argc, argv, &x, &y, &Nscout) ){
		exit(err);
	}
	int Nmap=x*y; // количесво клеток на карте
	int Naim=rand()%Nmap+1; //количество целей
	//Создаем сегмент разделяемой памяти для поля с целями и с информацией о координатах разведчиков
	int shmid_map, shmid_scout_pos, shmid_finding_aims;
	shmid_map = shmget(IPC_PRIVATE, sizeof(char)*Nmap, 0444 | IPC_CREAT); 
	shmid_scout_pos = shmget(IPC_PRIVATE, sizeof(vectors)*Nscout, 0666 | IPC_CREAT);
	shmid_finding_aims = shmget(IPC_PRIVATE, sizeof(int)*Nscout, 0666 | IPC_CREAT);
	if ( shmid_map == -1 || shmid_scout_pos == -1 || shmid_finding_aims == -1){
		perror("shmget fuck up");
		exit(1);
	}
	//Инициализируем сегменты разделяемой памяти
	vectors cur_pos={ (float) (rand()%(x-2)+2), (float) (rand()%(y-2)+2)}; //случайно выбираем место старта разведчиков, не на границе карты
	char *shm_map; vectors *shm_scout_pos; int *shm_finding_aims;
	shm_map = (char *) shmat( shmid_map, NULL, 0);
	shm_scout_pos = (vectors *) shmat( shmid_scout_pos, NULL, 0);
	shm_finding_aims = (int *) shmat( shmid_finding_aims, NULL, 0);
	if ( shm_map == (char *) -1 || shm_scout_pos == (vectors *) -1 || shm_finding_aims == (int) -1 ){ perror("shmat fuck up"); exit(1); };
	init_map_scouts( shm_map, shm_scout_pos, Naim, Nmap, Nscout, cur_pos);
	//if ( shmdt(shm_map) == -1 || shmdt(shm_scout_pos) == -1){ return 102; };
	
	//Напечатаем карту без разведчиков
	print_array_ncurses(shm_map, x, y, max_x, max_y, &ch);

	//Создаем семафор для контроля доступа к shmid_scan, а вот зачем непонятно?
	int semid;
	if ( ( semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1 ){
		perror("semget fuck up");
		exit(1);
	}
	//Инициализация семафора
	struct sembuf lock_res = { 0, -1, 0};
	struct sembuf relock_res = { 0, 1, 0};
	union semun arg;
	arg.val=1;
	if ( semctl( semid, 0, SETVAL, arg) == -1 ){
		perror("semctl fuck up");
		exit(3);
	}

	pid_t pid[Nscout];
	int parent_pid=getpid();
	for (int i = 0; i < Nscout; ++i)
	{
		pid[i]=fork();
		if (pid[i]==-1){
			perror("fork fuck up");
			exit(1);
		}else if (!pid[i]){
			int chaild_pid=getpid();
			//Дочерний процесс определяет свой курс
			vectors course = { (float) VELOCITY*((float) sin(2*PI*i/Nscout)), (float) VELOCITY*((float) cos(2*PI*i/Nscout))};
			int x_pos, y_pos;
			int_vectors( &x_pos, &y_pos, &shm_scout_pos[i]);
			int last_j=-1;
			int j=(y_pos-1)*x+(x_pos-1);
			while ( x_pos>=1 && x_pos<=x && y_pos>=1 && y_pos<=y ){
				if ( j!=last_j ){
					if ( shm_map[j]==1 ){ shm_finding_aims[i]++; };
				}
				last_j=j;
				add_vectors( &shm_scout_pos[i], &course);
				int_vectors( &x_pos, &y_pos, &shm_scout_pos[i]);
				usleep(300000);
				j=(y_pos-1)*x+(x_pos-1);
				//printf("%d %d %d\n", chaild_pid, x_pos, y_pos);
			}
			
			exit(0);
		}	
	}
	int x_pos, y_pos, j;
	int k=1;
	char map_and_scout[Nmap];
	while(k){
		k=0;
		for (int i = 0; i < Nmap; ++i)
		{
			map_and_scout[i]=shm_map[i];
		}
		for (int i = 0; i < Nscout; ++i)
		{
			int_vectors( &x_pos, &y_pos, &shm_scout_pos[i]);
			j=(y_pos-1)*x+(x_pos-1);
			if ( x_pos>=1 && x_pos<=x && y_pos>=1 && y_pos<=y){
				map_and_scout[j]=2;
				k++;
			}
		}
		print_array_ncurses(map_and_scout, x, y, max_x, max_y, &ch);
		usleep(10000);
		print_array_ncurses(shm_map, x, y, max_x, max_y, &ch);
	}
	endwin();                    // Выход из curses-режима. Обязательная команда.
	for (int i = 0; i < Nscout; ++i)
	{
		printf("Разведчик №%d: PID = %d, обнаружено %d целей\n", i+1, pid[i], shm_finding_aims[i]);
	}
	return(0);
}
