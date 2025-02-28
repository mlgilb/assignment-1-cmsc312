#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define INT_KEY 1234
#define STR_KEY 5678
#define PID_KEY 9101

int main() {
    int shmid_int = shmget(INT_KEY, sizeof(int), 0666);
    if (shmid_int == -1) {
        perror("shmget INT failed");
        exit(1);
    }

    int shmid_str = shmget(STR_KEY, 50 * sizeof(char), 0666);
    if (shmid_str == -1) {
        perror("shmget STR failed");
        exit(1);
    }

    int shmid_pid = shmget(PID_KEY, sizeof(int), 0666);
    if (shmid_pid == -1) {
        perror("shmget PID failed");
        exit(1);
    }

    int *shared_int = (int *)shmat(shmid_int, NULL, 0);
    if (shared_int == (void *)-1) {
        perror("shmat INT failed");
        exit(1);
    }

    char *shared_str = (char *)shmat(shmid_str, NULL, 0);
    if (shared_str == (void *)-1) {
        perror("shmat STR failed");
        exit(1);
    }

    int *shared_pid = (int *)shmat(shmid_pid, NULL, 0);
    if (shared_pid == (void *)-1) {
        perror("shmat PID failed");
        exit(1);
    }

    while (*shared_int != 10) {
        printf("Process B waiting, current value: %d\n", *shared_int);
        sleep(1);
    }

    *shared_int = 20;
    sprintf(shared_str, "I AM PROCESS B");
    *shared_pid = getpid();

    shmdt(shared_int);
    shmdt(shared_str);
    shmdt(shared_pid);
    return 0;
}
