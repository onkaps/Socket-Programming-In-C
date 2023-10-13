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

void Video_file_transfer ( int sock_fd ) {
    FILE* video = fopen("received_video.mp4", "wb");
    if(!video){
        perror("\nError in opening Video file in Binary write <'wb'> mode.\n");
        exit(EXIT_FAILURE);
    }

    char video_buffer[1024 * 1024];
    size_t byte_written;
    while ((byte_written = recv(sock_fd, video_buffer, sizeof(video_buffer), 0)) > 0)
    {
        fwrite(video_buffer, 1, byte_written, video);
    }
    fclose(video);
    close(sock_fd);
}

void PDF_file_transfer ( int sock_fd ) {
    FILE* pdf = fopen("received_PDF.pdf", "wb");
    if(!pdf){
        perror("\nError in opening PDF file in Binary write <'wb'> mode.\n");
        exit(EXIT_FAILURE);
    }

    char pdf_buffer[1024 * 1024];
    size_t byte_written;
    while ((byte_written = recv(sock_fd, pdf_buffer, sizeof(pdf_buffer), 0)) > 0)
    {
        fwrite(pdf_buffer, 1, byte_written, pdf);
    }
    fclose(pdf);
    close(sock_fd);
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
    printf("Check event no. : %d ", event);

    // Text_file_transfer(client_fd);
    // Image_file_transfer(client_fd);
    // Video_file_transfer(client_fd);
    switch (event)
    {
    case 1:
        Text_file_transfer(client_fd);
        printf("\nText File received successfully.\n");
        break;

    case 2:
        Image_file_transfer(client_fd);
        printf("\nImage File received successfully.\n");
        break;
    
    case 3:
        Video_file_transfer(client_fd);
        printf("\nVideo File received successfully.\n");
        break;

    case 4:
        PDF_file_transfer(client_fd);
        printf("\nPDF File received successfully.\n");
        break;
        
    default:
        break;
    }



    close(server_fd);
    exit(EXIT_SUCCESS);
    return 0;
}