#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

int main()
{ int clifd;
  struct sockaddr_in clientinfo;
  char rbuff[30];

  if((clifd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  		perror("Socket Creation Failed\n");
  else
       printf("\n Socket Created Successfully\n");
       
  
   clientinfo.sin_family = AF_INET;
   clientinfo.sin_port =htons(5000);
   clientinfo.sin_addr.s_addr= inet_addr("192.168.43.197"); 
if(bind(clifd, (struct sockaddr *)&clientinfo, sizeof(clientinfo))==-1)  
   perror("Binding Failed\n");
   else
       printf("\n Binding is Successful\n");
       
   if(connect(clifd, (struct sockaddr *)&clientinfo, sizeof(clientinfo))==-1)
        perror("Connect Failed\n");
   else
       printf("\n Connect Successful\n");
read(clifd,rbuff, sizeof(rbuff));
   printf("%s\n", rbuff);    
   strcpy(rbuff,"");
   strcpy(rbuff,"Thank You Server.");
   write(clifd,rbuff,sizeof(rbuff));
         
  close(clifd);
}