#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#define MaxCharStr 1024
#define MaxStr 1024
#define MinMas 11
#define MaxMas 20
#define MinLenMas 1
#define MaxLenMas 1000

double mymean(double *mas, int Nmas);

double mymean(double *mas, int Nmas){
	double result = 0.0;
	for (int i = 0; i < Nmas; ++i)
	{
		result=++mas[i]/Nmas;
	}
	return result;
}


int main( int argc, char **argv[]){
	//mtrace();
	int i, stime;
  	long ltime;
  	/* получает текущее календарное время */
  	ltime = time(NULL);
  	stime = (unsigned) ltime/2;
  	// обновляем ГСЧ
 	srand(stime);
	//получаем количество строк которые мы будем считать
 	int N = (int)( rand()%(MaxMas-MinMas+1) + MinMas);
 	pid_t pid[N];
 	//Массив для хранения количества элементов в двумерном массиве
 	int Nmas[N];
 	double **mas[N];
 	double *d;
 	for (int i = 0; i < N; ++i)
 	{
 		Nmas[i]=(int)( rand()%(MaxLenMas-MinLenMas+1) + MinLenMas);
 		mas[i]=(double *)calloc(Nmas[i],sizeof(double));
 		if ( mas[i] == NULL ){
 			//хорошо бы освободить уже выделенную память
 			return 1;
 		}
 		d=mas[i];
 		for (int j = 0; i < Nmas[j]; ++j)
 		{
 			d[j]=(double)( rand()/RAND_MAX );
 		}
 	}
 	//запускаем через for дочерние процессы
	for (int i = 0; i < N; ++i)
	{
		pid[i]=fork();
		if ( pid[i]==0 ){
			printf("Я дочерний процесс №%d и мой PID = %d, длина моего массива = %d, среднее значение моего массива = %f\n", i, getpid(), Nmas[i], mymean( mas[i], Nmas[i]));
			exit(0);
		}
	}
	int status;
	for (i = 0; i< N; i++) { 
    	waitpid(pid[i],&status,0);
    }
	printf("Я родительский процесс и мой PID = %d\n", getpid());

	for (int i = 0; i < N; ++i)
	{
		free(mas[i]);
	}
	return 0;
}