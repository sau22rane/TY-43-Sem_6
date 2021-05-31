#include<iostream>
#define A 2
#define B 15
int MOD = 23;
using namespace std;


typedef struct Point{
    int x, y;
    Point(int x, int y){
        this->x = x;
        this->y = y;
    }
    Point(){};
}Point;

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





int main(){
    Point P(18,5);
    int arr[11] = {3,7,11,13,17,29,23,29,31,37,41};
    for(int i =0; i<11; i++){
        MOD = arr[i];
        Point res = Opt_add(P,3);
        cout<<"MOD = "<<MOD<<" ("<<res.x<<", "<<res.y<<")" <<endl;
    }
    return 0;
}
