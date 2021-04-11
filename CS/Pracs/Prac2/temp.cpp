#include<iostream>
using namespace std;

int **data;

int determinant(int ** mat, int dim){
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
        int h = determinant(t, dim-1);
        sum = sum + mat[0][i]*flag*h;
        flag*=-1;
    }
    
    return sum;
}

void DataMatrix(string d){
    int dim = 3, dim_c;
    int m = d.length();
    dim_c = m/dim;
    if(m%dim!=0)
        dim_c++;
    data = (int**) malloc(sizeof(int*)*dim);
    for(int i = 0; i<dim;i++){
        data[i] = (int*)malloc(sizeof(int)*dim_c);
    }

    
    for(int i =0; i<d.length(); i++)
        data[i%dim][i/dim] = (int) (d.at(i) - 'A');


    for(int i = d.length(); i<=(dim-1)*(dim_c-1); i++)
        data[i/dim][i%dim] = (int)'\0';


    cout<<"\nData Matrix:"<<endl;
    for(int i = 0;i<dim;i++){
        for(int j =0; j<dim_c; j++)
            cout<<data[i][j] << "\t";
    cout<<endl;
    }
    cout<<endl;
}

int main(){
    cout<<"Enter the data: ";
    string d;
    cin>>d;
    DataMatrix(d);
    cout<<determinant(data, 3);
}