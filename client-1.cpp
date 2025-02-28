#include <iostream>
#include <mqueue.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

#define CLIENT_QUEUE "/client_queue"
#define SERVER_QUEUE "/server_queue"
#define MAX_SIZE 64

int main() {
    mqd_t mq_client, mq_server;
    char buffer[MAX_SIZE];

    // Open message queues
    mq_client = mq_open(CLIENT_QUEUE, O_WRONLY | O_CREAT, 0666, NULL);
    mq_server = mq_open(SERVER_QUEUE, O_RDONLY | O_CREAT, 0666, NULL);

    if (mq_client == -1 || mq_server == -1) {
        perror("Client: mq_open");
        exit(1);
    }

    std::cout << "I AM CLIENT. ENTER MESSAGE WHICH I WILL PASS TO SERVER (Press ENTER):\n";

    while (true) {
        std::cin.getline(buffer, MAX_SIZE);
        if (strcmp(buffer, "exit") == 0) break;

        mq_send(mq_client, buffer, strlen(buffer) + 1, 0);
        mq_receive(mq_server, buffer, MAX_SIZE, NULL);

        std::cout << "THE REVERSED STRING RECEIVED FROM SERVER: " << buffer << std::endl;
    }

    mq_close(mq_client);
    mq_close(mq_server);
    mq_unlink(CLIENT_QUEUE);
    return 0;
}
