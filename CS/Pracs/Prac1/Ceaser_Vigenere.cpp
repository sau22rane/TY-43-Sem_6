// Implementation of Ceaser, Vigenere cryptography

#include<iostream>
#include<string.h>
#include<random>
#include<time.h>

using namespace std;

class Ceaser{
    private:
    int security;

    public:
    
    Ceaser();
    string Encrypt(string unEncryptedData);
    void BruteForece(string EncryptedData); 
    string Decrypt(string EncryptedData);
    void updateSecret(int sec);
    int getSecretKey();
};


class Vigenere{
    private:
    int* security, len;

    public:
    Vigenere(int len);
    Vigenere();
    string Encrypt(string unEncryptedData);
    string Decrypt(string EncryptedData);
    void getSecretKey();
    void updateSecret(int len);
};

void VigenereMenu(Vigenere *ptr);
void CeaserMenu(Ceaser *ptr);




Ceaser::Ceaser(){
    cout<<"Generating random Secret Key....!!"<<endl<<endl;
    this->security = rand()%26;
}

string Ceaser::Encrypt(string unEncryptedData){
    int len = unEncryptedData.length();
    char* Encrypted;
    Encrypted = (char*) malloc(sizeof(char)*(len+1));
    for(int i=0;i<len; i++){
        if('A'<=unEncryptedData.at(i)  && unEncryptedData.at(i) <='Z')
            *(Encrypted+i) = 'A'+(unEncryptedData.at(i) +this->security-'A')%26;
        else if('a'<=unEncryptedData.at(i)  && unEncryptedData.at(i) <='z')
            *(Encrypted+i) = 'a'+(unEncryptedData.at(i) +this->security-'a')%26;
    }
    *(Encrypted+len) = '\0';
    return string(Encrypted);
}

void Ceaser::BruteForece(string EncryptedData){
    int len = EncryptedData.length();
    char* unEncryptedData;
    cout<<endl<<"Starting Brute Force attack:\n";
    unEncryptedData = (char*) malloc(sizeof(char)*(len+1));
    for (int j = 0; j<26; j++){
        for(int i=0; i<len;i++){
            if('A'<=EncryptedData.at(i)  && EncryptedData.at(i) <='Z')
                *(unEncryptedData+i) = 'A'+(EncryptedData.at(i) +j-'A')%26;
            else if('a'<=EncryptedData.at(i)  && EncryptedData.at(i) <='z')
                *(unEncryptedData+i) = 'a'+(EncryptedData.at(i) +j-'a')%26;
        }
        cout<<unEncryptedData<<endl;
    }
} 

string Ceaser::Decrypt(string EncryptedData){
    int len = EncryptedData.length();
    char* unEncryptedData;
    unEncryptedData = (char*) malloc(sizeof(char)*(len+1));
    for(int i=0; i<len;i++){
        if('A'<=EncryptedData.at(i)  && EncryptedData.at(i) <='Z')
                *(unEncryptedData+i) = 'A'+(EncryptedData.at(i) +26-this->security-'A')%26;
        else if('a'<=EncryptedData.at(i)  && EncryptedData.at(i) <='z')
            *(unEncryptedData+i) = 'a'+(EncryptedData.at(i) +26-this->security-'a')%26;
    }
    return string(unEncryptedData);
}

void Ceaser::updateSecret(int sec){
    this->security = sec;
}

int Ceaser::getSecretKey(){
    return this->security;
}






Vigenere::Vigenere(int len){
    this->len = len;
    this->security = (int*) malloc(sizeof(int)*len);
    cout<<"Generating random Secret Key of length: "<<this->len<<endl<<endl;
    for(int i =0; i<len; i++){
        *(this->security+i) = rand()%26;
    }
}

Vigenere::Vigenere(){
    this->len = rand()%10 + 5;
    this->security = (int*) malloc(sizeof(int)*len);
    cout<<"Generating random Secret Key of length: "<<this->len<<endl<<endl;
    for(int i =0; i<len; i++){
        *(this->security+i) = rand()%26;
    }
}

string Vigenere::Encrypt(string unEncryptedData){
    int len = unEncryptedData.length();
    char* Encrypted;
    Encrypted = (char*) malloc(sizeof(char)*(len+1));

    for(int i=0; i<len; i++){

        if('A'<=unEncryptedData.at(i)  && unEncryptedData.at(i) <='Z')
            *(Encrypted+i) = 'A'+(unEncryptedData.at(i) +*(this->security+(i%this->len))-'A')%26;
        else if('a'<=unEncryptedData.at(i)  && unEncryptedData.at(i) <='z')
            *(Encrypted+i) = 'a'+(unEncryptedData.at(i) +*(this->security+(i%this->len))-'a')%26;
    }
    return string(Encrypted);
}

string Vigenere::Decrypt(string EncryptedData){
    int len = EncryptedData.length();
    char* unEncryptedData;
    unEncryptedData = (char*) malloc(sizeof(char)*(len+1));
    for(int i=0; i<len; i++){
        if('A'<=EncryptedData.at(i)  && EncryptedData.at(i) <='Z')
                *(unEncryptedData+i) = 'A'+(EncryptedData.at(i) +26-*(this->security+(i%this->len))-'A')%26;
        else if('a'<=EncryptedData.at(i)  && EncryptedData.at(i) <='z')
            *(unEncryptedData+i) = 'a'+(EncryptedData.at(i) +26-*(this->security+(i%this->len))-'a')%26;
    }
    return string(unEncryptedData);
}

void Vigenere::getSecretKey(){
    for(int i =0; i<len; i++)
        cout<<*((this->security)+i)<<" ";
}

void Vigenere::updateSecret(int len){
    this->len = len;
    this->security = (int*) malloc(sizeof(int)*len);
    cout<<"Generating random Secret Key of length: "<<this->len<<endl;
    for(int i =0; i<len; i++){
        *(this->security+i) = rand()%26;
    }
    cout<<"Security Key updated...!!"<<endl;
}


int main(){
    srand(time(0));
    while(1){
        cout<<"Encryption Schemes:\n\n1. Ceaser\n2. Vigenere\n0. Exit\n\nYour Seclection: ";
        int opt = 0;
        cin>>opt;
        cout<<endl<<"--------------------------------------------"<<endl<<endl;
        switch(opt){
            case 1:{
            cout<<"Ceaser Encryption\n\n";
            Ceaser c = Ceaser();
            CeaserMenu(&c);
            break;
            }

            case 2:{
            cout<<"Vigenere Encryption\n\n";
            Vigenere v = Vigenere();
            VigenereMenu(&v);
            break;
            }

            case 0:
            system("clear");
            return 0;
        }
        system("clear");
    }
}


void VigenereMenu(Vigenere *ptr){
    while(1){
        cout<<"1. Encrypt Text"<<endl;
        cout<<"2. Decrypt Text"<<endl;
        cout<<"3. Display Secret Key"<<endl;
        cout<<"4. Change Secret Key"<<endl;
        cout<<"0. Exit"<<endl<<endl<<"Your Selection: ";
        int opt = 0;
        cin>>opt;
        cout<<endl;
        switch (opt)
        {
        case 1:{
            string plain;
            cout<<"Enter the String to be Encrypted:\nString: ";
            cin>>plain;
            cout<<"Encrypted Text: "<<ptr->Encrypt(plain);
            break;
        }
        case 2:{
            string enc;
            cout<<"Enter the String to be Decrypted:\nString: ";
            cin>>enc;
            cout<<"Encrypted Text: "<<ptr->Decrypt(enc);
            break;
        }
        case 3:{
            cout<<"Secret Key: ";
            ptr->getSecretKey();
            cout<<endl;;
            break;
        }
        case 4:{
            cout<<"Length of new Secret Key: ";
            int len;
            cin>>len;
            ptr->updateSecret(len);
            cout<<endl<<"Secret Key Updated"<<endl;
            break;
        }
        case 0:
        return;
        
        
        default:
            break;
        }
        cout<<endl<<"--------------------------------------------"<<endl<<endl;
    }
}

void CeaserMenu(Ceaser *ptr){
    while(1){
        cout<<"1. Encrypt Text"<<endl;
        cout<<"2. Decrypt Text"<<endl;
        cout<<"3. Display Secret Key"<<endl;
        cout<<"4. Change Secret Key"<<endl;
        cout<<"5. Brute Force Decrypt"<<endl;
        cout<<"0. Exit"<<endl<<"Your Selection: ";
        int opt = 0;
        cin>>opt;
        cout<<endl;
        switch (opt)
        {
        case 1:{
            string plain;
            cout<<"Enter the String to be Encrypted:\nString: ";
            cin>>plain;
            cout<<"Encrypted Text: "<<ptr->Encrypt(plain)<<endl;;
            break;
        }
        case 2:{
            string enc;
            cout<<"Enter the String to be Decrypted:\nString: ";
            cin>>enc;
            cout<<"Encrypted Text: "<<ptr->Decrypt(enc)<<endl;;
            break;
        }
        case 3:{
            cout<<"Secret Key: "<< ptr->getSecretKey()<<endl;;
            break;
        }
        case 5:{
            string enc;
            cout<<"Enter the String for Brute Forece Decryption:\nString: ";
            cin>>enc;
            cout<<"Encrypted Text: ";
            ptr->BruteForece(enc);
            break;
        }
        case 4:{
            cout<<"Old Security Key: "<<ptr->getSecretKey()<<endl;
            cout<<"New Secret Key: ";
            int sec;
            cin>>sec;
            ptr->updateSecret(sec);
            cout<<endl<<"Secret Key Updated"<<endl;
            break;
        }
        case 0:
            return;
        
        default:
            break;
        }
        cout<<endl<<"--------------------------------------------"<<endl<<endl;
    }
}