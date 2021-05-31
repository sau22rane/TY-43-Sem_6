#include<iostream>
using namespace std;

class AES{
    private:
    int key, W0, W1, W2, W3, W4, W5, K1, K2, K3;
    int S_BOX[16]={ 0b1001, 0b0100, 0b1010, 0b1011,
                    0b1101, 0b0001, 0b1000, 0b0101,
                    0b0110, 0b0010, 0b0000, 0b0011,
                    0b1100, 0b1110, 0b1111, 0b0111 };
    int S_BOX_INV[16]={
                    0b1010, 0b0101, 0b1001, 0b1011,
                    0b0001, 0b0111, 0b1000, 0b1111,
                    0b0110, 0b0000, 0b0010, 0b0011,
                    0b1100, 0b0100, 0b1101, 0b1110};
                    
    int irreducible = 0b10011;
    int Me[2][2] = {{1,4},{4,1}};
    int InvMe[2][2] = {{9,2},{2,9}};

    int SubNib(int num, int n);
    int InvSubNib(int num, int n);
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

    int key = 19189;

    AES aes( key );
    aes.KeyGenerate();
    int enc = aes.Encrypt( 102 );
    int dec = aes.Decrypt( enc );
    binary(enc, 16);
    binary(dec, 16);
    
    for(int i = (int) 'a'; i<=(int) 'z'; i++){
        int enc = aes.Encrypt( i );
        int dec = aes.Decrypt( enc );
        if(dec!=i){
        cout<<"Failed "<<i<<" "<<dec<<"\n";
        break;
        }
    }
}

AES::AES(int key){
    this->key = key;
    binary(key, 16); // 100101011110101
    cout<<this->key<<endl;

//    for(int i = 0; i<16; i++){
//        this->S_BOX_INV[ this->S_BOX[i] ] = i;
//    }
}

void AES::KeyGenerate(){
    this->W0 = (this->key >> 8);
    this->W1 = (this->key & 0xFF);
    this->W2 = (this->W0 ^ 0x80 ^ SubNib(RotNib(this->W1), 8) );
    this->W3 = (this->W2 ^ this->W1);
    this->W4 = (this->W2 ^ 0x30 ^ SubNib(RotNib(this->W3), 8) );
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
    cout<<"---Debugging Encrypt---\n";
    int ARK1 = (data ^ this->K1);
    int subNib = SubNib(ARK1, 16);
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
    binary(mixColn, 16);
    int ARK2 = (mixColn^this->K2);
    binary(ARK2, 16);
    int subNib2 = SubNib(ARK2, 16);
    binary(subNib2, 16);
    int shiftRow2 = RowShift(subNib2);

    int ARK3 = (shiftRow2^this->K3);
    
    return ARK3;

}

int AES::Decrypt(int data){
    cout<<"\n\n---Debugging Decrypt---\n";
    int ARK3 = (data^this->K3);
    binary(ARK3, 16);
    int shiftRow2 = RowShift(ARK3);
    binary(shiftRow2, 16);
    int subNib2 = InvSubNib(shiftRow2, 16);
    binary(subNib2, 16);
    int ARK2 = (subNib2^this->K2);
    binary(ARK2, 16);

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
    int subNib1 = InvSubNib(shiftRow1, 16);
    int ARK1 = (subNib1^this->K1);
    cout<<endl<<endl;
    return ARK1;
}




int AES::SubNib(int num, int n){
    int i = 0, res = 0;
    if(n == 8){
        res|= (S_BOX[((num & (15<<0*4))>>0*4)]<<0*4);
        res|= (S_BOX[((num & (15<<1*4))>>1*4)]<<1*4);
    }
    else{
        res|= (S_BOX[((num & (15<<0*4))>>0*4)]<<0*4);
        res|= (S_BOX[((num & (15<<1*4))>>1*4)]<<1*4);
        res|= (S_BOX[((num & (15<<2*4))>>2*4)]<<2*4);
        res|= (S_BOX[((num & (15<<3*4))>>3*4)]<<3*4);
    
    }
    return res;
}

int AES::InvSubNib(int num, int n){
    int i = 0, res = 0;
    if(n == 8){
        res|= (S_BOX_INV[((num & (15<<0*4))>>0*4)]<<0*4);
        res|= (S_BOX_INV[((num & (15<<1*4))>>1*4)]<<1*4);
    }
    else{
        res|= (S_BOX_INV[((num & (15<<0*4))>>0*4)]<<0*4);
        res|= (S_BOX_INV[((num & (15<<1*4))>>1*4)]<<1*4);
        res|= (S_BOX_INV[((num & (15<<2*4))>>2*4)]<<2*4);
        res|= (S_BOX_INV[((num & (15<<3*4))>>3*4)]<<3*4);
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
    for(int i = n-1;i>=0;i--){
        cout<<( (num>>(i)) & 1);
        if(i%4==0)
            cout<<" ";
    }
    cout<<endl;
}
