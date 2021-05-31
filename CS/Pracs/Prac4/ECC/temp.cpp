#include<iostream>
using namespace std;
int MOD = 11;
int A = 1;
int B = 6;
int k = 3;

typedef struct Point{
    int x, y;
    Point(int x, int y){
        this->x = x;
        this->y = y;
    }
    Point(){};
}Point;

typedef struct Ciphertext{
    Point kB, msg;
}Ciphertext;


int inv(int num, int mod);
Point add(Point P, Point Q);
Point Opt_add(Point P, int index);

int main(){
    Point q = Point(13, 22);
    Point p = Point(22, 19);
    Point r = add(p,q);
    cout<<"("<<p.x<<", "<<p.y<<") + ("<<q.x<<", "<<q.y<<") = ("<<r.x<<" "<<r.y<<")"<<endl;
    // p.x*=-1;
    p.y*=-1;
    q = add(r,p);
    // cout<<q.x<<" "<<q.y<<endl;
    cout<<"("<<r.x<<", "<<r.y<<") + ("<<p.x<<", "<<p.y<<") = ("<<q.x<<" "<<q.y<<")"<<endl;

    // Point B = Point(4,5);
    // Point Pm = Point(11, 17);
    // Ciphertext cipher;
    // cipher.kB = Opt_add(B, k);

    for(int i = 0; i<MOD; i++){
        int res = i*i*i + A*i + B;
        for(int j = 0; j<MOD; j++){
            if((j*j)%MOD == res%MOD)
                cout<<"("<<i<<", "<<j<<") ";
        }
    }
}

Point add(Point P, Point Q){
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

Point Opt_add(Point P, int index){
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

int inv(int num, int mod){
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