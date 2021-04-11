#include<iostream>
#include<cstring>
#include<queue>
using namespace std;

typedef struct conn{
    int dest;
    int weight;
    conn *next;
}conn;



typedef class Node{

    public:
    int node_number;
    int cost;
    int from;
    conn *head, *tail;
    Node(int new_node);
    Node();
    ~Node();

    void AddConnection(int new_dest, int cost);
    bool operator()(Node * const &obj1, Node * const &obj2);
}Node;

typedef class Network{
    public:
    Node** nodes;
    int size;
    Network(int size);
    void AddNode(int src, int dest, int weight);
    void DisplayNetwork();
    void findPath(int src, int dest);
    void printPath(Node *tempNode, int src, int dest);
}Network;


int main(){
    Network net = Network(6);
    net.AddNode(1,4,1);
    net.AddNode(1,2,1);
    net.AddNode(2,4,1);
    net.AddNode(2,3,2);
    net.AddNode(4,3,1);
    net.AddNode(4,5,2);
    net.AddNode(4,6,15);
    net.AddNode(3,5,1);
    net.AddNode(3,6,1);
    net.AddNode(5,6,1);

    cout<<"The Network is:\n";
    net.DisplayNetwork();    
    cout<<endl<<endl;
    net.findPath(1,5);
    return 0;
}



Node::Node(int new_node){
    this->node_number = new_node;
    this->cost = INT8_MAX;
    this->tail = NULL;
    this->head = NULL;
}

Node::Node(){

}
Node::~Node(){

}

void Node::AddConnection(int new_dest, int weight){
    if(this->head == NULL){
        this->head = (conn*) malloc(sizeof(conn));
        this->head->dest=new_dest;
        this->head->weight = weight;
        this->head->next = NULL;
        this->tail = this->head;
    }
    else{
        conn *temp;
        temp = (conn*) malloc(sizeof(conn));
        temp->dest = new_dest;
        temp->weight = weight;
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

void Network::AddNode(int src, int dest, int weight){
    (*(nodes + src -1))->AddConnection(dest, weight);
    (*(nodes + dest -1))->AddConnection(src, weight);
}

void Network::DisplayNetwork(){
    for(int i = 0; i<this->size; i++){
        conn *temp;
        temp =  (*(this->nodes + i))->head;
        cout<<i+1<<" {";
        while(temp!=NULL){
            cout<<" "<<temp->dest<<"("<<temp->weight<<") ";
            temp = temp->next;
        }
        cout<<"}"<<endl;
    }
}



bool Node::operator()(Node * const &obj1, Node * const &obj2){
    return (obj1->cost > obj2->cost);
}

void Network::findPath(int src, int dest){
    Node *tempNode = (*(this->nodes + src -1));
    int *visited = (int*) malloc(sizeof(int)*this->size);
    memset(visited, 0, sizeof(visited));

    priority_queue <Node *, vector<Node *>, Node> Q;
    int count =0;
    cout<<"The path from: "<<src<<" to: "<<dest<<" is:\n";

    tempNode->cost = 0;
    Q.push(tempNode);
    while(visited[dest-1]!=1 && !Q.empty()){
        tempNode = Q.top();
        Q.pop();
        visited[tempNode->node_number-1] = 1;
        conn *t;
        t=tempNode->head;
        while(t!=NULL){
            int d = t->dest, w = t->weight;
            if(visited[d-1]==1){
                t = t->next;
                continue;
            }
            Node *t1 = (*(this->nodes + d -1));
            if(tempNode->cost+w < t1->cost){
                t1->cost = tempNode->cost+w;
                t1->from = tempNode->node_number;
            }
            Q.push(t1);
            t = t->next;
        }
        
    }

    if(visited[dest-1]==1){
        Node *tempNode = (*(this->nodes + dest -1));
        this->printPath(tempNode, src, dest);
        cout<<tempNode->node_number<<endl<<"Total cost: "<<tempNode->cost<<endl;

    }
}

void Network::printPath(Node *tempNode, int src, int dest){
        if(tempNode->node_number != src){
            this->printPath((*(this->nodes + tempNode->from -1)), src, dest);
            cout<<tempNode->from<<"->";
        }
}
