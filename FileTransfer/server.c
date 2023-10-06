#define SUPPRESS_SOCKET_LOGS 1
#include "socket.h"
#define PORT 8080


void Write_File ( int sock_fd ) {
    int n;
    FILE *fp;
    char *filename = "recv.txt";
    char buffer[SIZE];

    fp = fopen(filename, "w");
    while (1)
    {
        n = recv(sock_fd, buffer, SIZE * sizeof(char), 0);
        if( n <= 0){
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;    
}


int main () {
    int server_fd = Create_socket(AF_INET, SOCK_STREAM, PROTOCOL);
    Set_socket_option(server_fd);
    Address* server_address = Create_Address(AF_INET, PORT);
    Bind_socket(server_fd, server_address);
    listen_socket(server_fd, 5);
    int client_fd = Accept_connection(server_fd, server_address);

    while (1)
    {
        // Write_File(client_fd);
        // exit(EXIT_SUCCESS);

        FILE* image_file = fopen("recv_image.jpg", "wb");
        if( image_file == NULL){
            perror("\nError in Opening image file in binaey write mode.\n");
            exit(EXIT_FAILURE);
        }
        char buffer[1024];
        int byte_recv;
        while (byte_recv = recv(client_fd, buffer, sizeof(buffer), 0) > 0)
        {
            fwrite(buffer, 1, byte_recv, image_file);
        }

        fclose(image_file);
        close(client_fd);
        printf("\nFile recevied \n");
        exit(EXIT_SUCCESS);
        
    }
    
        
    return 0;
}