#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define INT_KEY 1234
#define STR_KEY 5678
#define PID_KEY 9101

int main() {
    int shmid_int = shmget(INT_KEY, sizeof(int), IPC_CREAT | 0666);
    int shmid_str = shmget(STR_KEY, 50 * sizeof(char), IPC_CREAT | 0666);
    int shmid_pid = shmget(PID_KEY, sizeof(int), IPC_CREAT | 0666);

    int *shared_int = (int *)shmat(shmid_int, NULL, 0);
    char *shared_str = (char *)shmat(shmid_str, NULL, 0);
    int *shared_pid = (int *)shmat(shmid_pid, NULL, 0);

    *shared_int = 10;
    sprintf(shared_str, "I AM PROCESS A");
    *shared_pid = getpid();
    printf("%d %s whose id is: %d\n", *shared_int, shared_str, *shared_pid);

    int printed_B = 0; // Flag to track Process B output
    int printed_C = 0; // Flag to track Process C output

    while (!printed_C) {  // Keep looping until Process C updates shared memory
        if (*shared_int == 20 && !printed_B) {  // Print B only once
            printf("%d %s whose id is: %d\n", *shared_int, shared_str, *shared_pid);
            printed_B = 1;
        }

        if (*shared_int == 30 && !printed_C) {  // Print C only once
            printf("%d %s whose id is: %d\n", *shared_int, shared_str, *shared_pid);
            printed_C = 1;
        }

        sleep(1);
    }

    printf("Good Bye World, I am Done!!\n");

    shmdt(shared_int);
    shmdt(shared_str);
    shmdt(shared_pid);
    shmctl(shmid_int, IPC_RMID, NULL);
    shmctl(shmid_str, IPC_RMID, NULL);
    shmctl(shmid_pid, IPC_RMID, NULL);

    return 0;
}

