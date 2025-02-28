#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>

#define SERVER_QUEUE "/server_queue"
#define CLIENT_QUEUE "/client_queue"
#define MAX_SIZE 64

using namespace std;

void reverseString(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        swap(str[i], str[len - i - 1]);
    }
}

int main() {
    cout << "I AM SERVER! I WILL RECEIVE MESSAGE FROM CLIENT AND WILL SEND REVERSE OF THIS" << endl;

    struct mq_attr attr = {0, 10, MAX_SIZE, 0};

    // create server queue (to receive messages from client)
    mqd_t server_mq = mq_open(SERVER_QUEUE, O_CREAT | O_RDONLY, 0666, &attr);
    if (server_mq == -1) {
        perror("mq_open failed (server queue)");
        exit(1);
    }

    // open client queue (to send responses back)
    mqd_t client_mq = mq_open(CLIENT_QUEUE, O_CREAT | O_WRONLY, 0666, &attr);
    if (client_mq == -1) {
        perror("mq_open failed (client queue)");
        mq_unlink(SERVER_QUEUE);
        exit(1);
    }

    char buffer[MAX_SIZE];

    while (true) {
        if (mq_receive(server_mq, buffer, MAX_SIZE, NULL) == -1) {
            perror("mq_receive failed");
            break;
        }

        cout << "MESSAGE RECEIVED FROM CLIENT: " << buffer << endl;
        reverseString(buffer);

        if (mq_send(client_mq, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("mq_send failed");
            break;
        }

        cout << "SENT REVERSE OF MESSAGE TO CLIENT" << endl;
    }

    mq_close(server_mq);
    mq_close(client_mq);
    mq_unlink(SERVER_QUEUE);
    mq_unlink(CLIENT_QUEUE);

    return 0;
}
