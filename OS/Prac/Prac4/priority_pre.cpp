#include<iostream>
#include<pthread.h>
#include <queue>
#include<semaphore.h>
#include<ctime>
#include <unistd.h>

using namespace std;

typedef struct Gantt{
    char processID;
    Gantt *next;
}Gantt;

class Process{
    public:
    char ProcessID;
    int AT, BT, TAT, WT, FT, priority, RBT;

    Process(char id, int AT, int BT);
    Process(char id, int AT, int BT, int priority);
    Process(){};
    bool operator()(Process * const &obj1, Process * const &obj2);

};


class priority_pre{
    public:
    void static *processScheduler(void *i);
    void static displaySchedule();
    void static displayGantt();
};


Process *ps;
priority_queue <Process *, vector<Process *>, Process> Q;
sem_t s, print;
time_t start;
Gantt *head, *tail;

void *processGenerator(void *i);


int main(){

    start = time (NULL);

    sem_init(&s,0,1);
    sem_init(&print,0,1);

    pthread_t ps_gen, ps_sch;
    pthread_create(&ps_gen,NULL, processGenerator,(void *)0);
    pthread_create(&ps_sch,NULL, priority_pre::processScheduler,(void *)0);
    pthread_join(ps_gen,NULL);
    pthread_join(ps_sch,NULL);

    priority_pre::displaySchedule();
    
    priority_pre::displayGantt();

}


Process::Process(char id, int AT, int BT){
    this->ProcessID = id;
    this->AT = AT;
    this->BT = BT;
    this->RBT = BT;
}

Process::Process(char id, int AT, int BT, int priority){
    this->ProcessID = id;
    this->AT = AT;
    this->BT = BT;
    this->priority = priority;
    this->RBT = BT;
}

bool Process::operator()(Process * const &obj1, Process * const &obj2){
    if(obj1->priority==obj2->priority)
        return (obj1->AT >= obj2->AT);
    return (obj1->priority > obj2->priority);
}




void *processGenerator(void *i){
    time_t elapsed;
    int ATs[]={0,2,4,6,8};
    int Pri[]={4,3,2,1,5};
    int BTs[]={3,6,4,5,2};
    ps = (Process*) malloc(sizeof(Process)*10);
    for(int i = 0;i<5;i++){

        elapsed = time (NULL) - start;
        sleep(ATs[i]-elapsed);
        elapsed = time (NULL) - start;


        ps[i] = Process(i+'a', elapsed, BTs[i], Pri[i]);
        sem_wait(&s);
        Q.push(ps+i);
        sem_post(&s);
        sem_wait(&print);
        cout<<"Process "<<(char)(i+'a')<<" added\n";
        sem_post(&print);
    }
}

void *priority_pre::processScheduler(void *i){
    time_t elapsed;
    Process *p, *prev=NULL;
    int count = 0;
    Gantt *temp;
    while (1) {
        sem_wait(&s);
        if(!Q.empty() || prev!=NULL){
            if(!Q.empty()){
                p = Q.top();
                Q.pop();

                if(prev!=NULL && prev->RBT>0 && (prev->priority <= p->priority)){
                    Q.push(p);
                    p = prev;
                }
                else if(prev!=NULL && prev->RBT>0){
                    Q.push(prev);
                }
            }
            else{
                p = prev;
            }
            sem_post(&s);
            if(head == NULL){
                head = (Gantt*) malloc(sizeof(Gantt));
                head->processID = p->ProcessID;
                head->next=NULL;
                tail = head;
            }
            else{
                temp = (Gantt*) malloc(sizeof(Gantt));
                temp->processID = p->ProcessID;
                temp->next=NULL;
                tail->next = temp;
                tail = tail->next;
            }
            sleep(1);
            p->RBT--;
            if(p->RBT==0){
                count++;
                elapsed = time (NULL) - start;
                p->FT = elapsed;
                p->TAT = p->FT-p->AT;
                p->WT = p->TAT-p->BT;
                sem_wait(&print);
                cout<<"Process "<<p->ProcessID<<" completed\n";
                sem_post(&print);
                p=NULL;
            }
            if(count==5)
                break;
            prev = p;
            // cout<<count;
        }
        else{
            sem_post(&s);
        }
    }
    p=NULL;
    prev=NULL;
}

void priority_pre::displaySchedule(){
    cout<<endl<<"Priority Preemptive Schedule:\n";
    cout<<"----------------------------------------------------------\n";
    cout<<"|Process|  AT\t| Pri\t|  BT\t| Fin T\t|  TAT\t|  WT\t|\n";
    cout<<"----------------------------------------------------------\n";
    float avg_TAT=0, avg_WT=0;
    for(int i = 0;i<5;i++){
        cout << "|  "<<ps[i].ProcessID<<"\t|   "<<ps[i].AT<<"\t|   "<<ps[i].priority<< "\t|   " << ps[i].BT<< "\t|   " << ps[i].FT<< "\t|   " << ps[i].TAT<<"\t|   "<<ps[i].WT << "\t|\n";
        avg_TAT+=(float) ps[i].TAT;
        avg_WT+=(float) ps[i].WT;
    }
    cout<<"----------------------------------------------------------\n";
    cout<<"|\t\t\t\t\t|Avg TAT|Avg WT\t|\n";
    cout<<"|\t\t\t\t\t| "<<avg_TAT/10<<"\t| "<<avg_WT/10<<"\t|\n";
    cout<<"----------------------------------------------------------\n";
}

void priority_pre::displayGantt(){
    Gantt *temp;
    temp = head;
    cout<<endl;
    cout<<"Gantt Chart:\n";
    while (temp!=NULL)
    {
        cout<<"|  "<<temp->processID<<"  ";
        temp=temp->next;
    }
    cout<<"|"<<endl;
    temp = head;
    while (temp!=NULL)
    {
        cout<<"|-----";
        temp=temp->next;
    }
    cout<<"|"<<endl;
    int i=0;
    temp = head;
    while (temp!=NULL)
    {
        if(i<9)
        cout<<"|"<<i<<"   "<<++i;
        else if(i==9)
        cout<<"|"<<i<<"  "<<++i;
        else
        cout<<"|"<<i<<" "<<++i;
        temp=temp->next;
    }
    cout<<"|"<<endl;
    
}