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
struct sigaction act;

int main(int argc, char **argv){	
	
 	act.sa_handler = SIG_IGN;
	//sigaction(SIGUSR1, &act, NULL);
	//sigaction(SIGUSR2, &act, NULL);
	//sigaction(SIGKILL, &act, NULL);
	sigset_t set;
    sigemptyset( &set);
    sigaddset( &set, SIGUSR1);
    sigaddset( &set, SIGUSR2);
 //   sigaddset( &set, SIGKILL);
 	sigprocmask(SIG_BLOCK, &set, NULL); // Разобраться зачем это нужно, без этой строчки не работает                                              
    int sig;
 //   siginfo_t si;
	//sigaction(&set, &act, NULL);
    int N=1;
	pid_t pid[N];
	int parent_pid=getpid();
	int chaild_pid;

	for (int i = 0; i < N; ++i)
	{
		pid[i]=fork();
		if (!pid[i]){
			chaild_pid=getpid();
			sigwait( &set, &sig);
			printf("%d %s\n", chaild_pid, "Получен 1 сигнал");
			if (sig == SIGUSR1)
			printf("%d %s\n", chaild_pid, "Получен 1 сигнал");
			sigwait( &set, &sig);
			printf("%d %s\n", chaild_pid, "Получен 2 сигнал");
			if (sig == SIGUSR1)
			printf("%d %s\n", chaild_pid, "Получен 2 сигнал");
			sigwait( &set, &sig);
			printf("%d %s\n", chaild_pid, "Получен 3 сигнал");
			if (sig == SIGUSR1)
			printf("%d %s\n", chaild_pid, "Получен 3 сигнал");
			sigwait( &set, &sig);
			printf("%d %s\n", chaild_pid, "Получен 4 сигнал");
			if (sig == SIGUSR2)
				printf("%d %s\n", chaild_pid, "Получен 4 сигнал");
			sigwait( &set, &sig);
			printf("%d %s\n", chaild_pid, "Получен 5 сигнал");
			if (sig == SIGUSR1)
				printf("%d %s\n", chaild_pid, "Получен 5 сигнал");
			exit(0);
		}	
	}
	int status;
	kill(pid[0],SIGUSR1);
	sleep(1);
	kill(pid[0],SIGUSR1);
	sleep(1);
	kill(pid[0],SIGUSR1);
	sleep(1);
	kill(pid[0],SIGUSR2);
	sleep(1);
	kill(pid[0],SIGUSR1);
	
	waitpid(pid[0], status, NULL);
	
	return(0);
}