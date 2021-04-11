#include<iostream>
using namespace std;

class AES{
    private:
    int key, W0, W1, W2, W3, W4, W5, K1, K2, K3;
    int S_BOX[16]={ 0b1001, 0b0100, 0b1010, 0b1011,
                    0b1101, 0b0001, 0b1000, 0b0101,
                    0b0110, 0b0010, 0b0000, 0b0011,
                    0b1100, 0b1110, 0b1111, 0b0111 };
    int S_BOX_INV[16];
    int irreducible = 0b10011;
    int Me[2][2] = {{1,4},{4,1}};
    int InvMe[2][2] = {{9,2},{2,9}};

    int SubNib(int num);
    int InvSubNib(int num);
    int RotNib(int num);
    int RowShift(int num);
    int PolyReduce(int num);
    void MatMul(int a[2][2], int b[2][2], int res[2][2]);
    int MixCol(int a[2][2]);

    public:
    AES(int key);
    void KeyGenerate();
    int Encrypt(int data);
    int Decrypt(int data);
};

void binary(int num, int n);

int main(){

    string key_str;
    int key = 0, len;
    cout<<"Enter a Key(16 bits): ";         // 0100101011110101
    cin>>key_str;
    len = key_str.length();
    for(int i = 0; i<len; i++){
        key = key<<1;
        key|=(key_str.at(i) - '0');
    }

    int data= 0 ;
    string data_str;
    cout<<"Enter data(16 bits): ";          // 1101011100101000
    cin>>data_str;
    for(int i = 0; i<16; i++){
        data = data<<1;
        data|=(data_str.at(i) - '0');
    }
    cout<<"\nKey:\t";
    binary(key, 16);
    cout<<"Data:\t";
    binary(data, 16);
    cout<<endl;

    AES aes( key );
    aes.KeyGenerate();
    int enc = aes.Encrypt( data );
    cout<<"\nCipher Text: ";
    binary(enc, 16);
    int dec = aes.Decrypt( enc );
    cout<<"Decrypted Text: ";
    binary(dec, 16);
}

AES::AES(int key){
    this->key = key;

    for(int i = 0; i<16; i++){
        this->S_BOX_INV[ this->S_BOX[i] ] = i;
    }
}

void AES::KeyGenerate(){
    this->W0 = (this->key >> 8);
    this->W1 = (this->key & 0xFF);
    this->W2 = (this->W0 ^ 0x80 ^ SubNib(RotNib(this->W1)) );
    this->W3 = (this->W2 ^ this->W1);
    this->W4 = (this->W2 ^ 0x30 ^ SubNib(RotNib(this->W3)) );
    this->W5 = (this->W4 ^ this->W3);
    cout<<"W0:\t";
    binary(W0, 8);
    cout<<"W1:\t";
    binary(W1, 8);
    cout<<"W2:\t";
    binary(W2, 8);
    cout<<"W3:\t";
    binary(W3, 8);
    cout<<"W4:\t";
    binary(W4, 8);
    cout<<"W5:\t";
    binary(W5, 8);

    this->K1 = ((this->W0 << 8) | this->W1);
    this->K2 = ((this->W2 << 8) | this->W3);
    this->K3 = ((this->W4 << 8) | this->W5);
    cout<<"\nK1:\t";
    binary(this->K1, 16);
    cout<<"K2:\t";
    binary(this->K2, 16);
    cout<<"K3:\t";
    binary(this->K3, 16);
}

int AES::Encrypt(int data){
    int ARK1 = (data ^ this->K1);
    int subNib = SubNib(ARK1);
    int shiftRow = RowShift(subNib);
   
    int mat[2][2];
    for(int i = 0; i<2; i++){
        for(int j = 0; j<2; j++){
            mat[j][i] = ((shiftRow >> (3 - i*2-j)*4) & 0x0F);
        }
    }

    int S_mat[2][2];
    MatMul(Me, mat, S_mat);

    int mixColn = MixCol(S_mat);
    int ARK2 = (mixColn^this->K2);
    int subNib2 = SubNib(ARK2);
    int shiftRow2 = RowShift(subNib2);

    int ARK3 = (shiftRow2^this->K3);
    
    return ARK3;

}

int AES::Decrypt(int data){
    int ARK3 = (data^this->K3);
    int shiftRow2 = RowShift(ARK3);
    int subNib2 = InvSubNib(shiftRow2);
    int ARK2 = (subNib2^this->K2);

    int mat[2][2];
    for(int i = 0; i<2; i++){
        for(int j = 0; j<2; j++){
            mat[j][i] = ((ARK2 >> (3 - i*2-j)*4) & 0x0F);
        }
    }

    int S_mat[2][2];
    MatMul(InvMe, mat, S_mat);

    int mixColn = MixCol(S_mat);
    int shiftRow1 = RowShift(mixColn);
    int subNib1 = InvSubNib(shiftRow1);
    int ARK1 = (subNib1^this->K1);
    return ARK1;
}




int AES::SubNib(int num){
    int i = 0, res = 0;
    while ((num>> i*4) !=0){
        res|= (S_BOX[((num & (15<<i*4))>>i*4)]<<i*4);
        i++;
    }
    return res;
}

int AES::InvSubNib(int num){
    int i = 0, res = 0;
    while ((num>> i*4) !=0){
        res|= (S_BOX_INV[((num & (15<<i*4))>>i*4)]<<i*4);
        i++;
    }
    return res;
}

int AES::RowShift(int num){
    return ((num & 0xF0F0) | ((num & 0x000F)<<8) | ((num & 0x0F00)>>8));
}

int AES::RotNib(int num){
    return ( ((num & 0xF0)>>4) | ((num & 0x0F)<<4) );
}


int AES::PolyReduce(int num){
    int i = 7;
    while( (num>>4) >0 && i>3){
        if( (num & (1<<i) ) == 0 ){
            i--;
            continue;
        }
        num^=(this->irreducible<<(i-4));
        i--;
    }
    return num;
}

void AES::MatMul(int a[2][2], int b[2][2], int res[2][2]){
    for(int i = 0; i<2; i++){
        for(int j = 0; j<2; j++){
            int sum = 0;
            for(int k = 0; k<2; k++){
                int t = 0;
                for(int l = 0; l<4; l++){
                    t ^= ( (a[i][k]<<l) * ((b[k][j]>>l)&1) );
                }
                sum^= PolyReduce(t);

            }
            res[i][j] = sum;
        }
    }
}

int AES::MixCol(int a[2][2]){
    int res = 0;
    for(int i = 0; i<2; i++){
        for(int j = 0; j<2; j++){
            res |= (a[j][i] << (3 - i*2-j)*4);
        }
    }
    return res;
}



void binary(int num, int n){
    for(int i = 1;i<=n;i++){
        cout<<( (num>>(n-i)) & 1);
        if(i%4==0)
            cout<<" ";
    }
    cout<<endl;
}