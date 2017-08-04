#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>

int main(int argc, char **argv[]) {
    char buf[10]="123\n\0";
    //sprintf( buf, "%d\n", 123);
    strcat( buf, "12345\n\0");
    printf("%s %d\n", buf, strlen(buf));
    return 0;
}
//---------------
