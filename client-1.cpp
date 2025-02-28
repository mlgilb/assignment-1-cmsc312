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

int main() {
    cout << "I AM CLIENT. ENTER MESSAGE WHICH I WILL PASS TO SERVER (Press <ENTER>):" << endl;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;  
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // Ensure the server queue exists before proceeding
    mqd_t server_mq = mq_open(SERVER_QUEUE, O_WRONLY);
    if (server_mq == -1) {
        perror("Client failed to open server queue (Is the server running?)");
        exit(1);
    }

    // Create and open client message queue
    mqd_t client_mq = mq_open(CLIENT_QUEUE, O_CREAT | O_RDONLY, 0666, &attr);
    if (client_mq == -1) {
        perror("Client message queue creation failed");
        mq_close(server_mq);
        exit(1);
    }

    char buffer[MAX_SIZE];

    while (true) {
        memset(buffer, 0, MAX_SIZE);
        cout << "ENTER MESSAGE WHICH I WILL PASS TO SERVER (Press <ENTER>): " << endl;
        cin.getline(buffer, MAX_SIZE);

        if (strlen(buffer) == 0) continue;  // Ignore empty input

        // Send message to the server
        if (mq_send(server_mq, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("Client sending failed");
            break;
        }

        // Receive the reversed message from the server
        memset(buffer, 0, MAX_SIZE);
        ssize_t bytes_read = mq_receive(client_mq, buffer, MAX_SIZE, NULL);
        if (bytes_read == -1) {
            perror("Client receiving failed");
            break;
        }

        cout << "THE REVERSED STRING RECEIVED FROM SERVER: " << buffer << endl;
    }

    // Cleanup
    mq_close(server_mq);
    mq_close(client_mq);
    mq_unlink(CLIENT_QUEUE);

    return 0;
}

