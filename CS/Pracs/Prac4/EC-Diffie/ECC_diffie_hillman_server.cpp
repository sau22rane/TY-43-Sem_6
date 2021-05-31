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
    int listenfd, connfd;
    struct sockaddr_in srvinfo, cliinfo;
    socklen_t len;
    char buff[30];
    Point G = Point(0,0);
    int key;

    int inv(int num, int mod);
    Point add(Point P, Point Q);
    Point Opt_add(Point P, int index);
    
    public:
    ECC();
    ~ECC();
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
    bzero(buff, sizeof(buff));
    int n = recvfrom(listenfd, (char *)buff, 30, MSG_WAITALL, ( struct sockaddr *) &cliinfo, &len);
    
    // for(int i= 0; i<12; i++){
    //     cout<<((int)buff[i] - 48);
    // }
    // cout<<endl;

    A|=((int)buff[0]-48)<<8*3;
    A|=((int)buff[1]-48)<<8*2;
    A|=((int)buff[2]-48)<<8*1;
    A|=((int)buff[3]-48);

    B|=((int)buff[4]-48)<<8*3;
    B|=((int)buff[5]-48)<<8*2;
    B|=((int)buff[6]-48)<<8*1;
    B|=((int)buff[7]-48);

    MOD|=((int)buff[8]-48)<<8*3;
    MOD|=((int)buff[9]-48)<<8*2;
    MOD|=((int)buff[10]-48)<<8*1;
    MOD|=((int)buff[11]-48);

    G.x|=((int)buff[12]-48)<<8*3;
    G.x|=((int)buff[13]-48)<<8*2;
    G.x|=((int)buff[14]-48)<<8*1;
    G.x|=((int)buff[15]-48);

    G.y|=((int)buff[16]-48)<<8*3;
    G.y|=((int)buff[17]-48)<<8*2;
    G.y|=((int)buff[18]-48)<<8*1;
    G.y|=((int)buff[19]-48);

    cout<<"A = "<<A<<", B = "<<B<<"\nG = ("<<G.x<<", "<<G.y<<"), MOD = "<<MOD<<endl<<endl;

    cout<<"Private key(integer): ";
    cin>>this->key;
    Point res = Opt_add(G,key);
    Point Pa = Point(0,0);

    buff[12] = '0'+((res.x & 0xFF000000) >> 8*3);
    buff[13] = '0'+((res.x & 0x00FF0000) >> 8*2);
    buff[14] = '0'+((res.x & 0x0000FF00) >> 8*1);
    buff[15] = '0'+((res.x & 0x000000FF));

    buff[16] = '0'+((res.y & 0xFF000000) >> 8*3);
    buff[17] = '0'+((res.y & 0x00FF0000) >> 8*2);
    buff[18] = '0'+((res.y & 0x0000FF00) >> 8*1);
    buff[19] = '0'+((res.y & 0x000000FF));
    
    sendto(listenfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliinfo, len);


    n = recvfrom(listenfd, (char *)buff, 30, MSG_WAITALL, (struct sockaddr *) &cliinfo, &len);
    Pa.x|=((int)buff[12]-48)<<8*3;
    Pa.x|=((int)buff[13]-48)<<8*2;
    Pa.x|=((int)buff[14]-48)<<8*1;
    Pa.x|=((int)buff[15]-48);

    Pa.y|=((int)buff[16]-48)<<8*3;
    Pa.y|=((int)buff[17]-48)<<8*2;
    Pa.y|=((int)buff[18]-48)<<8*1;
    Pa.y|=((int)buff[19]-48);
    cout<<"\nPa = ("<<Pa.x<<", "<<Pa.y<<")\n";
    Point shared_sec = Opt_add(Pa,key);
    cout<<endl;
    cout<<"Shared Secret: ("<<shared_sec.x<<", "<<shared_sec.y<<")\n";
}

ECC::~ECC(){
    close(listenfd);   
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