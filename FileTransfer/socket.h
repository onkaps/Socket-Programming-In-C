#define SUPPRESS_SOCKET_LOGS 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<strings.h>

#define SIZE 1024


#define PROTOCOL 0

typedef struct sockaddr_in Address;

void Check ( int condition, char* message) {
    if(condition){
        printf("\n%s\n", message);
    }
}

int Create_socket(int family, int type, int protocol){
    int socket_fd = socket( family, type, protocol);
    Check( socket_fd < 0, "[+]Error in Socket Creation.");
    #ifndef SUPPRESS_SOCKET_LOGS
    printf("\nSocket created Successfully.\n");
    #endif
    return socket_fd;
}

Address* Create_Address ( int family, int PORT ) {
    Address* addr = (Address *)malloc(sizeof(Address));
    addr->sin_family = family;
    addr->sin_port = htons(PORT);
    addr->sin_addr.s_addr = INADDR_ANY;
    return addr;
}

void Bind_socket ( int socket_fd, Address* addr ) {
    socklen_t addr_length = sizeof(*addr);
    int status = bind(socket_fd, (struct sockaddr *)addr, addr_length);
    Check( status < 0, "[+]Error in Socket binding.");
    #ifndef SUPPRESS_SOCKET_LOGS
    printf("\nBinding Successfully.\n");
    #endif
}

void Set_socket_option( int socket_fd ) {
    int enable = 1;
    int status;
    status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(enable));
    Check( status < 0, "[+]Error in option setting.");
    #ifndef SUPPRESS_SOCKET_LOGS
    printf("\nsocket option set.\n");
    #endif
}

void listen_socket ( int socket_fd, int max_connection ) {
    int status;
    status = listen(socket_fd, max_connection);
    Check( status < 0, "[+]Error in Listening on Socket.");
    #ifndef SUPPRESS_SOCKET_LOGS
    printf("\nListening on socket Successfully\n");
    #endif
}

int  Accept_connection ( int socket_fd, Address* addr ) {
    int connet_fd;
    socklen_t addr_length = sizeof(*addr);
    connet_fd = accept(socket_fd, (struct sockaddr *)addr, &addr_length);
    Check( connet_fd < 0, "[+]Error in accepting connection.");
    #ifndef SUPPRESS_SOCKET_LOGS
    printf("\nConnection accepted\n");
    #endif
    return connet_fd;
}

void connect_to_address (int socket_fd, Address* addr) {
	socklen_t addr_length = sizeof(*addr);
    int status = connect(socket_fd, (struct sockaddr*)addr, addr_length);
	Check(status < 0, "[+]Error in connection.");
	#ifndef SUPPRESS_SOCKET_LOGS 
	printf("\nConnection established.\n");
	#endif
}

void send_on_socket ( int socket_fd, char *message ) {
    int status;
    char buffer[SIZE];
    strcpy(buffer, message);
    status = send(socket_fd, buffer, sizeof(buffer), 0);
    Check(status < 0, "[+]Error in sending message.");
	#ifndef SUPPRESS_SOCKET_LOGS 
	printf("\nMessage sent.\n");
	#endif
}

char* receive_from_socket ( int socket_fd ) {
    char* buffer = (char *)malloc(sizeof(char) * SIZE );
    size_t length = SIZE;
    int bytes_recv; 
    while ( (bytes_recv = recv(socket_fd, buffer, length, 0 )) != 0 )
    {
        if(bytes_recv < 0){
            printf("Error in reciving.");
            exit(EXIT_FAILURE);
        }
        buffer += bytes_recv;
        length -= bytes_recv;
        if(length == 0) break;
    }
    Check( bytes_recv == 0, "Connection closed befor data to be recevied.");
    
}