#include<iostream>
#include<fstream>
using namespace std;

int main(){
    ifstream file;
    file.open("data.txt");
    int temp, n = 10;
    for(int i = 0;i<n;i++){
    file >> temp;
    cout<<temp;
  }

}