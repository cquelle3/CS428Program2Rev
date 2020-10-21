#include <iostream>
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12001

using namespace std;

int main(){
    
    char buff[1024];
    int sockfd, connfd1, connfd2;
    socklen_t len;
    struct sockaddr_in servaddr, cliaddr;
    
    //create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //sets the option to enabling reusing sockets more than once just in case socket is not free at runtime
    int enable = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0){
        cout << "setsockopt error..." << endl;
        exit(0);
    }
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(enable)) < 0){
        cout << "setsockopt error..." << endl;
        exit(0);
    }
    
    //Fill server info
    servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 
    
    //Bind socket to server address
    if((::bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0){
        cout << "Socket bind failed..." << endl;
    }
    
    //Server listens for connections
    if((listen(sockfd, 5)) != 0){
        cout << "Listen failed..." << endl;
        exit(0);
    }
    
    len = sizeof(cliaddr);
    
    //Accept first connection from either client x or y
    connfd1 = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
    if(connfd1 < 0){
        cout << "Server could not accept connection..." << endl;
    }
    //Accept second connection from either client x or y
    connfd2 = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
    if(connfd2 < 0){
        cout << "Server could not accept connection..." << endl;
    }
    
    //read in first message received from first client connected
    bzero(buff, sizeof(buff));
    read(connfd1, buff, sizeof(buff));
    string msg1 = buff;
    
    //read in second message received from second client connected 
    bzero(buff, sizeof(buff));
    read(connfd2, buff, sizeof(buff));
    string msg2 = buff;
    
    cout << "From Client 1: '" << msg1 << "'" << endl;
    cout << "From Client 2: '" << msg2 << "'" << endl;
    
    //create response to send to client
    string str_message = msg1 + " received before " + msg2;
    char message[1024];
    strcpy(message, str_message.c_str());
    
    //send response to both clients
    write(connfd1, message, strlen(message));
    write(connfd2, message, strlen(message));
    
    cout << "Server Sending Acknowledgments: '" << message << "'" << endl;
    //cout << "Sent acknowledgment to both clients" << endl;
    
    return 0;
}
