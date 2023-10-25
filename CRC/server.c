#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#define PORT 8080

    char data_buffer[50];
    char tdata_buffer[50];
    char key[50];
    char check[50];
    char crc[50];
    char message[50];

    size_t i, j, tdata_length, N, one = 1;


void XOR () {
    for( j = 1; j < N; j++){
        crc[j] = (crc[j] == key[j]) ? '0': '1';
    }
}

void receiver () {
    for( i = 0; i < N; i++){
        crc[i] = tdata_buffer[i];
    }

    do{
        if(crc[0] == '1'){
            XOR();
        }
        for( j = 0; j < N - 1; j++){
            crc[j] = crc[j+1];
        }
        crc[j] =  tdata_buffer[i++];
    }while( i <= tdata_length );
}

void error () {

    

    if (!strcmp( check, crc))
        strcpy(message, "No ERROR in Transmission.");
    else
        strcpy(message, "Error in transmission.");
}


int main () {

    int server_socket = socket( AF_INET, SOCK_STREAM, 0);
    int enable = 1;
        if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(enable)) < 0){
            perror("\noption setting fails.\n");
            exit(EXIT_FAILURE);
        }

    struct sockaddr_in serveraddr;
    socklen_t serveraddr_length = sizeof(serveraddr);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;


    if(server_socket < 0) {
        perror("\nsocket creation fails.\n");
        exit(EXIT_FAILURE);
    }

    if( bind(server_socket, (struct sockaddr *)&serveraddr, serveraddr_length) < 0) {
        perror("\nBinding Fails.\n");
        exit(EXIT_FAILURE);
    }

    if( listen( server_socket, 5) < 0){
        perror("\nListening Fails.\n");
        exit(EXIT_FAILURE);
    }

    int client_socket = accept( server_socket, (struct sockaddr *)&serveraddr, &serveraddr_length );
    if( client_socket < 0) {
        perror("\nAcceptance Fails.\n");
        exit(EXIT_FAILURE);
    }


    while (1)
    {
        recv(client_socket, data_buffer, 50 *sizeof(char), 0);

        recv(client_socket, key, 50 * sizeof(char), 0);
        printf("\nkey recevied : %s\n", key);
        fflush(stdout);
        N = strlen(key);

        check[0] = '\0';
        for( i = 0; i < N - 1; i++){
            strcat(check, "0");
        }
        check[N] = '\0';
        printf("\ncheck : %s\n", check);

        recv(client_socket, tdata_buffer, 50 * sizeof(char), 0);
        printf("\ntransmitted data : %s\n", tdata_buffer);
        fflush(stdout);
        tdata_length = strlen(tdata_buffer);

        receiver();
        printf("\nCRC : %s\n", crc);

        error();
        send(client_socket, message, strlen(message)*sizeof(char), 0);

        exit(EXIT_SUCCESS);
    }
    


    return 0;
}