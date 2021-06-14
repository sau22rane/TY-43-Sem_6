#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<iostream>
using namespace std;

int main(int argc, char **argv){
    int clifd, n;
    socklen_t len;
    struct sockaddr_in servaddr;
    char buff[1024];

    if((clifd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("Socket Creation Failed\n");
    else
        cout<<"Socket Created Successfully\n";
        

    servaddr.sin_family = AF_INET;
    servaddr.sin_port =htons(atoi(argv[2]));
    servaddr.sin_addr.s_addr= inet_addr(argv[1]); 
    while(1){
        bzero(buff, sizeof(buff));
        cout<<"\nClient: ";
        scanf("%[^\n]", buff);
        
        sendto(clifd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
        if(strcmp(buff, "exit") == 0){
            cout<<"\nClosing socket\n";
            break;
        }
        bzero(buff, sizeof(buff));
        n = recvfrom(clifd, (char *)buff, 1024, MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
        buff[n] = '\0'; 
        cout<<"Server: "<<buff<<endl; 
        if(strcmp(buff, "exit") == 0){
            cout<<"\nClient is leaving\nClosing socket\n";
            break;
        }
        scanf("%*c");
    }
    close(clifd);
}