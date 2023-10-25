#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#define PORT 8080


    char data[50];
    char transmission_data[50];
    char key[50];
    char crc[50];
    char data1[50];
    char message[50];
    size_t   i,j;
    size_t data_length;
    size_t N, one = 1;

// char XOR (char a, char b) {
//     return a == b ? '0' : '1';
// }

void XOR () {
    for( j = 1; j < N; j++ ){
        crc[j] = (crc[j] == key[j]) ? '0' : '1';
    }
}

void sender () {

    for( i = 0; i < N; i++){
        crc[i] = data[i];
    }

    do{
        if( crc[0] == '1')
            XOR();
        
        for( j = 0; j < N - 1; j++){
            crc[j] = crc[j+1];
        }
        crc[j] = data1[i++];
    }while( i <= data_length + N - one);

}

void Error () {
    int x;
    printf("\nEnter bit number which is need to be alter : ");
    scanf("%d", &x);
    
}


int main () {

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    int enable = 1;
        if(setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(enable)) <  0){
            perror("\noption setting fails.\n");
            exit(EXIT_FAILURE);
        }

    struct sockaddr_in serveraddr;
    socklen_t serveraddr_length = sizeof(serveraddr);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    if( connect(client_socket, (struct sockaddr *)&serveraddr, serveraddr_length) < 0){
        perror("\nconnection fails\n");
        exit(EXIT_FAILURE);
    }



    while (1)
    {
        printf("\nEnter data(binary) string : ");
        scanf("%[^\n]%*c", data);
        data_length = strlen(data);
        send(client_socket, data, strlen(data)*sizeof(char), 0);

        printf("\nEnter key : ");
        scanf("%[^\n]%*c", key);
        N = strlen(key);
        send( client_socket, key, strlen(key)*sizeof(char), 0);

        strcpy( data1, data);
        printf("\ndata : %s\n", data);

        for ( i = 0; i < N - 1; i++)
            strcat(data1, "0");
        
        printf("\ndata1 : %s\nlength of data1 : %ld", data1, strlen(data1));
        sender();

        printf("\ncrc : %s\n", crc);
        char buffer[1024];
        strcpy(buffer, data);
        strcat(buffer, crc);
        strcpy(transmission_data, buffer);
        printf("\ntransmission data : %s\n", transmission_data);
        
        send(client_socket, transmission_data, strlen(transmission_data)*sizeof(char), 0);

        recv(client_socket, message, 50 * sizeof(char), 0);
        printf("\n\nServer's Reply : %s\n\n", message );
        fflush(stdout);

    exit(EXIT_SUCCESS);
    }
    
    

    return 0;
}