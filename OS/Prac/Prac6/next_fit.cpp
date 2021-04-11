#include<iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef struct Process{
    int id, size, mem_blk;
}Process;

typedef struct Mem_Block{
    int start_loc, size, process_id, blk_number;
}Mem_Block;

bool Sort_Mem(const Mem_Block* a, const Mem_Block* b);
void next_fit(vector<Process> *processes, vector<Mem_Block*> *rem_mem_blocks, vector<Mem_Block*> *all_mem_blocks);
void get_input(vector<Process> *processes, vector<Mem_Block*> *rem_mem_blocks, vector<Mem_Block*> *all_mem_blocks);
void print_results(vector<Process> *processes,vector<Mem_Block*> *all_mem_blocks);

int main(){
    vector<Process> processes;
    vector<Mem_Block*> rem_mem_blocks;
    vector<Mem_Block*> all_mem_blocks;
    get_input(&processes, &rem_mem_blocks, &all_mem_blocks);

    next_fit(&processes, &rem_mem_blocks, &all_mem_blocks);

    

    print_results(&processes, &all_mem_blocks);

    cout<<endl;

}


bool Sort_Mem(const Mem_Block* a, const Mem_Block* b){
    return a->start_loc < b->start_loc;
}

void next_fit(vector<Process> *processes, vector<Mem_Block*> *rem_mem_blocks, vector<Mem_Block*> *all_mem_blocks){
    Mem_Block *temp2;
    auto mem = (*rem_mem_blocks).begin();
    for (auto pr = (*processes).begin(); pr != (*processes).end(); ++pr){
        for(int i = 0; i< (*processes).size(); i++){
            if((*mem)->size>=pr->size){
                pr->mem_blk = (*mem)->blk_number;
                if((*mem)->size>pr->size){
                    temp2 = (Mem_Block*) malloc(sizeof(Mem_Block));
                    temp2->start_loc = (*mem)->start_loc;
                    temp2->size = pr->size;
                    temp2->blk_number = (*mem)->blk_number;
                    temp2->process_id = pr->id;
                    (*all_mem_blocks).push_back(temp2);

                    (*mem)->start_loc += pr->size;
                    (*mem)->size -= pr->size;
                }
                else{
                    (*mem)->process_id = pr->id;
                    (*rem_mem_blocks).erase(mem);
                    mem++;
                    if(mem == (*rem_mem_blocks).end()){
                        mem = (*rem_mem_blocks).begin();
                    }
                }
                break;

            }
            mem++;
            if(mem == (*rem_mem_blocks).end()){
                mem = (*rem_mem_blocks).begin();
            }
        }
    sort((*all_mem_blocks).begin(), (*all_mem_blocks).end(), Sort_Mem);
    print_results(processes, all_mem_blocks);
    }

    sort((*all_mem_blocks).begin(), (*all_mem_blocks).end(), Sort_Mem);
}





void get_input(vector<Process> *processes, vector<Mem_Block*> *rem_mem_blocks, vector<Mem_Block*> *all_mem_blocks){
    Process *temp1;
    Mem_Block *temp2;
    int n,m;
    cout<<"Number of Processes: ";
    cin>>n;
    cout<<n<<endl;
    cout<<"Number of Memory Blocks: ";
    cin>>m;
    cout<<m<<endl;

    int last = 0;

    cout<<"\nProcess size:\n";
    for(int i = 1; i<=n; i++){
        cout<<"Process "<<i<<": ";
        temp1 = (Process*) malloc(sizeof(Process));
        temp1->id = i;
        cin>>temp1->size;
        cout<<temp1->size<<endl;
        temp1->mem_blk = -1;
        (*processes).push_back(*temp1);
    }

    cout<<"\nMemory Block size:\n";
    for(int i = 1; i<=m; i++){
        cout<<"Memory Block "<<i<<": ";
        temp2 = (Mem_Block*) malloc(sizeof(Mem_Block));
        temp2->start_loc = last;
        temp2->process_id = -1;
        temp2->blk_number = i;
        cin>>temp2->size;
        cout<<temp2->size<<endl;
        last+=temp2->size;
        (*rem_mem_blocks).push_back(temp2);
        (*all_mem_blocks).push_back(temp2);
    }
}

void print_results(vector<Process> *processes,vector<Mem_Block*> *all_mem_blocks){
    cout<<endl<<"Next Fit\nMemory allocation\n";
    for (auto pr = (*processes).begin(); pr != (*processes).end(); ++pr){
        if(pr->mem_blk!=-1)
        cout<<"Process "<<pr->id<<" allocated in Block Number "<<pr->mem_blk<<endl;
        else
        cout<<"Process "<<pr->id<<": Memory not allocated"<<endl;
    }
    cout<<endl<<"Memory Blocks:\n";
    int external_frag = 0;
    for (auto mem = (*all_mem_blocks).begin(); mem != (*all_mem_blocks).end(); ++mem){
        if((*mem)->process_id!=-1){
            cout<<"|  Pr "<<(*mem)->process_id<<"("<<(*mem)->size<<")"<<"(Blk "<<(*mem)->blk_number<<")   ";
        }
        else{
            cout<<"|  Free("<<(*mem)->size<<")"<<"(Blk "<<(*mem)->blk_number<<")  ";
            external_frag+=(*mem)->size;
        }
    }
    cout<<"|"<<endl<<endl;
    cout<<"External Fragmentation: "<<external_frag<<endl;
}