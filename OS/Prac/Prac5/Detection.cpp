#include<iostream>
#include<fstream>
#include <cstring>

using namespace std;

void getData(int *n, int *m, int **Work, int ***Allocation, int ***Request);

int main(){
    int n, m, *Work, **Allocation, **Request;
    bool *Finish; 

    getData(&n, &m, &Work, &Allocation, &Request);
    Finish = (bool*) malloc(sizeof(bool) * n);
    memset(Finish, 0, sizeof(Finish));
    

    for(int i = 0; i<n; i++){
        bool flag = true;
        for(int j = 0; j<m; j++){
            if(Allocation[i][j]!=0){
                flag = false;
                break;
            }
        }
        Finish[i] = flag;
    }

    while(1){
        int flag = -1;
        for(int i = 0; i<n; i++){
            if(Finish[i] == true)
                continue;
            flag  = i;
            for(int j = 0; j<m; j++){
                if(Request[i][j]>Work[j]){
                    flag = -1;
                    break;
                }
            }
            if(flag == i)
                break;
        }
        if(flag == -1)
            break;
        
        for(int i = 0; i<m; i++){
            Work[i]+= Allocation[flag][i];
        }
            Finish[flag] = true;

    }

    cout<<"\nProcess Status:\n";
    bool deadlock = false;
    for(int i = 0; i<n; i++){
        cout<<"Process P"<<i<<" : ";
        if(Finish[i])
            cout<<"Completed"<<endl;
        else{
            cout<<"Deadlock"<<endl;
            deadlock = true;
        }
    }

    if(deadlock)
        cout<<"\nDeadlock Detected...!!!\n";
    else
        cout<<"\nDeadlock Free...!!!\n";


    return 0;
}


void getData(int *n, int *m, int **Work, int ***Allocation, int ***Request){
    ifstream file;
    file.open("data.txt");
    file >> *n;
    file >> *m;

    *Allocation = (int**) malloc(sizeof(int*) * (*n));
    *Request = (int**) malloc(sizeof(int*) * (*n));
    *Work = (int*) malloc(sizeof(int) * (*m));

    for(int i = 0; i<*n; i++){
        (*Allocation)[i] = (int*) malloc(sizeof(int)*(*m));
        (*Request)[i] = (int*) malloc(sizeof(int)*(*m));
    }

    for(int i = 0;i<*n;i++){
        for(int j = 0;j<*m;j++){
            file>>(*Allocation)[i][j];
        }
    }

    for(int i = 0;i<*n;i++){
        for(int j = 0;j<*m;j++){
            file>>(*Request)[i][j];
        }
    }

    for(int i = 0;i<*m;i++){
            file>>(*Work)[i];
    }


    cout<<"Number of processes: "<<*n<<endl;

    cout<<"Number of resources: "<<*m<<endl<<endl;

    cout<<"Allocation matrix:\n   ";
    for(int i = 0; i<*m; i++)
        cout<<" "<<(char)('A'+i);
    cout<<endl;
    
    for(int i = 0;i<*n;i++){
        cout<<"P"<<i<<": ";
        for(int j = 0;j<*m;j++){
            cout<<(*Allocation)[i][j]<<" ";
        }
        cout<<endl;
    }

    cout<<"\nRequest matrix:\n   ";
    for(int i = 0; i<*m; i++)
        cout<<" "<<(char)('A'+i);
    cout<<endl;

    for(int i = 0;i<*n;i++){
        cout<<"P"<<i<<": ";
        for(int j = 0;j<*m;j++){
            cout<<(*Request)[i][j]<<" ";
        }
        cout<<endl;
    }

    cout<<"\nWork matrix:\n";
    for(int i = 0; i<*m; i++)
        cout<<(char)('A'+i)<<" ";
    cout<<endl;
    for(int i = 0;i<*m;i++){
            cout<<(*Work)[i]<<" ";
    }
    cout<<endl;
}