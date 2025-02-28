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

// Function to reverse a string
void reverseString(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        swap(str[i], str[len - i - 1]);
    }
}

int main() {
    cout << "I AM SERVER! I WILL RECEIVE MESSAGE FROM CLIENT AND WILL SEND REVERSE OF THIS" << endl;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;  
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // Create and open message queue for receiving from client
    mqd_t server_mq = mq_open(SERVER_QUEUE, O_CREAT | O_RDONLY, 0666, &attr);
    if (server_mq == -1) {
        perror("Server message queue creation failed");
        exit(1);
    }

    // Create and open message queue for sending back to client
    mqd_t client_mq = mq_open(CLIENT_QUEUE, O_CREAT | O_WRONLY, 0666, &attr);
    if (client_mq == -1) {
        perror("Client message queue creation failed");
        mq_unlink(SERVER_QUEUE);
        exit(1);
    }

    char buffer[MAX_SIZE];

    while (true) {
        memset(buffer, 0, MAX_SIZE);

        // Receive message from the client
        ssize_t bytes_read = mq_receive(server_mq, buffer, MAX_SIZE, NULL);
        if (bytes_read == -1) {
            perror("Server receiving failed");
            break;
        }

        cout << "MESSAGE RECEIVED FROM CLIENT: " << buffer << endl;

        // Reverse the received string
        reverseString(buffer);

        // Send the reversed string back to the client
        if (mq_send(client_mq, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("Server sending failed");
            break;
        }

        cout << "SENT REVERSE OF MESSAGE TO CLIENT" << endl;
    }

    // Cleanup
    mq_close(server_mq);
    mq_close(client_mq);
    mq_unlink(SERVER_QUEUE);
    mq_unlink(CLIENT_QUEUE);

    return 0;
}

