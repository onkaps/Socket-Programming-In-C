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


void Text_file_transfer ( int sock_fd ) {
    FILE* fp = fopen("text.txt", "r"); 
    if( !fp){
        perror("\nError in opening Text file in Read <'r'>mode.\n");
        exit(EXIT_FAILURE);
    }
    Send_File(fp, sock_fd);

    close(sock_fd);
}


void Image_file_transfer ( int sock_fd ){
    FILE* picture = fopen("image.jpg", "rb");
    if( !picture){
        perror("\nError in opening Image file in Binary read <'rb'> mode.\n");
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
    
}

void Video_file_transfer ( int sock_fd ) {
    FILE* video = fopen("video.mp4", "rb");
    if(!video){
        perror("\nError in opening Video file in Binary read <'rb'> mode.\n");
        exit(EXIT_FAILURE);
    }
    char video_buffer[1024 * 1024];
    size_t byte_read;
    while ((byte_read = fread(video_buffer, 1, sizeof(video_buffer), video)) > 0)
    {
        if( send( sock_fd, video_buffer, byte_read, 0) < 0){
            perror("\nError in sending byte_read.\n");
            exit(EXIT_FAILURE);
        }
    }
    fclose(video);
    close(sock_fd);
}

void File_transfer ( int sock_fd ) {
    FILE* video = fopen("video.mp4", "rb");
    if(!video){
        perror("\nError in opening Video file in Binary read <'rb'> mode.\n");
        exit(EXIT_FAILURE);
    }
    char video_buffer[1024 * 1024];
    size_t byte_read;
    while ((byte_read = fread(video_buffer, 1, sizeof(video_buffer), video)) > 0)
    {
        if( send( sock_fd, video_buffer, byte_read, 0) < 0){
            perror("\nError in sending byte_read.\n");
            exit(EXIT_FAILURE);
        }
    }
    fclose(video);
    close(sock_fd);
}

void Add_extension ( char* fileName ) {

}

void PDF_file_transfer ( int sock_fd ) {
    FILE* pdf = fopen("PDF.pdf", "rb");
    if(!pdf){
        perror("\nError in opening PDF file in Binary read <'rb'> mode.\n");
        exit(EXIT_FAILURE);
    }
    char pdf_buffer[1024 * 1024];
    size_t byte_read;
    while ((byte_read = fread(pdf_buffer, 1, sizeof(pdf_buffer), pdf)) > 0)
    {
        if( send( sock_fd, pdf_buffer, byte_read, 0) < 0){
            perror("\nError in sending byte_read.\n");
            exit(EXIT_FAILURE);
        }
    }
    fclose(pdf);
    close(sock_fd);
}

int main () {
    int client_fd = Create_socket(AF_INET, SOCK_STREAM, PROTOCOL);
    Set_socket_option(client_fd);
    Address* server_address = Create_Address(AF_INET, PORT);
    connect_to_address(client_fd, server_address);
    fflush(stdout);

    printf("\n!--------------- FILE TRANSFER ---------------!\n\n\n");
    printf("Select which file you want to transfer : \n");
    printf("1.Transfer Text file.\n");
    printf("2.Transfer Image file. \n");
    printf("3.Transfer Video file. \n");
    printf("4.Transfer PDF file. \n");
    printf("(Enter '0' for exit.)\n");
 
    int event;
    printf("\nEnter option : ");
    scanf("%d", &event);
    // Text_file_transfer( client_fd, argv[1]);
    // Image_file_transfer(client_fd, argv[1]);
    // Video_file_transfer(client_fd, argv[1]);
    switch (event)
    {
    case 1:
        send(client_fd, &event, sizeof(event), 0);
        Text_file_transfer(client_fd);
        printf("\nText File sent...!\n");
        break;
    
    case 2:
        send(client_fd, &event, sizeof(event), 0);
        Image_file_transfer(client_fd);
        printf("\nImage File sent...!\n");
        break;
    
    case 3:
        send(client_fd, &event, sizeof(event), 0);
        Video_file_transfer(client_fd);
        printf("\nVideo File sent...!\n");
        break;
    
    case 4:
        send(client_fd, &event, sizeof(event), 0);
        PDF_file_transfer(client_fd);
        printf("\nPDF File sent...!\n");
        break;
    
    case 0:
        printf("\nExit Successfully ...\n");
        exit(EXIT_SUCCESS);
        break;

    default:
        printf("\n[-]Enter Valid Option For file transfer.\n");
        break;
    }

    exit(EXIT_SUCCESS);

    return 0;
}