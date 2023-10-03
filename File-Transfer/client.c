#include "socket.h"

#define PORT 8080

void Send_File(FILE *fp, int sock_fd) {
    int n ;
    char buffer[SIZE] = {0};
    while (fgets(buffer, SIZE, fp))
    {
        if( send(sock_fd, buffer, strlen(buffer) * sizeof(char), 0) == -1 ) {
            perror("Error in sending file");
            exit(EXIT_FAILURE);
        }
        bzero(buffer, SIZE);
    }
    
}


int main () {
    int client_fd = Create_socket(AF_INET, SOCK_STREAM, PROTOCOL);
    Set_socket_option(client_fd);
    Address* server_address = Create_Address(AF_INET, PORT);
    connect_to_address(client_fd, server_address);
    fflush(stdout);

    while (1)
    {
        FILE *fp;
        char *filename = "share.txt";
        fp = fopen(filename, "r");
        if( !fp){
            perror("\nError in Reading File.\n");
            exit(EXIT_FAILURE);
        }
        Send_File(fp, client_fd);
        printf("\nFile sent...!\n");

        close(client_fd);
        exit(EXIT_SUCCESS);
    }
    return 0;
}