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
    
    int sockfd;
    struct sockaddr_in servaddr;
    
    //create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    //fill in server information
    servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 
    
    //connect client to the tcp server
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0){
        cout << "Couldn't connect to server..." << endl;
        exit(0);
    }
    
    //get information from user
    string input1;
    string input2;
    cout << "Please input client name:" << endl;
    cin >> input1;
    cout << "Please input your name:" << endl;
    cin >> input2;
    string inputs = "Client " + input1 + ": " + input2;
    char message[1024];
    strcpy(message, inputs.c_str());
    
    //send message to the server
    write(sockfd, message, strlen(message));
    bzero(buff, sizeof(buff));
    
    //read message from the server
    read(sockfd, buff, sizeof(buff));
    cout << "Sent to Server (Client " << input1 << "): '" << message << "'" << endl;
    cout << "Received from Server (Client " << input1 << "): '" << buff << "'" << endl;    
    
    return 0;
}
