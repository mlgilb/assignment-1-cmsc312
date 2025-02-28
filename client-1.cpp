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

    // open server queue (client needs this to send messages)
    mqd_t server_mq = mq_open(SERVER_QUEUE, O_WRONLY);
    if (server_mq == -1) {
        perror("mq_open failed (server might not be running)");
        exit(1);
    }

    // create client queue for responses
    struct mq_attr attr = {0, 10, MAX_SIZE, 0};
    mqd_t client_mq = mq_open(CLIENT_QUEUE, O_CREAT | O_RDONLY, 0666, &attr);
    if (client_mq == -1) {
        perror("mq_open failed (client queue)");
        mq_close(server_mq);
        exit(1);
    }

    char buffer[MAX_SIZE];

    while (true) {
        cout << "ENTER MESSAGE WHICH I WILL PASS TO SERVER (Press <ENTER>): " << endl;
        cin.getline(buffer, MAX_SIZE);

        if (strlen(buffer) == 0) continue;  

        if (mq_send(server_mq, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("mq_send failed");
            break;
        }

        if (mq_receive(client_mq, buffer, MAX_SIZE, NULL) == -1) {
            perror("mq_receive failed");
            break;
        }

        cout << "THE REVERSED STRING RECEIVED FROM SERVER: " << buffer << endl;
    }

    mq_close(server_mq);
    mq_close(client_mq);
    mq_unlink(CLIENT_QUEUE);

    return 0;
}
