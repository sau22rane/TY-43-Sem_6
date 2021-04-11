#include<iostream>
using namespace std;

void binary(int num, int n){
    for(int i = 1;i<=n;i++){
        cout<<( (num>>(n-i)) & 1);
        if(i%4==0)
            cout<<" ";
    }
    cout<<endl;
}

int PolyReduce(int num){
    int irreducible = 0b10011;
    int i = 7;
    while( (num>>4) >0 && i>3){
        if( (num & (1<<i) ) == 0 ){
            i--;
            continue;
        }
        num^=(irreducible<<(i-4));
        i--;
    }
    return num;
}

int main(){

    int Me[2][2] = {{1,4},{4,1}};
    int mat[2][2] = {{2,14},{14,14}};
    int S_mat[2][2];

    // for(int i = 0; i<2; i++){
    //     for(int j = 0; j<2; j++){
    //         int sum = 0;
    //         for(int k = 0; k<2; k++){
    //             int t = 0;
    //             for(int l = 0; l<4; l++){
    //                 t ^= ( (Me[i][k]<<l) * ((mat[k][j]>>l)&1) );
    //             }
    //             sum^=t;
    //             binary(t, 8);

    //         }
    //         S_mat[i][j] = sum;
    //     }
    //     cout<<endl;
    // }

    // for(int i = 0; i<2; i++){
    //     for(int j = 0; j<2; j++){
    //         cout<<S_mat[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    int t = 0;
    for(int l = 0; l<4; l++){
        t ^= ( (15<<l) * ((15>>l)&1) );
    }
    binary(t, 8);
    binary(PolyReduce(t), 4);

}

