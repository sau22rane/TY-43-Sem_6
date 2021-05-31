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
    int clifd, n;
    socklen_t len;
    struct sockaddr_in servaddr;
    char buff[30];
    int key;
    int G;

    int inv(int num, int mod);
    int exp(int num, int index, int mod);

    void Exchange_key();
    public:
    Diffie();
    ~Diffie();
};


int main(){
   Diffie diffie;
}

Diffie::Diffie(){

    cout<<"G = ";
    cin>>G;
    cout<<"MOD = ";
    cin>>MOD;
    cout<<endl<<endl;
    
    if((clifd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("Socket Creation Failed\n");
    else
        printf("Socket Created Successfully\n");
        
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port =htons(5000);
    servaddr.sin_addr.s_addr= inet_addr("127.0.0.1");
    Exchange_key();
}

void Diffie::Exchange_key(){
    buff[0] = '0'+((G & 0xFF000000) >> 8*3);
    buff[1] = '0'+((G & 0x00FF0000) >> 8*2);
    buff[2] = '0'+((G & 0x0000FF00) >> 8*1);
    buff[3] = '0'+((G & 0x000000FF));

    buff[4] = '0'+((MOD & 0xFF000000) >> 8*3);
    buff[5] = '0'+((MOD & 0x00FF0000) >> 8*2);
    buff[6] = '0'+((MOD & 0x0000FF00) >> 8*1);
    buff[7] = '0'+((MOD & 0x000000FF));
    
    // for(int i= 0; i<12; i++){
    //     cout<<((int)buff[i] - 48);
    // }

    
    
    sendto(clifd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    cout<<"G and MOD sent...!\n\n";
    cout<<"Private key(integer): ";
    cin>>this->key;
    int res = exp(G,key, MOD);

    int Pb = 0;
    n = recvfrom(clifd, (char *)buff, 30, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    
    
    Pb|=((int)buff[0]-48)<<8*3;
    Pb|=((int)buff[1]-48)<<8*2;
    Pb|=((int)buff[2]-48)<<8*1;
    Pb|=((int)buff[3]-48);

    cout<<"\nPb = "<<Pb<<"\n";

    buff[0] = '0'+((res & 0xFF000000) >> 8*3);
    buff[1] = '0'+((res & 0x00FF0000) >> 8*2);
    buff[2] = '0'+((res & 0x0000FF00) >> 8*1);
    buff[3] = '0'+((res & 0x000000FF));

    cout<<"Sending Pa = "<<res<<endl;
    sendto(clifd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    
    int shared_sec = exp(Pb,key, MOD);
    cout<<endl;
    cout<<"Shared Secret: "<<shared_sec<<"\n";
    
}


Diffie::~Diffie(){
    close(clifd);   
}

int Diffie::inv(int num, int mod){
    if(num<0)num+=MOD;
    int a = num, m = mod, m0 = m;
    int y = 0, x = 1;
 
    if (m == 1)
        return 0;
 
    while (a > 1) {
        int q = a / m;
        int t = m;
        m = a % m, a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0)
        x += m0;
    return x;
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

