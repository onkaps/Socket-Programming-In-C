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

int Send_Image ( int sock_fd ) {
    FILE *picture;
    int size, read_size;
    char send_buffer[10240], verify;

    picture = fopen("image.png", "r");
    if(picture == NULL) {
        perror("\nError in picture opening.\n");
    }

    fseek(picture, 0, SEEK_END);
    size = ftell(picture);
    fseek( picture, 0, SEEK_SET);

    // Sending Picture size. 
    printf("\nSending Image size to client..\n");
    write(sock_fd, send_buffer, size);


    // if( read_size = read(sock_fd, send_buffer, size) == )

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
        Write_File(client_fd);

        exit(EXIT_SUCCESS);
    }
        
    return 0;
}