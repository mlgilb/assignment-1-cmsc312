#include <iostream>
#include <mqueue.h>
#include <cstring>
#include <cstdlib>
#include <algorithm>

#define CLIENT_QUEUE "/client_queue"
#define SERVER_QUEUE "/server_queue"
#define MAX_SIZE 64

int main() {
    mqd_t mq_client, mq_server;
    char buffer[MAX_SIZE];

    mq_client = mq_open(CLIENT_QUEUE, O_RDONLY | O_CREAT, 0666, NULL);
    mq_server = mq_open(SERVER_QUEUE, O_WRONLY | O_CREAT, 0666, NULL);

    if (mq_client == -1 || mq_server == -1) {
        perror("Server: mq_open");
        exit(1);
    }

    std::cout << "I AM SERVER! I WILL RECEIVE MESSAGE FROM CLIENT AND WILL SEND REVERSE OF THIS\n";

    while (true) {
        mq_receive(mq_client, buffer, MAX_SIZE, NULL);

        std::cout << "MESSAGE RECEIVED FROM CLIENT: " << buffer << std::endl;
        std::reverse(buffer, buffer + strlen(buffer));

        mq_send(mq_server, buffer, strlen(buffer) + 1, 0);
        std::cout << "SENT REVERSE OF MESSAGE TO CLIENT\n";
    }

    mq_close(mq_client);
    mq_close(mq_server);
    mq_unlink(SERVER_QUEUE);
    return 0;
}
