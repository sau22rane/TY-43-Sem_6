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
int A = 0;
int B = 0;

typedef struct Point{
    int x, y;
    Point(int x, int y){
        this->x = x;
        this->y = y;
    }
    Point(){};
}Point;

class ECC{
    private:
    int clifd, n;
    socklen_t len;
    struct sockaddr_in servaddr;
    char buff[30];
    int key;
    Point G;

    int inv(int num, int mod);
    Point add(Point P, Point Q);
    Point Opt_add(Point P, int index);
    void Exchange_key();
    public:
    ECC();
    ~ECC();
};


int main(){
   ECC ecc;
}

ECC::ECC(){

    cout<<"A and B for Generator (y^2 = x^3 + Ax + B)\nA = ";
    cin>>A;
    cout<<"B = ";
    cin>>B;
    cout<<"\nG(x,y)\nx = ";
    cin>>G.x;
    cout<<"y = ";
    cin>>G.y;
    cout<<"\nMOD = ";
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

void ECC::Exchange_key(){
    buff[0] = '0'+((A & 0xFF000000) >> 8*3);
    buff[1] = '0'+((A & 0x00FF0000) >> 8*2);
    buff[2] = '0'+((A & 0x0000FF00) >> 8*1);
    buff[3] = '0'+((A & 0x000000FF));

    buff[4] = '0'+((B & 0xFF000000) >> 8*3);
    buff[5] = '0'+((B & 0x00FF0000) >> 8*2);
    buff[6] = '0'+((B & 0x0000FF00) >> 8*1);
    buff[7] = '0'+((B & 0x000000FF));

    buff[8] = '0'+((MOD & 0xFF000000) >> 8*3);
    buff[9] = '0'+((MOD & 0x00FF0000) >> 8*2);
    buff[10] = '0'+((MOD & 0x0000FF00) >> 8*1);
    buff[11] = '0'+((MOD & 0x000000FF));

    buff[12] = '0'+((G.x & 0xFF000000) >> 8*3);
    buff[13] = '0'+((G.x & 0x00FF0000) >> 8*2);
    buff[14] = '0'+((G.x & 0x0000FF00) >> 8*1);
    buff[15] = '0'+((G.x & 0x000000FF));

    buff[16] = '0'+((G.y & 0xFF000000) >> 8*3);
    buff[17] = '0'+((G.y & 0x00FF0000) >> 8*2);
    buff[18] = '0'+((G.y & 0x0000FF00) >> 8*1);
    buff[19] = '0'+((G.y & 0x000000FF));
    
    // for(int i= 0; i<12; i++){
    //     cout<<((int)buff[i] - 48);
    // }

    
    
    sendto(clifd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    cout<<"Generator and Mod sent...!\n\n";
    cout<<"Private key(integer): ";
    cin>>this->key;
    Point res = Opt_add(G,key);

    Point Pb = Point(0,0);
    n = recvfrom(clifd, (char *)buff, 30, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    Pb.x|=((int)buff[12]-48)<<8*3;
    Pb.x|=((int)buff[13]-48)<<8*2;
    Pb.x|=((int)buff[14]-48)<<8*1;
    Pb.x|=((int)buff[15]-48);

    Pb.y|=((int)buff[16]-48)<<8*3;
    Pb.y|=((int)buff[17]-48)<<8*2;
    Pb.y|=((int)buff[18]-48)<<8*1;
    Pb.y|=((int)buff[19]-48);
    cout<<"\nPb = ("<<Pb.x<<", "<<Pb.y<<")\n";

    buff[12] = '0'+((res.x & 0xFF000000) >> 8*3);
    buff[13] = '0'+((res.x & 0x00FF0000) >> 8*2);
    buff[14] = '0'+((res.x & 0x0000FF00) >> 8*1);
    buff[15] = '0'+((res.x & 0x000000FF));

    buff[16] = '0'+((res.y & 0xFF000000) >> 8*3);
    buff[17] = '0'+((res.y & 0x00FF0000) >> 8*2);
    buff[18] = '0'+((res.y & 0x0000FF00) >> 8*1);
    buff[19] = '0'+((res.y & 0x000000FF));

    sendto(clifd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    Point shared_sec = Opt_add(Pb,key);
    cout<<endl;
    cout<<"Shared Secret: ("<<shared_sec.x<<", "<<shared_sec.y<<")\n";
    
}


ECC::~ECC(){
    close(clifd);   
}

int ECC::inv(int num, int mod){
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

Point ECC::add(Point P, Point Q){
    int m;
    if(P.x == Q.x && P.y == Q.y){
        m = ((3*P.x*P.x + A) * inv(2*P.y, MOD))%MOD;
    }
    else{
        m = ((P.y-Q.y) * inv(P.x-Q.x, MOD))%MOD;
    }
    if(m<0) m+=MOD;
    Point R;
    R.x = (m*m - P.x - Q.x)%MOD;
    R.y = (m*(P.x - R.x) - P.y)%MOD;
    if(R.x<0) R.x+=MOD;
    if(R.y<0) R.y+=MOD;


    // cout<<"("<<P.x<<", "<<P.y<<") + ";
    // cout<<"("<<Q.x<<", "<<Q.y<<") = ";
    // cout<<"("<<R.x<<", "<<R.y<<"), m = "<<m <<endl;
    return R;
}

Point ECC::Opt_add(Point P, int index){
    Point y=P, u=P;
    index--;
    while (index >0){
        if ((index % 2)==1)
            y= add(y,u);
        if (index > 0)
            index=(int)(index / 2);
        u=add(u,u);
    // cout<<"("<<y.x<<", "<<y.y<<")" <<endl;
    }
    return y;
}