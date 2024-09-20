#include "socketutil.h"
#include <stdio.h>
#include <sys/socket.h>

// TODO - Continue from Socket Programming in C time = 46:00

int main(){
    // socket accepts 3 args: domain, tcp vs udp, and protocol
    // protocol arg has something to do with underlying layers in the OSI model, but I don't fully understand right now. 
    
    // the socket function returns an int representing the file descriptor, represents the peer being connected to
    int socketFileDescriptor = createIPv4Socket();


    struct sockaddr_in *address = createIPv4Address("127.0.0.1", 2000);




    int result = connect(socketFileDescriptor, (struct sockaddr *)address, sizeof(*address));

    if(result == 0){
        printf("Connection was successful!\n");
    }

    char *line = NULL;
    size_t lineSize = 0;
    printf("Welcome to the Chat Service!\nType a message to send (type exit to quit)...\n");

    while(true){
        
        size_t charCount = getline(&line,&lineSize, stdin);
        

        if (charCount > 0){
            if(strcmp(line, "exit\n") == 0){
                break;
            }
            line[strcspn(line, "\n")] = 0; // remove newline character

            // printf("DEBUG - Var: line = %s\n", line);
            ssize_t amountWasSent = send(socketFileDescriptor, line, strlen(line), MSG_WAITALL);
        }

    }

    close(socketFileDescriptor);
    shutdown(socketFileDescriptor, SHUT_RDWR);


    return 0;
}

