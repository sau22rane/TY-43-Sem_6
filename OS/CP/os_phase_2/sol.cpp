#include <iostream>
#include <cstring>
#include <fstream>
#include<time.h>
#include <unistd.h>
#include<bits/stdc++.h>
#define TIME_QUANTUM 10
#define N 40
#define SIZE 300
#define K 4
using namespace std;
enum TASK{
    IS , OS , LD , RT , WT 
};
enum STATUS{
    Loading , Loaded , Ready , Execute, InputOutput , Terminate
};
class PCB{
    public:
    int ID;
	int TTL; //total time limit
	int TLL; //total line limit 
	int TTC; //total time conuter
	int LLC; //line limit counter
    int TSC; // Time slice counter
    char F;
    int  P_COUNT,D_COUNT;
    int  D , P ,O, END;
    int flag = 0;
    int PTR ;
    short IC;
    string terminateMsg ;
    STATUS status;
    PCB(PCB* pcb)
    {
        ID = pcb->ID;
        TTL = pcb->TTL;
        TLL = pcb->TLL;
        TTC = pcb->TTC;
        LLC = pcb->LLC;
        TSC = pcb->TSC;
        F = pcb->F;D = pcb->D;
        P_COUNT= pcb->P_COUNT;
        D_COUNT= pcb->D_COUNT;
        P = pcb->P , END = pcb->END;O = pcb->O;
        status = pcb->status;
        PTR = pcb->PTR;IC = pcb->IC;
        terminateMsg = pcb->terminateMsg;
    }
    PCB()
    {
        TTL = TLL = TTC = LLC = TSC  = D = P_COUNT = D_COUNT = P = END= IC =O= ID = 0;
        F = 'P';
        PTR = -1;
        terminateMsg = ' ';
        status = STATUS::Loading;
    }
};

class CH{
    public:
    int flag;
    int value;
    int time;
    int total_time;
    CH()
    {
        flag = 0 ; value = 0 ; time =  0; total_time=0;
    }
};
typedef struct buffer{
    int status ; // 0 - empty , 1 - ibq , 2-obq
    char value[40];
    void initalize(){
        for(int i =  0 ; i<40 ; i++)
        {
            
            value[i] = ' ';
        }
    }
}Buffer;

class MOS{
    private:
    char MainMemory[SIZE][K],DrumMemory[SIZE+200][K], SS[SIZE+1000][K], IR[K], R[K];
    // buffer[N];
    int drum = 0,memory=0;
    short RA ;
    bool C;//toggle
    int SI = 0 , PI = 0 , TI = 0 ,IR1=0,IR2=0,IR3=0,IOI = 0;
    CH* ch = new CH[3];
    int count = 0;
    ifstream fi;
    ofstream fo;
    bool mark[30] ;
    int eof = 0;
    PCB* pcb = new PCB();
    TASK task = TASK::IS;
    int flag = 0 ;
    queue<Buffer*> emptyQ,inputfulBufferQ,outputfulBufferQ ;
    queue<PCB*> readyQ,loadQ,inputOutputQ,terminateQ;

    
    void read();
	void write();
    void terminate(int,int);
    int cardReader();
    void startExecution();
    void LR();
    void SR();
    void CR();
    void BT(PCB*);
    void IRI1();
    void IRI2();
    void IRI3();
    void masterMode();
    void store_bufferDrum(Buffer*);
    void store_buffer_DrumMM(PCB*);
    bool check_operand(char , char);
    void simulation();
    void startCH(int);
    void loadDrumMM(PCB*);
    void Map(PCB*,int);
    void execute();
    void round_robin();
    bool mainMeomoryAvailable();
    void drum_to_OutputBuffer(PCB*);
    void releaseAll(PCB*);

    public:
    MOS();
    void begin();
    void initialization();
    void show(char [][4],int m , int n);
    void close_all();
    int Allocate();
    
};


int main()
{
    MOS my_os ;
    my_os.begin();
    my_os.close_all();
    return 0;
}
void MOS::round_robin()
{
    if(readyQ.empty())
        return;
    PCB* temp = readyQ.front();
    readyQ.pop();
    readyQ.push(temp);

}
bool MOS::mainMeomoryAvailable(){
    for(int i =  0 ; i<30 ; i++)
    {
        if(!mark[i])
            return true;
    }
    return false;
}
void MOS::begin(){
    cout<<"----------Starting Operating System----------\n\n";
    initialization();
    IOI = 1;
    do
    {   cout<<"\nEntering Execute funtion\n";
        execute();
        cout<<"\nExiting Execute funtion\n";
        cout<<"-------------------------------------------------------------\n";
        cout<<"\nEntering Simulation funtion\n";
        simulation();
        cout<<"\nExiting Simulation funtion\n";
        cout<<"-------------------------------------------------------------\n";
        cout<<"\nEntering Master Mode funtion\n";
        masterMode();
        cout<<"\nExiting Master Mode funtion\n";
        cout<<"-------------------------------------------------------------\n";
        round_robin();
        cout<<"-----------while---------"<<"  EOF:"<<eof<<" ifbQ:"<<!inputfulBufferQ.empty()<<" ofbQ:"<< !outputfulBufferQ.empty()<<"  LoadQ:"<<!loadQ.empty()<<"  ReadyQ:" <<!readyQ.empty()<<"  I/O Q:" << !inputOutputQ.empty() <<"  TerminateQ:"<< !terminateQ.empty()<<endl;
        
    }while(eof!=1 ||!inputfulBufferQ.empty() || !outputfulBufferQ.empty() || !loadQ.empty() || !readyQ.empty() || !inputOutputQ.empty() || !terminateQ.empty());
    cout<<"Main Memory : "<<endl;
    show(MainMemory,300,4);
    cout<<endl<<"Drum Memory : "<<endl;
    show(DrumMemory,500,4);
}
void showBuffer(queue<Buffer* >   q){
    queue<Buffer* > newQueue = q;
    cout<<"Size is : "<<q.size()<<endl;
    while(!q.empty())
    {
        cout<<q.front()->value<<endl;
        q.pop();
    }
}

void MOS::initialization(){
    
    memory = 0;
    //initialize MainMemory
    for(int i = 0 ; i<SIZE ; i++)
        for(int j = 0 ; j<K; j++)
            MainMemory[i][j] = ' ';
    
    for(int i =  0 ; i<30 ; i++)
        mark[i] = false;

    for(int i =  0 ; i<K ; i++)
        R[i] = ' ';
    
    //initialize ebq
    for(int i =  0 ; i<10;i++)
    {
        Buffer* temp = new Buffer;
        temp->initalize();
        temp->status = 0;//empty
        emptyQ.push(temp);
    }
    //initalized drum
    for(int i =  0 ; i<50;i++)
    {
        for(int j =  0 ; j<K;j++)
        {
            DrumMemory[i][j] = ' ';
        }
    }
    ch[0].value = 1;ch[1].value = 2;ch[2].value=4;
    ch[0].total_time = 5;ch[1].total_time = 5;ch[2].total_time=2;
    
}
int MOS::Allocate()
{
	srand(time(0));
	int random = rand() % 30;
	while(mark[random])
	{
		random = rand() % 30;
	}
	mark[ random ] = true;
	return random;
}
bool MOS::check_operand(char a , char b)
{
    if(!( ( (a<='9'&&a>='0') || a==' ') && ( (b<='9'&&b>='0')|| b==' ') )){
        PI = 2;
        return false;
    }
    return true;
}
void MOS::Map(PCB* temp , int VA)
{
    int i = 0 , pte_i ;
	int pte = temp->PTR + (int)VA/10;
    string content;
    for(i = 0 ;i<K;i++)
    { 
        content+=MainMemory[pte][i];
    }
    content[i]='\0';
    if(content == "    ")
    {
        PI = 3;//Page Fault
        cout<<"Page fault=> Setting PI=3 : "<<endl;
        return;
    }
    pte_i = stoi(content);
	RA = pte_i*10 + VA%10;
}
int MOS::cardReader(){
    
    int i = 0;
    char c;
    //memset(buffer, '\0', sizeof(buffer));
    if(eof == 1 || emptyQ.empty())
    {
        cout<<"emptyQ is empty or eof = 1"<<endl;
        return 1;
    }
    Buffer* temp = emptyQ.front();
    emptyQ.pop();
    temp->status = 1; //ibq
    if(inputfulBufferQ.size() == 10)
    {
        cout<<"Input Buffer queue is full ! cant read";
        return 1;
    }
    if(fi.is_open()) {
        int count = 0;
        fi.get(c);
        if(c == '\n')
            return 0;
        temp->value[count++] = c;
        while(fi.good()){
            fi.get(c);
            //cout<<"Reading card from input to ibq : "<<c<<endl;
            temp->value[count++] = c;
            if(count == 40 || c =='\n'){
                
                if(temp->value[0] == '$')
                {
                    if(temp->value[1] == 'A')
                    {
                        pcb = new PCB();
                        pcb->ID = stoi(string(1,temp->value[4])+string(1,temp->value[5])+string(1,temp->value[6])+string(1,temp->value[7]));
                        pcb->TTL = stoi(string(1,temp->value[8])+string(1,temp->value[9])+string(1,temp->value[10])+string(1,temp->value[11]));
                        pcb->TLL = stoi(string(1,temp->value[12])+string(1,temp->value[13])+string(1,temp->value[14])+string(1,temp->value[15]));
                        pcb->TTC = 0;
                        pcb->LLC = 0;
                        pcb->F = 'P';
                        cout<<"$AMJ PCB created for job id : "<<pcb->ID<<" "<<pcb->F<<endl; 
                        cout<<"PCB=> TTL:"<<pcb->TTL<<" TLL:"<<pcb->TLL<<" TTC:"<<pcb->TTC<<" LLC:"<<pcb->LLC<<endl;
              
                    }
                    else if(temp->value[1] == 'D'){
                        pcb->F = 'D';
                        cout<<"$DTA Encountered "<<endl;   
                    }
                }
                
                inputfulBufferQ.push(temp);
                if(!( (IOI >> 2) & 1))
                {
                    IOI+=4; //start cannel 3
                   
                }
              
                task = TASK::IS;
                cout<<"Added an inputful buffer with ibq size : "<<inputfulBufferQ.size()<<" IOI : "<<IOI<<endl;
                cout<<"Inputful buffer with value "<<temp->value<<endl; 

                cout<<endl<<"IBQ looks like : "<<endl;
                showBuffer(inputfulBufferQ);               
                return 0;
            }
            if(c =='H' && pcb->F == 'P'){
                temp->value[count++] = ' ';
                temp->value[count++] = ' ';
                temp->value[count++] = ' ';
            }

            if(fi.eof() && flag == 0)
            {
                inputfulBufferQ.push(temp);
                task = TASK::IS;
                 cout<<"Added an inputful buffer with ibq size : "<<inputfulBufferQ.size()<<" IOI : "<<IOI<<endl;
                cout<<"Inputful buffer with value "<<temp->value<<endl; 
                cout<<endl<<"IBQ looks like : "<<endl;
                showBuffer(inputfulBufferQ);   
                
                cout<<"found-------------"<<endl;
                flag = 1;
            }
        

        }
    }
    return 1;
}
void MOS::show(char data[][4],int m , int n){
    for(int i = 0 ; i<m ; i++)
    {
        if(i%10==0){
            cout<<endl;
            cout<<i<<'\t';
        }
        for(int j = 0 ; j<n ; j++)
        {
            if(data[i][j]==' ' || data[i][j]=='\n')
                cout<<'_';
            else
                cout<<data[i][j];
        }
        cout<<'\t';
        
    }
    cout<<endl;
}
void MOS::store_buffer_DrumMM(PCB* temp){

    //allocate a block and update PT
    int m = Allocate()*10;
    //cout<<"Program card : "<<memory<<endl;
    int itr = temp->PTR;
    while(MainMemory[itr][3] != ' ')
    {
        itr++;
    }
    MainMemory[itr][3] = '0'+ (m%100)/10;  
    MainMemory[itr][2] = '0'+ (m/100);
    MainMemory[itr][1] = '0';
    MainMemory[itr][0] = '0';


    for(int i = 0 ; i< 10 ; i++){
        for(int j = 0 ; j<K ; j++){
            MainMemory[m+i][j] = DrumMemory[temp->P+i][j];   
        }
    }
    temp->P+=10;
}
void MOS::loadDrumMM(PCB* temp){
    if(temp->PTR == -1)
    {
        temp->PTR = Allocate()*10;
    } 
    store_buffer_DrumMM(temp);         
}
void MOS::execute(){
    //cout<<"---------execute---------"<<endl;
    if(readyQ.empty())
    {
        cout<<"No process in ready queue\n";
        return;
    }
    PCB* temp = readyQ.front();
    cout<<"\nExecuting Process: "<<temp->ID<<endl;
    if(temp->status != STATUS::Execute)
    {
        temp->status = STATUS::Execute;
    }
    temp->TSC = 0;
    while( (temp->TSC != TIME_QUANTUM) || (temp->IC != (temp->D)) )
    {
        temp->TSC++;
        temp->TTC++;
        if(temp->TTC > temp->TTL)
        {
            cout<<"Total Time Limit Exceeded"<<endl;
            TI = 2; //Total Time Limit Exceeded     
        }
        //map IC to RA
        Map(temp,temp->IC);
        if(PI)
        {
            break;      
        }
        cout<<"Mapped IC to RA=> "<<temp->IC<<" : "<<RA<<endl;
        //initilaize IR
        for(int k = 0 ; k<K ; k++){
            IR[k] = MainMemory[RA][k];
        }
        if(IR[0]=='H' && IR[1]==' ')
        {
            //cout<<"H called"<<endl;
            SI = 3;
            break;
        }
        //inc IC
        temp->IC++;
        if(!check_operand(IR[2],IR[3])){
            break;
        }
        int operand = stoi(string(1,IR[2])+string(1,IR[3]));
        Map(temp,operand);
        if(PI)
        {
            break;          
        }
        // cout<<"OPERAND TO RA : "<<operand<<" : "<<RA<<endl;
        string instruction =  string(1,IR[0])+string(1, IR[1]);
        cout<<"-------Evaluating Instruction-------------"<<instruction<<operand<<endl;
        if(instruction == "LR")
        {
            LR();               
        }               
        else if(instruction == "SR")
        {
            SR();            
        }
        else if(instruction == "CR")
        {
            CR();          
        }
        else if(instruction == "BT")
        {
            BT(temp);
        }
        else if(instruction == "GD")
        {
            cout<<"Setting SI=>1\n";
            SI = 1;
            task = TASK::RT;
            return;
        }
        else if(instruction == "PD")
        {
            cout<<"Setting SI=>2\n";
            SI = 2;
            task = TASK::WT;
            return;          
        } 
        else if(instruction[0] == 'H')  
        {
            cout<<"Setting SI=>3\n";
            //cout<<"H called"<<endl;
            SI=3;
            task = TASK::OS; 
            return;            
        }
        else
        {
            PI = 1; //operation error
            return;
        } 
        cout<<" end of if "<<endl;
    }
    if(temp->TSC == TIME_QUANTUM)
    {
        TI = 1; //Time Slice Out 
    }
}


void MOS::LR(){
    for(int i = 0 ; i<K ; i++){
        R[i] = MainMemory[RA][i];
    }
    
}

void MOS::SR(){
    for(int i = 0 ; i<K ; i++){
        MainMemory[RA][i] = R[i];
    }
}

void MOS::CR(){
    C = true;
    for(int i = 0 ; i<K ; i++){
        if (MainMemory[RA][i] != R[i]){
            C = false;
            return;
        }
    }
}

void MOS::BT(PCB* temp){
    if(C){
        temp->IC = (IR[2]-'0')*10+(IR[3]-'0');
    } 
}

void MOS::drum_to_OutputBuffer(PCB* temp){
    if(emptyQ.empty() || temp->LLC < 0)
        return;
    Buffer* obq ;
    if(temp->LLC == 0)
    {
        cout<<"All output lines done "<<temp->terminateMsg<<emptyQ.size()<<endl;
        terminateQ.pop();
        if(!emptyQ.empty())
        {
            obq = emptyQ.front();
            obq->status = 2;
            for(int i =  0 ; i<40;i++)
            {
                obq->value[i] = temp->terminateMsg[i];
            }
            
            outputfulBufferQ.push(obq);
            cout<<"Outputful buffer is : "<<endl;
            showBuffer(outputfulBufferQ);
            emptyQ.pop();
        }
        if(emptyQ.size() >=2)
        {
                obq = emptyQ.front();
                emptyQ.pop();
                obq->status = 2;
                outputfulBufferQ.push(obq);
                obq = emptyQ.front();
                emptyQ.pop();
                obq->status = 2;
                outputfulBufferQ.push(obq);
        }
        return;
    }
    obq = emptyQ.front();
    emptyQ.pop();
    obq->status = 2;
    int k = 0;
    for(int i =  0 ; i<10;i++)
    {
        for(int j =  0 ; j<K;j++)
        {
            obq->value[k++] = DrumMemory[temp->O+i][j];
            //free track
            //DrumMemory[temp->O+i][j] = ' ';
        }
    }
    outputfulBufferQ.push(obq);
    temp->O+=10;
    temp->LLC--;
    cout<<"OutputfulBufferQ looks like : "<<temp->LLC <<endl;
    showBuffer(outputfulBufferQ);

}

void MOS::releaseAll(PCB* temp)
{
    //free(temp);
}
//doubt
void MOS::IRI1(){
    cout<<"\nInside IR1"<<endl;
    eof = cardReader();
    cout<<"eof: "<<eof<<endl;
    if(eof !=1 && !emptyQ.empty())
    {
        cout<<"Starting Channel 1\n";
        startCH(0); //channel 1
    }
}
void MOS::IRI2(){
    if(outputfulBufferQ.empty())
    return;
    Buffer* temp = outputfulBufferQ.front();
    cout<<"Inside IRI2 for : "<<temp->value<<" , OBQ size : "<<outputfulBufferQ.size() <<endl;
    outputfulBufferQ.pop();
    temp->value[39] = '\n';
    fo<<temp->value;
    temp->status = 0;
    temp->initalize();
    emptyQ.push(temp);
    cout<<"Starting Channel 2\n";
    startCH(1);
}
void MOS::IRI3(){
    cout<<"Inside IRI3 with TASK : "<<task<<endl;
    switch (task)
    {
    case TASK::IS:
    {
        
        if(inputfulBufferQ.empty())
        {
            break;
        }
        if(drum<=490)
        {
            cout<<"Starting Channel 3, ibq size : "<<inputfulBufferQ.size()<<endl;
            startCH(2);
        }
        cout<<"\nInputful Buffer q rn looks : "<<endl;
        showBuffer(inputfulBufferQ);
        Buffer* temp = inputfulBufferQ.front();
        inputfulBufferQ.pop();
        cout<<"Channel 3 performs IS on : "<<temp->value<<endl;
        if(temp->value[0] == '$')
        {
            cout<<"Encountered Control card: ";
            if(temp->value[1] == 'A')
            {
                cout<<"$AMJ\n\nLoading new Job into Drum....!!!\n";
                pcb->F = 'P';
                pcb->P = drum;
            }
            else if(temp->value[1] == 'D'){
                pcb->F = 'D';
                pcb->D = drum;
                cout<<"$DTA"<<endl;   
            }
            else //$END
            {
                pcb->O = drum;
                pcb->END = drum - 10;
                pcb->status = STATUS::Loaded;
                PCB* temp = new PCB(pcb);
                loadQ.push(temp);
                task = TASK::LD;
                cout<<"$END"<<endl<<endl; 
                cout<<"PCB Values of Job id : "<<pcb->ID<<endl;
                cout<<"Start address of P , D , O : "<<pcb->P<<" , "<<pcb->D<<" , "<<pcb->O<<endl;
                cout<<"End address : "<<pcb->END<<endl;
                cout<<"Start P_count , D_count : "<<pcb->P_COUNT<<" , "<<pcb->D_COUNT<<endl;
                drum += (pcb->TLL)*10; 

                cout<<"\nJob added Load queue\n";
                cout<<"-------------------------------------------------------------\n";
                cout<<endl<<"Drum Memory:";
                show(DrumMemory,500,4);
                cout<<"-------------------------------------------------------------\n";
                cout<<endl<<"Main Memory:";
                show(MainMemory,300,4);
                cout<<"-------------------------------------------------------------\n";
            }
        }
        else
        {
            // cout<<"prgm or data card "<<pcb->F<<" : "<<temp->value<<endl;
            store_bufferDrum(temp);
            if(pcb->F == 'P') //program card
            {            
                cout<<"Encountered Program card.\n";
                pcb->P_COUNT +=1;              
            }
            else if(pcb->F == 'D')//data card
            {
                cout<<"Encountered Data card.\n";
                pcb->D_COUNT +=1;
            }
            else
            {
                cout<<"Invalid value of F"<<endl;
            }
        }
        temp->status = 0;
        temp->initalize();
        emptyQ.push(temp);
    break;
    }
    case TASK::LD:
    {
        if(loadQ.empty())
            break;
        PCB* temp = loadQ.front();
        loadDrumMM(temp);
        temp->P_COUNT--;
        if(temp->P_COUNT == 0)
        {
            temp->status = STATUS::Ready;
            readyQ.push(temp);
            loadQ.pop();
            cout<<"Job loaded of id : "<<temp->ID<<endl;
            // show(DrumMemory,500,4);
        }
    break;
    }
    case TASK::RT:
    {
        
        if(inputOutputQ.empty())
            break;
        
        PCB* temp = inputOutputQ.front();
        cout<<"PCB Values of Job id : "<<temp->ID<<endl<<endl;
        cout<<"Start address of P , D , O : "<<temp->P<<" , "<<temp->D<<" , "<<temp->O<<endl;
        cout<<"End address : "<<temp->END<<endl;
        cout<<"Etart P_count , D_count : "<<temp->P_COUNT<<" , "<<temp->D_COUNT<<endl;
        if(temp->D_COUNT == 0)
        {
            cout<<"EM:"<<1<<"Out of Data Error=>Adding to Terminate queue"<<endl;
            terminateQ.push(temp);
            inputOutputQ.pop();
            
            terminate(1,-1);

            break;
        }
        // cout<<"hey reading !!"<<endl;
        for(int i = 0 ; i< 10 ; i++){
            for(int j = 0 ; j<K ; j++){
                MainMemory[RA+i][j] = DrumMemory[temp->D+i][j];
            }
        }
        temp->D+=10;
        temp->D_COUNT--;
        temp->TSC = 0;
        readyQ.push(temp);
        inputOutputQ.pop();
        cout<<"Data read successfully\nProcess added to ready queue\n";
    break;
    }
    case TASK::WT:
    {
        if(inputOutputQ.empty())
            break;
        PCB* temp = inputOutputQ.front();
        temp->LLC++;
        if(temp->LLC > temp->TLL)
        {
            cout<<"EM:"<<2<<" Line Limit Error=> Terminating process"<<endl<<endl;
            terminateQ.push(temp);
            inputOutputQ.pop();
            terminate(2,-1);
            break;
        }
         cout<<"hey WRITING!!"<<endl;
        for(int i = 0 ; i< 10 ; i++){
            for(int j = 0 ; j<K ; j++){
                DrumMemory[temp->END+10+i][j] = MainMemory[RA+i][j];
            }
        }
        temp->END+=10;
        temp->TSC = 0;
        readyQ.push(temp);
        inputOutputQ.pop();
        cout<<"Data written successfully\nProcess added to ready queue\n";
    break;   
    }
    case TASK::OS:
    {
        if(terminateQ.empty())
            break;
        PCB* temp = terminateQ.front();
        drum_to_OutputBuffer(temp);
        releaseAll(temp);
        if(!( (IOI >> 1) & 1))
        {
            IOI+=2;
        }

    break;   
    }
    default:
    {
        break;
    }
    }


    //priority wise
    if(!inputfulBufferQ.empty() && drum <=490)
    {
        //cout<<"start channel 3 #############"<<endl;
        task = TASK::IS;
        //startCH(2);
    }
    // if(!inputOutputQ.empty())
    // {
    //     //cout<<"start channel 3 #############"<<endl;
    //     task = TASK::RT;
    //     startCH(2);
    // }

}
void MOS::masterMode(){
    cout<<"Inside master mode with PI : "<<PI<<" : SI : "<<SI<<" : TI : "<<TI<<" IOI : "<<IOI<<endl;
    
    if(!readyQ.empty() &&( PI || SI || TI))
    {
        // cout<<"heya !"<<endl;
    PCB* temp = readyQ.front();
    if(TI == 0 || TI == 1)
    {
        // CASE TI=0 AND PI
        if(PI == 1)
        {
            cout<<"EM:"<<4<<" Operation Error=> Terminating process\n\n";
            terminateQ.push(temp);
            readyQ.pop();
            terminate(4,-1);
        }
        else if(PI == 2)
        {
            cout<<"EM:"<<5<<" Operand Error=> Terminating process\n\n";
            terminateQ.push(temp);
            readyQ.pop();
            terminate(5,-1);
        }
        else if(PI == 3)
        {
            string instruction = string(1,IR[0])+string(1, IR[1]);
            if(instruction=="PD" || instruction=="LR" || instruction=="CR") //invalid page fault
            {
                cout<<"EM:"<<6<<" Invalid Page Fault=> Terminating process\n\n";
                terminateQ.push(temp);
                readyQ.pop();
                terminate(6,-1);      
            }                     
            else
            {
                //Page Fault
                if(!mainMeomoryAvailable())
                {
                    cout<<"-------------Main Memory is full------"<<endl;
                    exit(0);
                    return;
                }
                int m = Allocate()*10;
                // cout<<"updated page table with : "<<m<<" of job id : "<<temp->ID<<endl;
                // int itr = temp->PTR;
                // while(MainMemory[itr][3] != ' ')
                // {
                //     itr++;
                // }
                int itr = temp->PTR+stoi(string(1,IR[2])+string(1, IR[3]))/10;
                cout<<"Valid Page Fault.\nAllocating page "<<(itr-temp->PTR)<<" to frame "<<m<<endl<<endl;
                //updated page table
                MainMemory[itr][3] = '0'+ (m%100)/10;  
                MainMemory[itr][2] = '0'+ (m/100);
                MainMemory[itr][1] = '0';
                MainMemory[itr][0] = '0';  
                temp->IC--;
                PI = 0;
                // cout<<" pAGE FAULT HANDLED ! "<<temp->IC<<endl;
                return;        

            }            	    
        }
        // CASE TI=0 AND SI
        else if(SI == 1)
        {
            inputOutputQ.push(temp);
            readyQ.pop();
            task = TASK::RT;
            cout<<"SI = 1 "<<inputOutputQ.empty()<<endl;
            SI = 0;
            //executeUserProgram();            
        }
        else if(SI == 2)
        {
            inputOutputQ.push(temp);
            readyQ.pop();
            task = TASK::WT;
            cout<<"SI = 1 "<<inputOutputQ.empty()<<endl;
            SI = 0;
            //executeUserProgram();         
        }
        else if(SI == 3)
        {
            cout<<"Job Completed Successfully\n\n";
            terminateQ.push(temp);
            readyQ.pop();
            terminate(0,-1);   
            SI = 0;
        }   

    }
    else
    {
        cout<<"EM:"<<3<<" Time Limit Exceeded=> Terminating process\n\n";
        // CASE TI=2 AND PI
        if(PI == 1)
        {
            terminateQ.push(temp);
            readyQ.pop();
            terminate(3,4);      
        }
        else if(PI == 2)
        {
            terminateQ.push(temp);
            readyQ.pop();
            terminate(3,5);      
        }        
        else if(PI ==3)
        {
            terminateQ.push(temp);
            readyQ.pop();
            terminate(3,-1);      
        }        
        // CASE TI=2 AND SI
        else if(SI == 1)
        {
            terminateQ.push(temp);
            readyQ.pop();
            terminate(3,-1);      
        }          
        else if(SI == 2)
        {
            //write
            // inputOutputQ.push(temp);
            // readyQ.pop();
            // task = TASK::WT;
            //terminate
            terminateQ.push(temp);
            terminate(3,-1);                      
        }
        else if(SI == 3)
        {
            terminateQ.push(temp);
            readyQ.pop();
            terminate(0,-1);          
        }
        // else
        //     terminate(3,-1);

    }  
    }
    //IOI
        
    
    if( (IOI >> 2) & 1)
        IRI3();
    if( (IOI >> 1) & 1)
        IRI2();
    if( (IOI >> 0) & 1)
        IRI1();

}

void MOS::store_bufferDrum(Buffer* temp){
    int k= 0;
    if(drum == 490)
    {
        cout<<"DrumMemory is full !"<<endl;
        return;
    }
    cout<<"storing at : "<<drum<<endl; 
    for(int i = 0 ; i< 10 ; i++){
        for(int j = 0 ; j<K ; j++){
            if(temp->value[k]=='\0')
                break;
            DrumMemory[drum+i][j] = temp->value[k++];   
        }
    }
    drum+=10;
    
}

MOS::MOS(){
    fi.open("input.txt",ios::in); 
    fo.open("output.txt",ios::out); 
    memset(DrumMemory,' ', sizeof(DrumMemory));
}

void MOS::close_all(){
    fi.close();
    fo.close();
}

void MOS::terminate(int EM1 , int EM2){
    task = TASK::OS; 
    if(terminateQ.empty())
        return;
    PCB* temp = terminateQ.front();
    string msg = "Terminated due to ";
    if(EM1 == 0)
        msg = "Terminated Succesfully";
    else if(EM1 == 1)
        msg+="Out of Data Error";
    else if(EM1 == 2)
        msg+="Line Limit Error";
     else if(EM1 == 6)
        msg+="Invalid Page Fault";
     else if(EM1 == 4)
        msg+="Operation Code Error";
     else if(EM1 == 5)
        msg+="Operand Error";
     else {
        msg+= "Time Limit Exceeded Error & ";
        if(EM2 == -1)
        msg+="Time Limit Exceeded Error";
        else if(EM2 == 4)
            msg+=" Operation Code Error";
        else 
            msg+=" Operand Error";      
     }
     temp->terminateMsg = msg;
     PI = 0 ; SI = 0 ; TI = 0;
} 

void MOS::simulation()
{

    cout<<"Inside simulation"<<endl;
    for(int i =  0 ; i<3 ; i++)
    {
        if(ch[i].flag)
        {        
            ch[i].time++;
            cout<<"FLag of channel "<<i<<" is busy and has time of : "<<ch[i].time<<endl;

            if(ch[i].time == ch[i].total_time)
            {
                    cout<<"Channel "<<i<<" finsihed"<<endl;

                IOI+= ch[i].value;
                ch[i].flag = 0;
                //channel complete interrupt
            }
        }
    }
}

void MOS::startCH(int channel){
    
    IOI -= ch[channel].value;
    ch[channel].time = 0;
    ch[channel].flag = 1; //busy
    cout<<"Start Channel : "<<channel<<" IOI "<<IOI<<endl;

}