#include<iostream>
using namespace std;

int exp(int num, int index, int mod){
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

int main(){
    cout<<exp(9, 3, 23)<<endl;;
}