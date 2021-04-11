#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

int main()
{ int listenfd, connfd;
  struct sockaddr_in srvinfo, cliinfo;
  int len;
  char buff[30];
  
  //Creation of Socket
  //int socket(int domain, int type, int protocol);

  if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  		perror("Socket Creation Failed\n");
  else
       printf("\n Socket Created Successfully\n");
       
  // Associating sockaddr_in info
srvinfo.sin_family = AF_INET;
   srvinfo.sin_port =htons(5000);
   srvinfo.sin_addr.s_addr= inet_addr("192.168.43.197"); 
   
 // Registering socket at IP/Internet Layer 
  
   if(bind(listenfd, (struct sockaddr *)&srvinfo, sizeof(srvinfo))==-1)  
   perror("Binding Failed\n");
   else
       printf("\n Binding is Successful\n");
       
   listen(listenfd,4);
   len = sizeof(srvinfo);
//int accept(int s, struct sockaddr *addr, socklen_t *addrlen); 
   connfd = accept(listenfd, (struct sockaddr *)&cliinfo, &len);
   
   strcpy(buff, "Hello Client -msg from Server");
   
   write(connfd,buff,sizeof(buff));
   strcpy(buff,"");
   read(connfd,buff,sizeof(buff));
   printf("%s\n", buff);
       
   close(listenfd);
   //close(connfd);
}