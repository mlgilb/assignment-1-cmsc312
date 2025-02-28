#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    int *shared_int = (int *)shmat(shmid, NULL, 0);
    *shared_int = 10;  // A initializes shared memory
    printf("10 I AM PROCESS A whose id is: %d\n", getpid());

    pid_t pidB = fork();
    if (pidB == 0) {  // Child process B
        while (*shared_int != 10) { sleep(1); }  // Ensure A runs first
        *shared_int = 20;
        printf("20 I AM PROCESS B whose id is: %d\n", getpid());
        exit(0);
    }

    pid_t pidC = fork();
    if (pidC == 0) {  // Child process C
        while (*shared_int != 20) { sleep(1); }  // Wait for B to write 20
        *shared_int = 30;
        printf("30 I AM PROCESS C whose id is: %d\n", getpid());
        exit(0);
    }

    // A waits for both children
    wait(NULL);
    wait(NULL);

    // A prints final message
    printf("Good Bye World, I am Done!!\n");

    shmdt(shared_int);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

