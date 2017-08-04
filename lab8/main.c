#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/msg.h>
//#include <sys/siginfo.h>

#define MaxLenStr 4096
#define MaxChildProc 1024
struct mymsgpids {
        long mtype;
        int mpids[MaxChildProc];
        int mpidscount;
};
void send_msg_pids(int msgqid, struct mymsgpids *msg, long type, int *pids, int pids_count){
    msg->mtype = type;
    if (pids){
    	msg->mpidscount = pids_count;
	    for (int i = 0; i < pids_count; ++i)
	    {
	    	msg->mpids[i] = pids[i];
	    }
	}
    if( msgsnd( msgqid, (struct mymsgpids *)msg, sizeof(*msg)-sizeof(long), 0) == -1 ){
        perror("msgsnd");
        exit(1);
	}
};
int read_msg_pids(int msgqid, struct mymsgpids *msg, long type){
    return msgrcv(msgqid, (struct mymsgstr *)msg, sizeof(*msg)-sizeof(long), type, 0);
};
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
};
int write_pid_to_str(char *str, pid_t *pids, int pids_count, int max_len_str){
	str[0]='\0';
	char buf[12];
	for (int i = 0; i < pids_count; ++i)
	{	
			sprintf( buf, "%d\n", pids[i]);
			max_len_str-=strlen(buf);
			if ( max_len_str > 1 ){
				strcat(str, buf);
			}else{
				return -1;
			}

	}
	str[max_len_str]='\0';
	return 0;
};



int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "%s\n", "Должен быть входной аргумент, целое беззнаковвое число");
		return 1;
	}
	int N=atoi(argv[1]);
	if ( N > MaxChildProc ){
		fprintf(stderr, "%s\n", "Введеное число слишком большое, надо не более 1024");
		return 1;
	}	
	struct mymsgpids msg;
	int msgqid;
	key_t key;
    int qtype = 1;
    key = ftok(".", 'm'); //почитать про ftok
    if((msgqid = msgget(key, IPC_CREAT|0660)) == -1) {
		perror("msgget");
		exit(1);
	}

	sigset_t set;
    sigemptyset( &set );
    sigaddset( &set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL); // Разобраться зачем это нужно, без этой строчки не работает                                              
    int sig;
    siginfo_t si;

	pid_t pid[N];
	int parent_pid=getpid();
	int chaild_pid;
	int the_one_who_will_be_killed=-1;
	for (int i = 0; i < N; ++i)
	{
		pid[i]=fork();
		if (!pid[i]){
			chaild_pid=getpid();
			srand(chaild_pid);
			sigwait( &set, &sig);
			read_msg_pids(msgqid, &msg, qtype);
			while (msg.mpidscount!=1){
				while ( msg.mpids[(the_one_who_will_be_killed=rand()%msg.mpidscount)]==chaild_pid ){}
				kill( msg.mpids[the_one_who_will_be_killed], SIGKILL);
				printf("%d убил %d\n", chaild_pid, msg.mpids[the_one_who_will_be_killed]);
				msg.mpids[the_one_who_will_be_killed]=msg.mpids[msg.mpidscount-1];
				msg.mpids[msg.mpidscount]=0;
				msg.mpidscount--;
				usleep(100);
				send_msg_pids( msgqid, &msg, qtype, NULL, 0);
				fflush(stdout);
				usleep(10);
				read_msg_pids(msgqid, &msg, qtype);
			}
			kill( parent_pid, SIGUSR1);
			sigwait(&set, &sig);
			exit(0);
		}	
	}
	send_msg_pids( msgqid, &msg, qtype, pid, N);
	printf("%s\n", "Родительский процесс отправил сообщение");
	for (int i = 0; i < N; ++i)
	{
		printf("%d ", pid[i]);
	}
	printf("\n");
	for (int i = 0; i < N; ++i)
	{
		kill( pid[i], SIGUSR1);
	}
	sigwaitinfo( &set, &si);
	printf("Победитель: %d\n", si.si_pid);
	sigwaitinfo( &set, &si);
	return(0);
}
