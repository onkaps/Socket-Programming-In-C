#include "socket.h"
#define PORT 8080

// To store size of file 
size_t file_size;

//This function generate the actual path of a file which is to be sent.By concatinating current directery + folderName + filename
char* generate_filePath(char *folder, char *fileName ){
    char *filePath = (char *)malloc( (strlen(folder) + strlen(fileName)) * sizeof(char));
    strcat(folder, fileName);
    strcpy(filePath, folder);
    return filePath;
}

//Sends the text file by reading file contains line by line.
void Send_File(FILE *fp, int socket_fd) {
    int n ;
    char buffer[SIZE] = {0};
    while (fgets(buffer, SIZE, fp))
    {
        if( send(socket_fd, buffer, strlen(buffer) * sizeof(char), 0) == -1 ) {
            perror("Error in sending file");
            exit(EXIT_FAILURE);
        }
        file_size = file_size + strlen(buffer) * sizeof(char);
        bzero(buffer, SIZE);
    }
}

//opening the text file in read mode.
void Text_file_transfer ( int socket_fd, char *filePath ) {
    FILE* fp = fopen(filePath, "r"); 
    if( !fp){
        perror("\nError in opening Text file in Read <'r'>mode.\n");
        exit(EXIT_FAILURE);
    }
    Send_File(fp, socket_fd);
    close(socket_fd);
}

//This function transfer the file which require to open file in binary read mode.
void File_transfer ( int socket_fd, char *filePath) {
    FILE* file = fopen(filePath, "rb");
    if(!file){
        perror("\nError in opening  file in Binary read <'rb'> mode.\n");
        exit(EXIT_FAILURE);
    }
    //To store data byte
    char file_buffer[SIZE * SIZE];

    //read data byte
    size_t byte_read;
    while ((byte_read = fread(file_buffer, 1, sizeof(file_buffer), file)) > 0)
    {
        if( send( socket_fd, file_buffer, byte_read, 0) < 0){
            perror("\nError in sending byte_read.\n");
            exit(EXIT_FAILURE);
        }
        file_size = file_size + byte_read;
    }
    fclose(file);
    close(socket_fd);
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
    char fileName[256];
    char *filePath;
    char folder[256] = "./share/";
    printf("\nEnter File Name (with extension) : ");
    scanf("%[^\n]%*c", fileName);
    printf("\nEnter option : ");
    scanf("%d", &event);
    filePath = generate_filePath(folder, fileName);
    printf("\nfile Path : %s\n", filePath); 

    switch (event)
    {
    case 1:
        send(client_fd, &event, sizeof(event), 0);
        Text_file_transfer(client_fd, filePath);
        printf("\nText File sent...!\n");
        printf("\nFile size = %ld bytes\n", file_size);
        break;
    
    case 2:
        send(client_fd, &event, sizeof(event), 0);
        File_transfer(client_fd, filePath);
        printf("\nImage File sent...!\n");
        printf("\nFile size = %ld bytes\n", file_size);
        break;
    
    case 3:
        send(client_fd, &event, sizeof(event), 0);
        File_transfer(client_fd, filePath);
        printf("\nVideo File sent...!\n");
        printf("\nFile size = %ld bytes\n", file_size);
        break;
    
    case 4:
        send(client_fd, &event, sizeof(event), 0);
        File_transfer(client_fd, filePath);
        printf("\nPDF File sent...!\n");
        printf("\nFile size = %ld bytes\n", file_size);
        break;
    
    case 0:
        printf("\nExit Successfully ...\n");
        exit(EXIT_SUCCESS);
        break;

    default:
        printf("\n[-]Enter Valid Option For file transfer.\n");
        break;
    }



    return 0;
}