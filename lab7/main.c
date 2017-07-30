#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
//#include <sys/siginfo.h>

#define MaxLenStr 1024

/*
#define MaxStr 1024
#define MinMas 11
#define MaxMas 20
#define MinLenMas 1
#define MaxLenMas 1000
*/

//Возвращает количество считаных pid
int read_pid_from_str(char *str, pid_t *pids){
	int i=0;
	int shift=0;
	int lenstr=strlen(str);
	char buf[10];
	do
	{
		sscanf( str+shift, "%s^\n", buf);
		if (strlen(buf)){
			shift+=(strlen(buf)+1);
			pids[i]=atoi(buf);
			i++;
		}

	}while( lenstr != shift);
	return i;
}

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "%s\n", "Должен быть входной аргумент, целое беззнаковвое число");
		return 1;
	}
	int N=atoi(argv[1]);
	
	sigset_t set;
    sigemptyset( &set );
    sigaddset( &set, SIGUSR1);
    sigaddset( &set, SIGUSR2);
    sigprocmask(SIG_BLOCK, &set, NULL); // Разобраться зачем это нужно, без этой строчки не работает                                              
    int sig;
    siginfo_t si;

	int fd[2];
	pid_t pid[N];
	char buf[MaxLenStr]="\0";
	char bufbuf[20];
	pipe(fd);
	int parent_pid=getpid();
	int chaild_pid;
	int n;
	int the_one_who_will_be_killed=-1;
	for (int i = 0; i < N; ++i)
	{
		pid[i]=fork();
		if (!pid[i]){
			chaild_pid=getpid();
			srand(chaild_pid);
			/*
			do{
				sigwait( &set, &sig);
				printf("%d\n", chaild_pid);
				sleep(3);
				if (i==2){
					kill(parent_pid, SIGUSR1);
				}
				if (sig==SIGUSR1){
					N++;
				}
			}while(N<6);
			*/
			do{
				sigwait( &set, &sig);
				if(sig==SIGUSR1){
					kill(parent_pid, SIGUSR1);
				}
				if(sig==SIGUSR2){
					read( fd[0], buf, MaxLenStr);
					printf("%d Прочитал из pipe\n", chaild_pid);
					n=read_pid_from_str(buf, pid);
					buf[0]='\0';
					while ( pid[(the_one_who_will_be_killed=rand()%n)]==chaild_pid ){}
					kill( pid[the_one_who_will_be_killed], SIGKILL);
					printf("%d убил %d\n", chaild_pid, pid[the_one_who_will_be_killed]);
					for (int i = 0; i < n; ++i)
					{	
						if(i!=the_one_who_will_be_killed){
							sprintf( bufbuf, "%d\n", pid[i]);
							strcat( buf, bufbuf);
						}
					}
					n--;
					write( fd[1], buf, strlen(buf)+1);
					printf("%d сделал запись ставшихся\n", chaild_pid);
					if (n!=1){
						kill(parent_pid, SIGUSR1);
						printf("%d отослал сигнал родителю на повторение розыгрыша\n", chaild_pid);
					}else{
						kill(parent_pid, SIGUSR2);
						printf("%d отослал сигнал родителю на конец игры\n", chaild_pid);
					}
				}
			}while (n!=1);
			exit(0);
		}	
	}
	printf("Я родитель%d\n", parent_pid);
	int pid1;
	close(fd[0]);
	for (int i = 0; i < N; ++i)
	{	
		sprintf(bufbuf, "%d\n", pid[i]);
		strcat(buf, bufbuf);
	}
	write( fd[1], buf, strlen(buf)+1);
	do{
		for (int i = 0; i < N; ++i)
		{
			kill( pid[i], SIGUSR1);
		}
		sigwaitinfo( &set, &si);
		printf("%d был первым\n", si.si_pid);
		pid1=si.si_pid;
		kill( si.si_pid, SIGUSR2);
		do{	
			printf("Я родитель и жду сигнала от ребенка %d\n", pid1);
			sigwaitinfo( &set, &si);
		}while(pid1!=si.si_pid);
	}while(si.si_signo!=SIGUSR2);
	printf("Победитель: %d\n", si.si_pid);
	return(0);
}