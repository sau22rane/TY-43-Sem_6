#include<iostream>

using namespace std;
int gcd(int a, int b, int* x, int* y);

class Hill_Cipher{
    public:
        int **key, **inverse;
        int dim_c;
        int dim = 3, **data, **enc, **dec;
        Hill_Cipher();
        int modMultiInverse(int num, int mod);
        int modAddInverse(int num, int mod);
        int** Inverse(int** m , int n );
        int** matMulti(int **mat_k, int **mat_d);
        int Determinant(int ** mat, int dim);
        void DataMatrix(string d);
        int** Encrypt();
        int** Decrypt();
};

int main(){
    Hill_Cipher hillC;

    cout<<"Enter the data: ";
    string d;
    cin>>d;

    hillC.DataMatrix(d);
    
    
    // int **enc = hillC.matMulti(hillC.mat, hillC.data);
    int **enc = hillC.Encrypt();
    cout<<"Cipher Text: ";

    for(int i = 0;i<d.length();i++){
        cout<<(char)(enc[i%hillC.dim][i/hillC.dim]+'A');
    }
    cout<<endl;

    cout<<"Decrypted Text: ";
    int **dec = hillC.Decrypt();
    for(int i = 0;i<d.length();i++){
        cout<<(char)(dec[i%hillC.dim][i/hillC.dim]+'A');
    }
    cout<<endl;
    return 0;
}


Hill_Cipher::Hill_Cipher(){
    cout<<"The dimension of the Key matrix: "<<this->dim<<"*"<<this->dim<<endl;
    cout<<"Enter the key: ";
    this->key = (int**) malloc(sizeof(int*)*this->dim);
    char temp ;
    for(int i = 0; i<this->dim;i++){
        this->key[i] = (int*)malloc(sizeof(int)*this->dim);
        for(int j = 0;j<this->dim;j++){
            cin>>temp;
            this->key[i][j] = (int) (temp - 'A');
        }
    }
    this->inverse = this->Inverse(this->key, dim);

    cout<<endl<<"Encryption Matrix:"<<endl;
    for(int i = 0;i<dim;i++){
        for(int j =0; j<dim; j++)
            cout<<this->key[i][j] << "\t";
    cout<<endl;
    }
    cout<<endl;
    cout<<endl<<"Decryption Matrix(Inverse):"<<endl;

    for(int i = 0;i<dim;i++){
        for(int j =0; j<dim; j++)
            cout<<this->inverse[i][j] << "\t";
    cout<<endl;
    }
    cout<<endl;
    cout<<endl;   
}


int Hill_Cipher::modMultiInverse(int num, int mod){
    int x, y;
    int g = gcd(num, mod, &x, &y);
    if (g != 1){
        printf("Inverse doesn't exist");
        return -1;
    }
    else{
        int res = (x % mod + mod) % mod;
        return res;
    }

}

int gcd(int a, int b, int* x, int* y){
    if (a == 0) {
        *x = 0, *y = 1;
        return b;
    }

    int x1, y1;
    int g = gcd(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
 
    return g;
}

int Hill_Cipher::modAddInverse(int num, int mod){
    if(num>=0)
        return num%mod;
    num*=-1;
    return mod-num%mod;
}

int Hill_Cipher::Determinant(int ** mat, int dim){
    if(dim==1){
        return mat[0][0];
    }
    int **t = (int**) malloc(sizeof(int*)*(dim-1));
    for(int i = 0; i<dim-1; i++)
        t[i] = (int*) malloc(sizeof(int)*(dim-1));
    int sum = 0;
    int flag = 1;
    for(int i = 0; i<dim;i++){

        for(int j = 1; j < dim; j++){
            for(int k = 0; k < dim; k++){
                if(k == i)
                    continue;
                int indx = k;
                if(k>i)
                    indx--;
                t[j-1][indx] = mat[j][k];
            }
        }
        int h = Determinant(t, dim-1);
        sum = sum + mat[0][i]*flag*h;
        flag*=-1;
    }

    return sum;
}


int** Hill_Cipher::Inverse(int** m , int n ){
	int **adj, **invr;
    adj = (int**) malloc(sizeof(int*)*n);
    invr = (int**) malloc(sizeof(int*)*n);
    for(int i = 0; i<n;i++){
        adj[i] = (int*)malloc(sizeof(int)*n);
        invr[i] = (int*)malloc(sizeof(int)*n);
    }

    for(int i = 0; i<n;i++)
        for(int j = 0;j<n;j++)
            adj[i][j] = 0;

	int d = Determinant(m , n);
    d = modAddInverse(d, 26);
    cout<<d<<endl;
	int d_inv = modMultiInverse(d, 26);
    if(d_inv == -1){
        cout<<"Inverse Does not exist. Choose another key..!\n";
        exit(0);
    }

    int temp[5][5] = {0} ;
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
            temp[i][j] = m[i%3][j%3] ;

    for(int i=1; i<=3 ; i++)
        for(int j=1; j<=3 ; j++)
            adj[j-1][i-1] = temp[i][j]*temp[i+1][j+1] - temp[i][j+1]*temp[i+1][j];

	for(int i=0; i<n ; i++)
		for(int j=0; j<n ; j++)
			invr[i][j] = modAddInverse(adj[i][j] * d_inv, 26) ;
    for(int i = 0; i<dim; i++)
        free(adj[i]);
    free(adj);
    return invr;
}


void Hill_Cipher::DataMatrix(string d){
    int m = d.length();
    this->dim_c = m/this->dim;
    if(m%this->dim!=0)
        this->dim_c++;
    data = (int**) malloc(sizeof(int*)*this->dim);
    for(int i = 0; i<this->dim;i++){
        data[i] = (int*)malloc(sizeof(int)*this->dim_c);
    }

    
    for(int i =0; i<d.length(); i++)
        data[i%this->dim][i/this->dim] = (int) (d.at(i) - 'A');


    for(int i = d.length(); i<=(this->dim-1)*(this->dim_c-1); i++)
        data[i/this->dim][i%this->dim] = (int)'\0';


    cout<<"\nData Matrix:"<<endl;
    for(int i = 0;i<this->dim;i++){
        for(int j =0; j<this->dim_c; j++)
            cout<<data[i][j] << "\t";
    cout<<endl;
    }
    cout<<endl;
}


int** Hill_Cipher::matMulti(int **mat_k, int **mat_d){
    int **t = (int**) malloc(sizeof(int*)*dim);
    for(int i = 0; i<dim; i++){
        t[i] = (int*) malloc(sizeof(int)*dim_c);
    }
    for(int i = 0; i<dim; i++){
        for(int j = 0; j<dim_c; j++){
            int sum = 0;
            for(int k = 0; k<dim; k++){
                sum+=mat_d[k][j]*mat_k[i][k];
            }
            t[i][j] = sum%26;
        }
    }

    return t;
}

int** Hill_Cipher::Encrypt(){
    this->enc = this->matMulti(this->key, this->data);
    return this->enc;
    
}

int** Hill_Cipher::Decrypt(){
    this->dec = this->matMulti(this->inverse, this->enc);
    return this->dec;
}