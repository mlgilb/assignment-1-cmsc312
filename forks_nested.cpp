#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    int *shared_int = (int *)shmat(shmid, NULL, 0);
    *shared_int = 10;

    pid_t pidB = fork();
    if (pidB == 0) {
        while (*shared_int != 10) sleep(1);
        *shared_int = 20;

        pid_t pidC = fork();
        if (pidC == 0) {
            while (*shared_int != 20) sleep(1);
            *shared_int = 30;
            exit(0);
        }

        wait(NULL);
        exit(0);
    }

    wait(NULL);
    printf("Final Value: %d\n", *shared_int);
    shmdt(shared_int);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
