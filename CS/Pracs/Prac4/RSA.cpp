#include<iostream>
using namespace std;

class RSA{
    private:
    int p, q, d, phi;
    int GCD(int a, int b);
    int exp(int num, int index, int mod);
    int inv(int num);

    public:
    int n, e;

    RSA();
    int Encrypt(int data, int e, int mod);
    int Decrypt(int cipher);
};


int main(){
    RSA rsa;
    cout<<"\nEnter an integer to encrypt: ";
    int msg;
    cin>>msg;
    int enc = rsa.Encrypt(msg, rsa.e, rsa.n);
    cout<<"\nEncrypted message: "<<enc<<endl;

    int dec = rsa.Decrypt(enc);
    cout<<"Decrypted message: "<<dec<<endl;


}

RSA::RSA(){
    cout<<"Enter First Prime number(p): ";
    cin>>this->p;
    cout<<"Enter Second Prime number(q): ";
    cin>>this->q;
    cout<<"Enter Exponent(e): ";
    cin>>this->e;

    if(GCD( this->n, this->e) == 1){
        cout<<"Valid Exponent e = "<<this->e<<endl;
    }
    else{
        cout<<"Invalid Exponent e = "<<this->e<<endl;
        exit(0);
    }


    this->phi = (this->p-1) * (this->q-1);
    this->n = (this->p) * (this->q);
    this->d = this->inv(e);
    cout<<endl<<"-------------------------------------"<<endl;
    cout<<"P = "<<this->p<<endl;
    cout<<"Q = "<<this->q<<endl;
    cout<<"N = "<<this->n<<endl;
    cout<<"Phi(n) = "<<this->phi<<endl;
    cout<<"E = "<<this->e<<endl;
    cout<<"D = "<<this->d<<endl;
    cout<<"-------------------------------------"<<endl;

}

int RSA::Encrypt(int num, int e, int mod){
    return exp(num, e, mod);
}

int RSA::Decrypt(int num){
    return exp(num, this->d, this->n);
}

int RSA::exp(int num, int index, int mod){
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

int RSA::GCD(int a, int b){
    if(b == 1)
        return 1;
    if(b == 0)
        return 0;
    return GCD(b, a%b);
}

int RSA::inv(int num){
    int a = num, m = this->phi, m0 = m;
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