#define SUPPRESS_SOCKET_LOGS 1
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


void Text_file_transfer ( int sock_fd , char* file_send ) {
    FILE* fp = fopen(file_send, "r"); 
    if( !fp){
        perror("\nError in opening Text file in Read <'r'>mode.\n");
        exit(EXIT_FAILURE);
    }
    Send_File(fp, sock_fd);
    printf("\nFile sent...!\n");

    close(sock_fd);
    exit(EXIT_SUCCESS);
}

void Image_file_transfer ( int sock_fd , char* file_send){
    FILE* picture = fopen(file_send, "rb");
    if( !picture){
        perror("\nError in opening Image file in Binary read <'rb'>mode.\n");
        exit(EXIT_FAILURE);
    }

    char image_buffer[1024 * 1024];
    size_t byte_read;
    while ((byte_read = fread( image_buffer, 1, sizeof(image_buffer), picture)) > 0)
    {
        if( send(sock_fd, image_buffer, byte_read, 0) < 0) { 
            perror("\nError in sending byte_read.\n");
            exit(EXIT_FAILURE);
        }
    }
    fclose(picture);
    close(sock_fd);
    exit(EXIT_SUCCESS);
    
}



int main ( int argc, char *argv[] ) {
    int client_fd = Create_socket(AF_INET, SOCK_STREAM, PROTOCOL);
    Set_socket_option(client_fd);
    Address* server_address = Create_Address(AF_INET, PORT);
    connect_to_address(client_fd, server_address);
    fflush(stdout);

    if(argc != 3){
        printf("Usage: %s <file_to_be_send> <file_to_be_recevied>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Text_file_transfer( client_fd, argv[1]);

    Image_file_transfer(client_fd, argv[1]);


   
    

  
    
    return 0;
}