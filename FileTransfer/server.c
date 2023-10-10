#define SUPPRESS_SOCKET_LOGS 1
#include "socket.h"
#define PORT 8080


void Write_File ( int sock_fd ) {
    int n;
    FILE *fp;
    char buffer[SIZE];

    fp = fopen("received_text.txt", "w");
    while (1)
    {
        n = recv(sock_fd, buffer, SIZE * sizeof(char), 0);
        if( n <= 0){
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        fflush(stdout);
        bzero(buffer, SIZE);
    }
    return;    
}

void Text_file_transfer ( int sock_fd )  {
        Write_File(sock_fd );
        exit(EXIT_SUCCESS);
}

void Image_file_transfer ( int sock_fd ) {
    FILE* picture = fopen("received_image.jpg", "wb");
    if(!picture){
        perror("\nError in opening Image file in Binary write <'wb'> mode ");
        exit(EXIT_FAILURE);
    }

    char image_buffer[1024 * 1024];
    size_t byte_written;
    while ((byte_written = recv(sock_fd, image_buffer, sizeof(image_buffer), 0)) > 0)
    {
        fwrite(image_buffer, 1, byte_written, picture);
    }
    fclose(picture);
    close(sock_fd);
    
}

int main () {
    int server_fd = Create_socket(AF_INET, SOCK_STREAM, PROTOCOL);
    Set_socket_option(server_fd);
    Address* server_address = Create_Address(AF_INET, PORT);
    Bind_socket(server_fd, server_address);
    listen_socket(server_fd, 5);
    int client_fd = Accept_connection(server_fd, server_address);

    // Text_file_transfer(client_fd);
    Image_file_transfer(client_fd);
    close(server_fd);


    return 0;
}