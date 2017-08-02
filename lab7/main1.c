#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define MaxLenStr 80
#define FIFO1 "/tmp/fifo.1"

int main(int argc, char **argv){
	
	int readfd, writefd;

	//char buf[MaxLenStr]="ABCD\n\0";

	mkfifo(FIFO1, 0666);
	//printf("%s\n", buf);
	writefd = open(FIFO1,O_WRONLY,0);
	
	//printf("%s\n", buf);
	//write( writefd, buf, strlen(buf));
	close( writefd);
	/*
	sleep(2);

	readfd = open(FIFO1, O_RDONLY, 0);
	read( readfd, buf, MaxLenStr);
	printf("%s\n", buf);
	close(readfd);
	*/
	return(0);
}