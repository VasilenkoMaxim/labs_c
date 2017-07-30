 #include <stdio.h>
    #include <signal.h>
    #include <string.h>
    #include <unistd.h>
     
    struct sigaction act;
     
    void sighandler(int signum, siginfo_t *info, int *PID)
    {
        return info->si_pid;
    }
     
    int main()
    {
        printf("I am %lu\n", (unsigned long)getpid());
        memset(&act, 0, sizeof(act));
        act.sa_sigaction = sighandler;
        act.sa_flags = SA_SIGINFO;
     
        int PID;
        sigaction(SIGTERM, &act, &PID);
        printf("%d\n", PID);
     
        // Waiting for CTRL+C...
        sleep(100);
     
        return 0;
}