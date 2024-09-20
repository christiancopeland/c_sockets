#include "socketutil.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

// TODO - Continue Socket Programming in C from time = 46:00


struct AcceptedSocket
{

    int acceptedSocketFileDescriptor;
    struct sockaddr_in *address;
    int error;
    bool acceptedSuccessfully;

};

void receiveAndPrintIncomingData(int socketFileDescriptor);

struct AcceptedSocket * acceptIncomingConnection(int serverSocketFileDescriptor);

void startAcceptingIncomingConnections(int serverSocketFileDescriptor);

void acceptNewConnectionAndReceiveAndPrintItsData(int serverSocketFileDescriptor);

void receiveAndPrintIncomingDataOnSeparateThread(struct AcceptedSocket *pSocket);

void* receiveAndPrintIncomingDataWrapper(void* socketFileDescriptor);


void startAcceptingIncomingConnections(int serverSocketFileDescriptor)
{

    while(true)
    {
        struct AcceptedSocket* clientSocket = acceptIncomingConnection(serverSocketFileDescriptor);

        receiveAndPrintIncomingDataOnSeparateThread(clientSocket);
    }
    // pthread_t id;
    // pthread_create(&id, NULL, acceptNewConnectionAndReceiveAndPrintItsData, serverSocketFileDescriptor);

}


void acceptNewConnectionAndReceiveAndPrintItsData(int serverSocketFileDescriptor)
{


}


void receiveAndPrintIncomingDataOnSeparateThread(struct AcceptedSocket *pSocket)
{
    pthread_t id;
    pthread_create(&id, NULL, receiveAndPrintIncomingDataWrapper, (void*)&pSocket->acceptedSocketFileDescriptor );
    
}


// TODO - Catch the case when a user X's out of their terminal. Try poll or select functions to make sure that the socket is readable maybe?
void receiveAndPrintIncomingData(int socketFileDescriptor)
{
    char buffer[1024];
    while(true){
            bzero(buffer, 1024);
            ssize_t amountReceived = recv(socketFileDescriptor, buffer, 1024, 0);
            if(amountReceived > 0){
                printf("Client: %s\n", buffer);
            }

            else if(amountReceived == 0){
                bzero(buffer, 1024);
                printf("Connection closed by client");
                break;
            }
            else {
            {
                bzero(buffer, 1024);
                perror("recv");
                break;
            }
            }
            
        }

        close(socketFileDescriptor);
}


struct AcceptedSocket * acceptIncomingConnection(int serverSocketFileDescriptor) {
    struct sockaddr_in * clientAddress;
    unsigned int clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFileDescriptor = accept(serverSocketFileDescriptor, (struct sockaddr *)clientAddress, &clientAddressSize);

    struct AcceptedSocket* acceptedSocket = malloc(sizeof(struct AcceptedSocket));
    acceptedSocket->address = clientAddress;
    acceptedSocket->acceptedSocketFileDescriptor = clientSocketFileDescriptor;
    acceptedSocket->acceptedSuccessfully = clientSocketFileDescriptor > 0;
    if(!acceptedSocket->acceptedSuccessfully)
    {
        acceptedSocket->error = clientSocketFileDescriptor;
    }


    return acceptedSocket;
}



void* receiveAndPrintIncomingDataWrapper(void* socketFileDescriptor) {
    receiveAndPrintIncomingData(*(int*)socketFileDescriptor);
    return NULL;
}


int main() {
    
    int serverSocketFileDescriptor = createIPv4Socket();
    struct sockaddr_in *serverAddress = createIPv4Address("", 2000);

    int bindResult = bind(serverSocketFileDescriptor, (struct sockaddr *)serverAddress, sizeof(*serverAddress));

    if(bindResult == 0){
        printf("Server socket was bound successfully\n");
    }

    int listenResult = listen(serverSocketFileDescriptor, 10);


    startAcceptingIncomingConnections(serverSocketFileDescriptor);
    
    shutdown(serverSocketFileDescriptor, SHUT_RDWR);

    return 0;
}
