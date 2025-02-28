#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    int *shared_int = (int *)shmat(shmid, NULL, 0);
    
    *shared_int = 10;
    printf("10 I AM PROCESS A whose id is: %d\n", getpid());

    pid_t pidB = fork();
    if (pidB == 0) {  
        while (*shared_int != 10) sleep(1); 
        *shared_int = 20;
        printf("20 I AM PROCESS B whose id is: %d\n", getpid());
        exit(0);
    }

    pid_t pidC = fork();
    if (pidC == 0) {  
        while (*shared_int != 20) sleep(1); 
        *shared_int = 30;
        printf("30 I AM PROCESS C whose id is: %d\n", getpid());
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    printf("Good Bye World, I am Done!!\n");

    shmdt(shared_int);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
