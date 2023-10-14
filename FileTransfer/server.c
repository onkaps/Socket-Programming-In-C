#include "socket.h"
#define PORT 8080

//To wirte the (text files only) file contains received from client line by line to the new file.
void Write_File ( int socket_fd, char *filePath) {
    int n;
    FILE *fp;
    char buffer[SIZE];

    fp = fopen(filePath, "w");
    while (1)
    {
        n = recv(socket_fd, buffer, SIZE * sizeof(char), 0);
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

// write contains byte wise to the new file by opening th file in binary write mode
void file_receive ( int socket_fd,char *filePath ) {
    FILE* file = fopen(filePath, "wb");
    if(!file){
        perror("\nError in openin file in Binary write <'wb'> mode.\n");
        exit(EXIT_FAILURE);
    }

    char file_buffer[SIZE * SIZE];
    size_t byte_written;
    while ((byte_written = recv(socket_fd, file_buffer, sizeof(file_buffer), 0)) > 0)
    {
        fwrite(file_buffer, 1, byte_written, file);
    }
    fclose(file);
    close(socket_fd);
}


int main () {
    int server_fd = Create_socket(AF_INET, SOCK_STREAM, PROTOCOL);
    Set_socket_option(server_fd);
    Address* server_address = Create_Address(AF_INET, PORT);
    Bind_socket(server_fd, server_address);
    listen_socket(server_fd, 5);
    int client_fd = Accept_connection(server_fd, server_address);

 

    int event;
    recv(client_fd, &event, sizeof(event) , 0); 
    switch (event)
    {
    case 1:
        Write_File(client_fd, "./receive/received_text.txt");
        printf("\nText File received successfully.\n");
        break;

    case 2:
        file_receive(client_fd, "./receive/received_image.jpg");
        printf("\nImage File received successfully.\n");
        break;
    
    case 3:
        file_receive(client_fd, "./receive/received_video.mp4");
        printf("\nVideo File received successfully.\n");
        break;

    case 4:
        file_receive(client_fd, "./receive/received_PDF.pdf");
        printf("\nPDF File received successfully.\n");
        break;
        
    default:
        printf("\nError in transfer file\n");
        break;
    }

    close(server_fd);
    return 0;
}