#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

int main(){ 
    int listenfd, connfd;
    struct sockaddr_in srvinfo, cliinfo;
    int len;
    char buff[30];

    //Creation of Socket
    //int socket(int domain, int type, int protocol);

    if((listenfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("Socket Creation Failed\n");
    else
        printf("Socket Created Successfully");
        
    // Associating sockaddr_in info
    srvinfo.sin_family = AF_INET;
    srvinfo.sin_port =htons(5000);
    srvinfo.sin_addr.s_addr= inet_addr("10.0.2.4"); 
    
    // Registering socket at IP/Internet Layer 

    if(bind(listenfd, (struct sockaddr *)&srvinfo, sizeof(srvinfo))==-1)  
    perror("Binding Failed\n");
    else
        printf("\nBinding is Successful\n");
        
    len = sizeof(srvinfo);




    while(1){
        bzero(buff, sizeof(buff));
        int n = recvfrom(listenfd, (char *)buff, 30, MSG_WAITALL, ( struct sockaddr *) &cliinfo, &len); 
        
        printf("\nClient: %s\n", buff); 

        if(strcmp(buff, "exit") == 0){
                printf("\nClient is leaving\nClosing socket\n");
                break;
            }
        bzero(buff, sizeof(buff));
        printf("Server: ");
        scanf("%[^\n]", buff);
        sendto(listenfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliinfo, len);
        scanf("%*c");
        if(strcmp(buff, "exit") == 0){
            printf("\nClosing socket\n");
            break;
        }
    }
    close(listenfd);
}