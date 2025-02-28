#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define INT_KEY 1234
#define STR_KEY 5678
#define PID_KEY 9101

int main() {
    int shmid_int = shmget(INT_KEY, sizeof(int), 0666);
    int shmid_str = shmget(STR_KEY, 50 * sizeof(char), 0666);
    int shmid_pid = shmget(PID_KEY, sizeof(int), 0666);

    if (shmid_int == -1 || shmid_str == -1 || shmid_pid == -1) {
        perror("shmget failed");
        exit(1);
    }

    int *shared_int = (int *)shmat(shmid_int, NULL, 0);
    char *shared_str = (char *)shmat(shmid_str, NULL, 0);
    int *shared_pid = (int *)shmat(shmid_pid, NULL, 0);

    if (shared_int == (void *)-1 || shared_str == (void *)-1 || shared_pid == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // wait for b
    while (*shared_int != 20) {  
        sleep(1);
    }

    *shared_int = 30;
    snprintf(shared_str, 50, "I AM PROCESS C");
    *shared_pid = getpid();

    shmdt(shared_int);
    shmdt(shared_str);
    shmdt(shared_pid);

    return 0;
}
