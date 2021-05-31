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

typedef struct Point{
    int x, y;
    Point(int x, int y){
        this->x = x;
        this->y = y;
    }
    Point(){
        this->x = 0;
        this->y = 0;
    };
}Point;

typedef struct Ciphertext{
    Point kB, msg;
}Ciphertext;

typedef struct Generator : Point{
    int A;
    int B;
    int MOD;
    Point G;
    Generator()  : Point(){
        this->MOD = 0;
        this->A = 0;
        this->B = 0;
    }

}Generator;


class ECC{
    private:
    int listenfd, connfd;
    struct sockaddr_in srvinfo, cliinfo;
    socklen_t len;
    char buff[30];
    Point Pa = Point(0,0);
    Generator generator;
    int key;

    int inv(int num, int mod);
    Point add(Point P, Point Q);
    Point Sage_add(Point P, int index);
    
    public:
    ECC();
    ~ECC();
    Ciphertext Encrypt(Point Pm, int k);
    void Exchange_key();
};


int main(){
   ECC ecc;
}


ECC::ECC(){

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

void ECC::Exchange_key(){
    memset(buff, 0x00, sizeof(buff));
    int n = recvfrom(listenfd, (char *)buff, 30, MSG_WAITALL, ( struct sockaddr *) &cliinfo, &len);
    memcpy(&generator, buff, sizeof(generator));

    cout<<"A = "<<generator.A<<", B = "<<generator.B<<"\nG = ("<<generator.G.x<<", "<<generator.G.y<<"), MOD = "<<generator.MOD<<endl<<endl;

    cout<<"Private key(integer): ";
    cin>>this->key;

    Point res = Sage_add(generator.G,key);

    memset(buff, 0x00, sizeof(res));
    memcpy(buff, &res, sizeof(res));
    sendto(listenfd, (const char *)buff, sizeof(buff), MSG_CONFIRM, (const struct sockaddr *) &cliinfo, len);

    n = recvfrom(listenfd, (char *)buff, 30, MSG_WAITALL, (struct sockaddr *) &cliinfo, &len);
    memcpy(&Pa, buff, sizeof(Pa));

    cout<<"\nPa(Public Key of Client) = ("<<Pa.x<<", "<<Pa.y<<")\n";

    Point Pm;
    cout<<"Enter Point encoded message\nx = ";
    cin>>Pm.x;
    cout<<"y = ";
    cin>>Pm.y;
    cout<<"\nK (between 1 to "<<generator.MOD-1<<") = ";
    int k;
    cin>>k;
    
    Ciphertext cipher = Encrypt(Pm, k);
    cout<<"\nPm = ("<<Pm.x<<", "<<Pm.y<<")\n";
    cout<<"Cipher = "<<"["<<"("<<cipher.kB.x<<", "<<cipher.kB.y<<"), ("<<cipher.msg.x<<", "<<cipher.msg.y<<")"<<"]\n\n";

    memset(buff, 0x00, sizeof(cipher));
    memcpy(buff, &cipher, sizeof(cipher));


    sendto(listenfd, (const char *)buff, sizeof(buff), MSG_CONFIRM, (const struct sockaddr *) &cliinfo, len);
    cout<<"Encrypted message sent\n";
    
}

ECC::~ECC(){
    close(listenfd);   
}

int ECC::inv(int num, int mod){
    if(num<0)num+=generator.MOD;
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

Point ECC::add(Point P, Point Q){
    int m;
    if(P.x == Q.x && P.y == Q.y){
        m = ((3*P.x*P.x + generator.A) * inv(2*P.y, generator.MOD))%generator.MOD;
    }
    else{
        m = ((P.y-Q.y) * inv(P.x-Q.x, generator.MOD))%generator.MOD;
    }
    if(m<0) m+=generator.MOD;
    Point R;
    R.x = (m*m - P.x - Q.x)%generator.MOD;
    R.y = (m*(P.x - R.x) - P.y)%generator.MOD;
    if(R.x<0) R.x+=generator.MOD;
    if(R.y<0) R.y+=generator.MOD;
    return R;
}

Point ECC::Sage_add(Point P, int index){
    Point y=P, u=P;
    index--;
    while (index >0){
        if ((index % 2)==1)
            y= add(y,u);
        if (index > 0)
            index=(int)(index / 2);
        u=add(u,u);
    }
    return y;
}


Ciphertext ECC::Encrypt(Point Pm, int k){
    Ciphertext cipher;
    cipher.kB = Sage_add(generator.G, k);
    cipher.msg = add(Pm, Sage_add(Pa, k));
    return cipher;
}