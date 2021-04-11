#include<iostream>

using namespace std;

class DES{
    private:
        int P10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
        int P8[8] = {6, 3, 7, 4, 8, 5, 10, 9};
        int ip[8] = {2, 6, 3, 1, 4, 8, 5, 7};
        int ip_inv[8] = {4, 1, 3, 5, 7, 2, 8, 6};
        int E_P[8] = {4, 1, 2, 3, 2, 3, 4, 1};
        int p4[4] = {2,4,3,1};

        int S0[4][4] = {{1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 3, 2}};
        int S1[4][4] = {{0, 1, 2, 3}, {2, 0, 1, 3}, {3, 0, 1, 0}, {2, 1, 0, 3}};
        int SBOX0(int s);
        int SBOX1(int s);
        int P(int *p,int k, int len, int n);
        int LS(int k);

    public:
        int key, key1, key2;
        DES(int key);
        int Encrypt(int data,int key1,int key2);
        int Decrypt(int data,int key2,int key1);
        
};

void binary(int num, int n);

int main(){
    
    string key_str;
    int key = 0, len;
    cout<<"Enter a Key(10 bits): ";
    cin>>key_str;
    len = key_str.length();
    for(int i = 0; i<len; i++){
        key = key<<1;
        key|=(key_str.at(i) - '0');
    }
    cout<<"Key in Decimal is: "<<key<<endl;
    
    DES des(key);

    int data= 0 ;
    string data_str;
    cout<<"Enter data(8 bits): ";
    cin>>data_str;
    for(int i = 0; i<8; i++){
        data = data<<1;
        data|=(data_str.at(i) - '0');
    }
    cout<<"Data in decimal: "<<data<<endl<<endl;
    int enc = des.Encrypt(data, des.key1, des.key2);
    cout<<"Encrypted Text: ";
    binary(enc, 8);


    int dec = des.Decrypt(enc, des.key1, des.key2);
    cout<<"Decrypted Text: ";
    binary(dec, 8);
}




void binary(int num, int n){
    for(int i = 1;i<=n;i++){
        cout<<( (num>>(n-i)) & 1);
    }
    cout<<endl;
}

int DES::SBOX0(int s){
    int row = 0, col = 0;
    row = ( (s&8) >>2) | (s&1);
    col = ( (s&4) >>1) | ( (s&2) >>1);
    return S0[row][col];
}

int DES::SBOX1(int s){
    int row = 0, col = 0;
    row = ( (s&8) >>2) | (s&1);
    col = ( (s&4) >>1) | ( (s&2) >>1);
    return S1[row][col];
}

int DES::P(int *p,int k, int len, int n){
    int result = 0;
    for(int i = 1 ; i <= len ; i++){
        result |= ((k>>(n - p[i-1]) & 1)<<(len-i));
    }
    return result;
}


int DES::LS(int k){
    int result = 0 ;
    int temp1 = (k & 33);
    int temp2 = (k & 528);
    result = ((k<<1) & 990) | (temp2>>4);
    k = result;
    return result;
}

int DES::Encrypt(int data,int key1, int key2){
    int ip_result = P( ip , data, 8, 8);
    int left_ip = ip_result>>4;
    int right_ip = (ip_result & 15);
    
    int e_p_result = P( E_P ,  right_ip, 8, 4);
    int xor_result_1 = (e_p_result ^ key1);
    
    int sboxL0 = SBOX0(xor_result_1>>4);
    int sboxR0 = SBOX1(xor_result_1&15);
    int sbox = (sboxL0<<2) | sboxR0;

    int P_4bit = P( p4 , sbox, 4, 4) ;

    int xor_4bit =  ((P_4bit ^ left_ip)<<4) |  right_ip;
    left_ip = xor_4bit>>4;
    right_ip = (xor_4bit&15);

    int temp = ( P( E_P , left_ip, 8, 4) ^ key2 ); 

    sboxL0 = SBOX0(temp>>4);
    sboxR0 = SBOX1(temp&15);
    sbox = (sboxL0<<2) | sboxR0;
    P_4bit = P( p4 , sbox, 4, 4) ;
    xor_4bit =  ((P_4bit ^ right_ip)<<4) | left_ip ;

    int cipher_text = P(ip_inv , xor_4bit, 8, 8);
    return cipher_text;
}

int DES::Decrypt(int data,int key2,int key1){
    return Encrypt(data, key1, key2);
}


DES::DES(int key){
    this->key = key;
    int p =P(P10,key, 10, 10);
    int ls1 = LS(p);
    this->key1 =P(P8, ls1, 8, 10);
    int ls2 = LS(LS(ls1));
    this->key2 =P(P8, ls2, 8, 10);
    cout<<"\nKey1: ";
    binary(this->key1, 8);
    cout<<"Key2: ";
    binary(this->key2, 8);
    cout<<endl;
}
