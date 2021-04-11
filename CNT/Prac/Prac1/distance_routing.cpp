#include<iostream>
#include<cstring>
using namespace std;

typedef struct conn{
    int dest;
    int weight;
    conn *next;
}conn;

typedef struct nodeTable{
    int from, len;
    friend ostream& operator <<(ostream& stream, const nodeTable& table);
} nodeTable;


typedef class Node{

    public:
    int node_number;
    nodeTable *table;
    conn *head, *tail;
    Node(int new_node);
    // Node::Node(int new_dest, int weight);
    ~Node();

    void AddConnection(int new_dest);
    void CreateTable(int size);
    void DisplayTable(int size);
}Node;

typedef class Network{
    public:
    Node** nodes;
    int size;
    Network(int size);
    void AddNode(int src, int dest);
    void DisplayNetwork();
    void DisplayTables();
    void init();
    void exchangeTables();
    void findPath(int src, int dest);
}Network;


int main(){
    Network net = Network(6);
    net.AddNode(1,4);
    net.AddNode(1,2);
    net.AddNode(2,4);
    net.AddNode(2,3);
    net.AddNode(4,3);
    net.AddNode(4,5);
    net.AddNode(4,6);
    net.AddNode(3,5);
    net.AddNode(3,6);
    net.AddNode(5,6);

    cout<<"The Network is:\n";
    net.DisplayNetwork();
    net.init();
    net.exchangeTables();
    net.exchangeTables();
    net.exchangeTables();
    net.exchangeTables();
    net.exchangeTables();
    net.exchangeTables();
    net.exchangeTables();
    net.exchangeTables();
    net.exchangeTables();

    cout<<"The Routing Tables are:\n";
    net.DisplayTables();

    net.findPath(1,5);
    return 0;
}



Node::Node(int new_node){
    this->node_number = new_node;
    this->tail = NULL;
    this->head = NULL;
}

void Node::AddConnection(int new_dest){
    if(this->head == NULL){
        this->head = (conn*) malloc(sizeof(conn));
        this->head->dest=new_dest;
        this->head->next = NULL;
        this->tail = this->head;
    }
    else{
        conn *temp;
        temp = (conn*) malloc(sizeof(conn));
        temp->dest = new_dest;
        temp->next = NULL;
        this->tail->next = temp;
        this->tail = temp;
    }

}



Network::Network(int size){
    this->size = size;
    this->nodes = (Node**) malloc(sizeof(Node*)*size);
    for(int i = 0; i<this->size; i++){
        *(this->nodes+i) = new Node(i+1);
    }
}

void Network::AddNode(int src, int dest){
    (*(nodes + src -1))->AddConnection(dest);
    (*(nodes + dest -1))->AddConnection(src);
}

void Network::DisplayNetwork(){
    for(int i = 0; i<this->size; i++){
        conn *temp;
        temp =  (*(this->nodes + i))->head;
        cout<<i+1<<" {";
        while(temp!=NULL){
            cout<<temp->dest<<" ";
            temp = temp->next;
        }
        cout<<"}"<<endl;
    }
}

void Network::init(){
    for(int i = 0; i<this->size; i++){
        (*(this->nodes + i))->CreateTable(this->size);
    }
}

void Network::DisplayTables(){
    for(int i = 0; i<this->size; i++){
        cout<<"Table for node "<<i+1<<endl;
        cout<<"Node"<<"\t"<<"Dist"<<"\t"<<"From(Node)"<<endl;
        (*(this->nodes + i))->DisplayTable(this->size);
        cout<<endl;
    }
}

void Network::exchangeTables(){
    for(int i = 0;i<size;i++){
        Node *tempNode = (*(this->nodes + i));
        nodeTable *tempNodeTable = (*(this->nodes + i))->table;
        conn *tempConn = (*(this->nodes + i))->head;

        while(tempConn!=NULL){
            nodeTable *tempTable;
            tempTable = (*(this->nodes + (tempConn->dest) - 1 ))->table;
            for(int j = 0;j<size; j++){
                if( (tempNodeTable + j)->len > (tempTable + j)->len ){
                    (tempNodeTable + j)->len = (tempTable + j)->len +1;
                    (tempNodeTable + j)->from = tempConn->dest;
                }
            }
            tempConn = tempConn->next;
        }
    }
}

void Network::findPath(int src, int dest){
    Node *tempNode = (*(this->nodes + src -1));
    int count =0;
    cout<<"The path from: "<<src<<" to: "<<dest<<" is:\n";
    while(tempNode->node_number!=dest){
        cout<<tempNode->node_number<<" --> ";
        tempNode = (*(this->nodes + (tempNode->table + dest -1)->from -1) );
        count++;
    }
    cout<<dest<<endl;
}




void Node::CreateTable(int size){
    this->table = (nodeTable*) malloc(sizeof(nodeTable)*size);
    for(int i = 0;i< size;i++){
        (table+i)->len = INT16_MAX;
    }
    conn *temp;
    temp =  this->head;

    while(temp!=NULL){
        (table + temp->dest-1) -> len = 1;
        (table + temp->dest-1) -> from = temp->dest;
        temp = temp->next;
    }
    cout<<endl;
        (table + this->node_number-1) -> len = 0;
        (table + this->node_number-1) -> from = this->node_number;
    
}

void Node::DisplayTable(int size){
    for(int i = 0;i<size;i++){
        cout<< i+1<<"\t"<<*(table+i)<<endl;
    }
}



ostream& operator <<(ostream& stream, const nodeTable& table){
    stream<<table.len<<"\t"<<table.from;
}