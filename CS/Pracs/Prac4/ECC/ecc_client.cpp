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
    int clifd, n;
    socklen_t len;
    struct sockaddr_in servaddr;
    char buff[30];
    int key;
    Point Pb = Point(0,0);
    Generator generator;

    int inv(int num, int mod);
    Point add(Point P, Point Q);
    Point subtract(Point P, Point Q);
    Point Sage_add(Point P, int index);
    void Exchange_key();
    public:
    ECC();
    ~ECC();
    Point Decrypt(Ciphertext cipher);
};


int main(){
   ECC ecc;
}

ECC::ECC(){

    cout<<"A and B for Generator (y^2 = x^3 + Ax + B)\nA = ";
    cin>>generator.A;
    cout<<"B = ";
    cin>>generator.B;
    cout<<"\nG(x,y)\nx = ";
    cin>>generator.G.x;
    cout<<"y = ";
    cin>>generator.G.y;
    cout<<"\nMOD = ";
    cin>>generator.MOD;
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

void ECC::Exchange_key(){
    memset(buff, 0x00, sizeof(generator));
    memcpy(buff, &generator, sizeof(generator));
    
    sendto(clifd, (const char *)buff, sizeof(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    cout<<"Generator and Mod sent...!\n\n";
    cout<<"Private key(integer): ";
    cin>>this->key;
    Point res = Sage_add(generator.G,key);

    n = recvfrom(clifd, (char *)buff, 30, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    memcpy(&Pb, buff, sizeof(Pb));

    cout<<"\n\nPb(Public Key of Server) = ("<<Pb.x<<", "<<Pb.y<<")\n";
    memset(buff, 0x00, sizeof(res));
    memcpy(buff, &res, sizeof(res));

    sendto(clifd, (const char *)buff, sizeof(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    
    n = recvfrom(clifd, (char *)buff, 30, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    
    Ciphertext cipher;
    memcpy(&cipher, buff, sizeof(cipher));

    cout<<"\nRecieved Cipher = "<<"["<<"("<<cipher.kB.x<<", "<<cipher.kB.y<<"), ("<<cipher.msg.x<<", "<<cipher.msg.y<<")"<<"]\n";
    Point Pm;
    Pm = Decrypt(cipher);
    cout<<"Pm(Decrypted) = ("<<Pm.x<<", "<<Pm.y<<")\n";

}


ECC::~ECC(){
    close(clifd);   
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


Point ECC::Decrypt(Ciphertext cipher){
    return subtract(cipher.msg, Sage_add(cipher.kB, key));
}


Point ECC::subtract(Point P, Point Q){
    Q.y*=-1;
    return add(P, Q);
}