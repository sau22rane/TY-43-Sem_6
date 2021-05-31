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
int MOD = 0;

class Diffie{
    private:
    int listenfd, connfd;
    struct sockaddr_in srvinfo, cliinfo;
    socklen_t len;
    char buff[30];

    int G = 0;
    int key;
    int exp(int num, int index, int mod);
    
    public:
    Diffie();
    ~Diffie();
    void Exchange_key();
};


int main(){
   Diffie diffie;
}


Diffie::Diffie(){

    if((listenfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("Socket Creation Failed\n");
    else
        printf("Socket Created Successfully");

    srvinfo.sin_family = AF_INET;
    srvinfo.sin_port =htons(5000);
    srvinfo.sin_addr.s_addr= inet_addr("127.0.0.1");
    
    if(bind(listenfd, (struct sockaddr *)&srvinfo, sizeof(srvinfo))==-1) 
    perror("Binding Failed\n");
    else
        printf("\nBinding is Successful\n");
        
    len = sizeof(srvinfo);

    Exchange_key();
}

void Diffie::Exchange_key(){
    bzero(buff, sizeof(buff));
    int n = recvfrom(listenfd, (char *)buff, 30, MSG_WAITALL, ( struct sockaddr *) &cliinfo, &len);

    G|=((int)buff[0]-48)<<8*3;
    G|=((int)buff[1]-48)<<8*2;
    G|=((int)buff[2]-48)<<8*1;
    G|=((int)buff[3]-48);

    MOD|=((int)buff[4]-48)<<8*3;
    MOD|=((int)buff[5]-48)<<8*2;
    MOD|=((int)buff[6]-48)<<8*1;
    MOD|=((int)buff[7]-48);


    cout<<"G = "<<G<<", MOD = "<<MOD<<endl<<endl;

    cout<<"Private key(integer): ";
    cin>>this->key;
    int res = exp(G,key, MOD);
    int Pa = 0;

    buff[0] = '0'+((res & 0xFF000000) >> 8*3);
    buff[1] = '0'+((res & 0x00FF0000) >> 8*2);
    buff[2] = '0'+((res & 0x0000FF00) >> 8*1);
    buff[3] = '0'+((res & 0x000000FF));
    

    cout<<"Sending Pb = "<<res<<endl;
    sendto(listenfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliinfo, len);


    n = recvfrom(listenfd, (char *)buff, 30, MSG_WAITALL, (struct sockaddr *) &cliinfo, &len);
    Pa|=((int)buff[0]-48)<<8*3;
    Pa|=((int)buff[1]-48)<<8*2;
    Pa|=((int)buff[2]-48)<<8*1;
    Pa|=((int)buff[3]-48);

    cout<<"\nPa = "<<Pa<<"\n";
    int shared_sec = exp(Pa,key, MOD);
    cout<<endl;
    cout<<"Shared Secret: "<<shared_sec<<"\n";
}

Diffie::~Diffie(){
    close(listenfd);   
}


int Diffie::exp(int num, int index, int mod){
    int y=1, u=num % mod;
    while (index >0){
        if ((index % 2)==1)
            y=(y*u) % mod;
        if (index > 0)
            index=(int)(index / 2);
        u=(u*u) % mod;
    }
    return y%mod;
}