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

    int listenfd, connfd;
    struct sockaddr_in srvinfo, cliinfo;
    socklen_t len;
    byte buff[1024];

    //Creation of Socket
    //int socket(int domain, int type, int protocol);

    if((listenfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("Socket Creation Failed\n");
    else
        cout<<"Socket Created Successfully";
        
    // Associating sockaddr_in info
    srvinfo.sin_family = AF_INET;
    srvinfo.sin_port =htons(atoi(argv[2]));
    srvinfo.sin_addr.s_addr= inet_addr(argv[1]); 

    if(bind(listenfd, (struct sockaddr *)&srvinfo, sizeof(srvinfo))==-1)  
        perror("Binding Failed\n");
    else
        cout<<"\nBinding is Successful\n";
        
    len = sizeof(srvinfo);
    
    for(int i = 0; i<1; i++){
        // Registering socket at IP/Internet Layer 

        while(1){
            bzero(buff, sizeof(buff));
            int n = recvfrom(listenfd, (char *)buff, sizeof(buff), MSG_WAITALL, ( struct sockaddr *) &cliinfo, &len); 
            
            cout<<"\nClient: "<< buff << endl; 

            if(strcmp((const char *) buff, "exit") == 0){
                    cout<<"\nClient is leaving\nClosing socket\n";
                    break;
                }
            bzero(buff, sizeof(buff));
            cout<<"Server: ";
            scanf("%[^\n]", (char *) buff);
            sendto(listenfd, (const char *)buff, sizeof(buff), MSG_CONFIRM, (const struct sockaddr *) &cliinfo, len);
            scanf("%*c");
            if(strcmp((const char *) buff, "exit") == 0){
                cout<<"\nClosing socket\n";
                break;
            }
        }
    }
    close(listenfd);
}