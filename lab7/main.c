#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
//#include <sys/siginfo.h>

#define MaxLenStr 4096
//#define FIFO "/tmp/lab7_fifo.1"
#define FIFO1 "/tmp/fifo.1"

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
	int readfd, writefd;
	mkfifo(FIFO1, 0666);
	/*
	int fd[N][2];
	for (int i = 0; i < N; ++i){
		pipe(fd[i]);	
	}
	*/
	sigset_t set;
    sigemptyset( &set );
    sigaddset( &set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL); // Разобраться зачем это нужно, без этой строчки не работает                                              
    int sig;
    siginfo_t si;
    
	pid_t pid[N];
	char buf[MaxLenStr]="\0";
	char bufbuf[20];
	int parent_pid=getpid();
	int chaild_pid;
	int the_one_who_will_be_killed=-1;
	for (int i = 0; i < N; ++i)
	{
		pid[i]=fork();
		if (!pid[i]){
			chaild_pid=getpid();
			srand(chaild_pid);
			
			//close(fd[i][1]);
			printf("%d %s\n", chaild_pid, "abcd");
			readfd = open(FIFO1, O_RDONLY, 0);
			printf("%d %s\n", chaild_pid, "abcd");
			read( readfd, buf, MaxLenStr);
			printf("%s\n", buf);
			close(readfd);
			sigwait( &set, &sig);
			N=read_pid_from_str(buf, pid);
			do{
				while ( pid[(the_one_who_will_be_killed=rand()%N)]==chaild_pid ){};
				kill( pid[the_one_who_will_be_killed], SIGKILL);
				printf("%d убил %d\n", chaild_pid, pid[the_one_who_will_be_killed]);
				pid[the_one_who_will_be_killed]=pid[N-1];
				usleep(10);
				N--;
			}while(N!=1);
			kill(parent_pid, SIGUSR1);
			sigwait( &set, &sig);
			exit(0);
		}	
	}
	for (int i = 0; i < N; ++i){	
		sprintf(bufbuf, "%d\n", pid[i]);
		strcat(buf, bufbuf);
	}
	writefd = open( FIFO1, O_WRONLY, 0);
	/*
	for (int i = 0; i < N; ++i)
	{
		close( fd[i][0]);
		write( fd[i][1], buf, strlen(buf)+1);	
	}
	*/
	write( writefd, buf, strlen(buf)+1);
	close( writefd);
	printf("%s\n", "abcde");
	for (int i = 0; i < N; ++i)
	{
		kill( pid[i], SIGUSR1);
	}
	sigwaitinfo( &set, &si);
	printf("Победитель: %d; PIDmin = %d, PIDmax = %d\n", si.si_pid, pid[0], pid[N-1]);
	sigwait( &set, &sig);
	return(0);
}