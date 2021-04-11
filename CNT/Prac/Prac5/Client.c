#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

int main(){
    int clifd, len, n;
    struct sockaddr_in servaddr;
    char buff[30];

    if((clifd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("Socket Creation Failed\n");
    else
        printf("Socket Created Successfully\n");
        

    servaddr.sin_family = AF_INET;
    servaddr.sin_port =htons(5000);
    servaddr.sin_addr.s_addr= inet_addr("10.0.2.4"); 
    while(1){
        bzero(buff, sizeof(buff));
        printf("\nClient: ");
        scanf("%[^\n]", buff);
        
        sendto(clifd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
        if(strcmp(buff, "exit") == 0){
            printf("\nClosing socket\n");
            break;
        }
        bzero(buff, sizeof(buff));
        n = recvfrom(clifd, (char *)buff, 30, MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
        buff[n] = '\0'; 
        printf("Server: %s\n", buff); 
        if(strcmp(buff, "exit") == 0){
            printf("\nServer is leaving\nClosing socket\n");
            break;
        }
        scanf("%*c");
    }
    close(clifd);
}