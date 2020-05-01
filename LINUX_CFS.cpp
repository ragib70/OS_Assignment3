#include <bits/stdc++.h>
#include<time.h>
#include<math.h>
using namespace std;
#define TARGET_LATENCY 20
#define MININUM_GRANULARITY 4
#define INC_COUNT 20
#define TIME_UNIT 1

int INITIAL_PROC_COUNT=0;
double CURRENT_RUN_TIME=0.0;
double INITIAL_WEIGHT_TOTAL=0.0;
static const double NICE_TO_WEIGHT[40] = {
 /* -20 */     88761,     71755,     56483,     46273,     36291,
 /* -15 */     29154,     23254,     18705,     14949,     11916,
 /* -10 */      9548,      7620,      6100,      4904,      3906,
 /*  -5 */      3121,      2501,      1991,      1586,      1277,
 /*   0 */      1024,       820,       655,       526,       423,
 /*   5 */       335,       272,       215,       172,       137,
 /*  10 */       110,        87,        70,        56,        45,
 /*  15 */        36,        29,        23,        18,        15,
};
struct Node
{
    double vrunTime;
    int PID;
    int burst;
    int arrival;// Time Instant when a process arrives.
    double turnaround;// Time taken for the completion of the process from the instant it was submitted.
    double wait;//Time spent waiting in the ready queue.
    double completion;// Timestamp at which a particular process execution completes.###from the point of view of CPU.
    int nice_value;
    double actRunTime;
    // Constructor
    Node(double vrunTime,int burst,int PID,int arrival,int nice_value)
    {
       this->burst=burst;
       this->vrunTime = vrunTime;
       this->actRunTime=0.0;
       this->wait=0.0;
       this->completion=0.0;
       this->PID=PID;
       this->arrival=arrival;
       this->nice_value=nice_value;

    }

};

multimap<double,Node*,less<double> > statMap;

double MIN_ARR_TIME=INT_MAX;


double initialize_randomly(int pcount,vector<Node*> *nodeVector){

    srand (time(NULL));
    default_random_engine arrival_generator, burst_generator,nice_generator;
    normal_distribution<double> arrival_distribution(6,10), burst_distribution(5,8);
    // uniform_int_distribution<int> arrival_distribution(20,25), burst_distribution(20,25);
    uniform_int_distribution<int> nice_distribution(-20,19);

    int pid, arrival, burst, nice;
    double weightTotal=0;
    cout<<"Process ID\t\tArrival Time\t\tBurst Time\t\tNice Value"<<endl;

    for (int i = 0; i < pcount; i++) {
      pid = i+1;
      // pid = i%20; // for recurring processes
      arrival = (int) arrival_distribution(arrival_generator) ;
      arrival = abs(arrival);

      burst =  (int) burst_distribution(burst_generator);
      burst = abs(burst);

      // added for MLQ
      nice = (int)nice_distribution(nice_generator);
      //nice=abs(nice);


      if(nice<(-20))
        nice=-19;
      if(arrival<MIN_ARR_TIME)
        MIN_ARR_TIME=arrival;
      nodeVector->push_back(new Node(0.0,burst,pid,arrival,nice));
      weightTotal+=(NICE_TO_WEIGHT[nice+INC_COUNT]);
      //P[i].PID = pid; P[i].arrival = arrival; P[i].burst = burst; P[i].priority=pr;
      cout<<pid<<"\t\t\t"<<arrival<<"\t\t\t"<<burst<<"\t\t\t"<<nice<<endl;
    }
    return weightTotal;
}

double initialize_manually(int pcount,vector<Node*> *nodeVector)
{
    double weightTotal=0;
     cout<<"Arrival Time\t\tBurst Time\t\tNice Value"<<endl;
  for(int i=0; i<pcount; i++){
    int tempPID,tempArrTime,tempBurst;
    int tempNiceValue;
    cin>>tempArrTime>>tempBurst>>tempNiceValue;
    if(tempNiceValue<-20 ||tempNiceValue>19)
    {
        cout<<"Incorrect nice value entered"<<endl;
        INITIAL_PROC_COUNT--;
        continue;

    }
    tempPID=i+1;
    if(tempArrTime<MIN_ARR_TIME)
        MIN_ARR_TIME=tempArrTime;
    nodeVector->push_back(new Node(0.0,tempBurst,tempPID,tempArrTime,tempNiceValue));
    weightTotal+=(NICE_TO_WEIGHT[tempNiceValue+INC_COUNT]);
  }
  return weightTotal;
}

int printNodeMap( multimap<double,Node*,less<double> >* nodeMap)
{
if(nodeMap->size()==0 )
    {
        if(statMap.size()==INITIAL_PROC_COUNT)
            cout<<"--------All processes completed-----\n";
        return 0;
    }
cout<<"-------------------****-----------------"<<endl;
cout<<"Current Status-------CURRENT_TIME:"<<CURRENT_RUN_TIME<<"ms---"<<endl;
multimap <double,Node*> :: iterator itr;
cout  <<  "\tPID \tVRUNTIME \tACT_RUNTIME \n";
int weightTotal=0;
for (itr = nodeMap->begin(); itr != nodeMap->end(); ++itr)
    {
        cout  <<  '\t' << (itr->second)->PID
              <<  '\t' << itr->first
              <<  "\t\t" << (itr->second)->actRunTime << '\n';
              weightTotal+=NICE_TO_WEIGHT[(itr->second)->nice_value+INC_COUNT];
    }
    return weightTotal;
}
double calcStdDev(multimap<double,Node*,less<double> >* nodeMap,double mean)
{
  multimap <double,Node*> :: iterator itr;
  double standardDev=0;
  double variance=0;
  int procCount=0;
for (itr = nodeMap->begin(); itr != nodeMap->end(); ++itr)
    {
    variance+=pow(((itr->second)->wait)-mean,2);
    procCount+=1;
    }
    variance=variance/procCount;
    standardDev=sqrt(variance);
    return standardDev;

}
void deleteByPID(multimap<double,Node*,less<double> >* nodeMap,int PID)
{
multimap <double,Node*> :: iterator itr;

for (itr = nodeMap->begin(); itr != nodeMap->end(); ++itr)
    {
        if(((itr->second)->PID)==PID)
        {
            nodeMap->erase(itr);
            return;
        }

    }
    cout <<"No process with this PID exists"<< endl;
}

Node *findMinimum(multimap<double,Node*,less<double> >* nodeMap)
{

    if(nodeMap->size()==0)
        return NULL;
multimap <double,Node*,less<double> > :: iterator itr=nodeMap->begin();Node *ptr=NULL;Node *tempPtr;
for (itr = nodeMap->begin(); itr != nodeMap->end(); ++itr)
    {
    if(ptr==NULL)
        {

             ptr=itr->second;
             ptr->vrunTime=itr->first;
        }
    else
        {
            tempPtr=itr->second;
            tempPtr->vrunTime=itr->first;
            if(tempPtr->vrunTime==ptr->vrunTime)
            {
                if(tempPtr->actRunTime<ptr->actRunTime)
                    {
                    ptr=tempPtr;
                    }

            }
            else
                break;
        }
    }
return ptr;
}
void deleteByKey(multimap<double,Node*,less<double> >* nodeMap,double key)
{
    nodeMap->erase(key);
}

vector<Node*> updateNodeMap(multimap<double,Node*,less<double> >* nodeMap,double SYS_TIME,vector<Node*> nodeVector,double minVrunTime)
{
multimap <double,Node*> :: iterator itr;
vector<Node*> tempNodeVector;
for (int i=0;i<nodeVector.size();i++)
    {
        Node *ptr=nodeVector[i];
        double arrival=ptr->arrival;
        if(arrival<=SYS_TIME)
        {
          nodeMap->insert(pair<double,Node*>(minVrunTime,ptr));
        }
        else
            tempNodeVector.push_back(ptr);
    }
    return tempNodeVector;
}

bool checkNewProc(vector<Node*> nodeVector)
{
vector<Node*> tempNodeVector;
bool newProcArrived=false;
for (int i=0;i<nodeVector.size();i++)
    {
        Node *ptr=nodeVector[i];
        double arrival=ptr->arrival;
        if(arrival<=CURRENT_RUN_TIME)
        {
          newProcArrived=true;
        }

    }
    return newProcArrived;
}
double runSchedule(double weightTotal,multimap<double,Node*,less<double> >* nodeMap,vector<Node*> nodeVector)
{
    int normCount=TARGET_LATENCY/MININUM_GRANULARITY;
    int schedulerPeriod=0;
    double schedulerRunTime=0;
    int procCount=nodeMap->size();
    if(procCount==0)
    {
      CURRENT_RUN_TIME+=TIME_UNIT;
      CURRENT_RUN_TIME=abs(CURRENT_RUN_TIME);
      return TIME_UNIT;
    }
    if(procCount<=normCount)
    {
    schedulerPeriod=TARGET_LATENCY;
    }
    else
    {
    schedulerPeriod=procCount*MININUM_GRANULARITY;
    }
    cout<<"\n----Scheduler Runtime--------CURRENT_TIME:"<<CURRENT_RUN_TIME<<"ms";
    cout<<"----Scheduler Period: "<<schedulerPeriod<<"----Proc Count: "<<procCount<<"-----"<<endl;
    while(1)
    {
        bool procCompleted=false;
        bool newProcFlag=false;
        Node *ptr=findMinimum(nodeMap);
        double vrunTime=ptr->vrunTime;
        double actRunTime=ptr->actRunTime;
        double timeSlice=0.0;
        timeSlice=schedulerPeriod*(NICE_TO_WEIGHT[ptr->nice_value+INC_COUNT]/weightTotal);
        double remTime=(ptr->burst)-(ptr->actRunTime);
        if(remTime<timeSlice)
        {
            timeSlice=remTime;
        }
        if(timeSlice>(schedulerPeriod-schedulerRunTime))
            timeSlice=schedulerPeriod-schedulerRunTime;
        Node *tempPtr = new Node(ptr->vrunTime,ptr->burst,ptr->PID,ptr->arrival,ptr->nice_value);
        tempPtr->wait=ptr->wait;
        if((ptr->actRunTime)==0)
            {
                tempPtr->wait=CURRENT_RUN_TIME-(ptr->arrival);
            }
        deleteByPID(nodeMap,ptr->PID);

        double remTimeSlice=timeSlice;double timeJump=0;
        timeSlice=0;
        while(remTimeSlice>0)
        {
        if(remTimeSlice>=MININUM_GRANULARITY)
            {
                timeJump=MININUM_GRANULARITY;
            }
        else
                timeJump=remTimeSlice;
        timeSlice+=timeJump;
        remTimeSlice-=timeJump;
        actRunTime+=timeJump;
        vrunTime+=timeJump*(1024/NICE_TO_WEIGHT[tempPtr->nice_value+INC_COUNT]);
        schedulerRunTime+=timeJump;
        CURRENT_RUN_TIME+=timeJump;
        tempPtr->vrunTime=vrunTime;
        tempPtr->actRunTime=actRunTime;
        if(checkNewProc(nodeVector))
           {
           newProcFlag=true;
           break;
           }
        }

        if((tempPtr->actRunTime)>=(tempPtr->burst))
            {
                procCompleted=true;
                tempPtr->completion=CURRENT_RUN_TIME;
                tempPtr->turnaround=tempPtr->completion-tempPtr->arrival;
                statMap.insert(pair<double,Node*>(tempPtr->PID,tempPtr));
            }
        else
        {
        nodeMap->insert(pair<double,Node*>(vrunTime,tempPtr));
        }
        if(procCompleted)
        {

            cout<<"Process="<<tempPtr->PID<<" Time_Slice="<<timeSlice<<" Act_Run_Time="<<actRunTime<<" Burst_Time="<<tempPtr->burst<<" Sched_Run_Time="<<schedulerRunTime<<endl;
            cout<<"******Process="<<tempPtr->PID<<" completed at "<<CURRENT_RUN_TIME<<"ms******"<<endl;
        }
        if(newProcFlag)
        {
            if(!procCompleted)
                cout<<"Process="<<tempPtr->PID<<" Time_Slice="<<timeSlice<<" Act_Run_Time="<<actRunTime<<" Burst_Time="<<tempPtr->burst<<" Sched_Run_Time="<<schedulerRunTime<<endl;
            cout<<"#########New process/processes entered run queue#####"<<endl;
            break;
        }
        if(schedulerRunTime>=schedulerPeriod)
           {
               if(!procCompleted)
                  cout<<"Process="<<tempPtr->PID<<" Time_Slice="<<timeSlice<<" Act_Run_Time="<<actRunTime<<" Burst_Time="<<tempPtr->burst<<" Sched_Run_Time="<<schedulerRunTime<<endl;
               break;
           }
        if(!procCompleted)
        {
            cout<<"Process="<<tempPtr->PID<<" Time_Slice="<<timeSlice<<" Act_Run_Time="<<actRunTime<<" Burst_Time="<<tempPtr->burst<<" Sched_Run_Time="<<schedulerRunTime<<endl;
        }
        if(nodeMap->size()==0)
            {
                cout<<"########No active running processes########"<<endl;
                break;
            }
    }
        cout<<endl;
        return schedulerRunTime;
}
void printStatMap(multimap<double,Node*,less<double> >* nodeMap)
{
multimap <double,Node*> :: iterator itr;
cout<<"PID\tNice\tArrival\t\tBurst\t\tWait\t\tTurnaround\t\tCompletion Time"<<endl;
int procCount=0;
double waitTime=0;
double turnaround=0;
for (itr = nodeMap->begin(); itr != nodeMap->end(); ++itr)
    {
        Node *ptr=itr->second;
        cout  <<  "" << ptr->PID
              <<  "\t"<<ptr->nice_value
              <<  "\t" << ptr->arrival
              <<  "\t\t" << ptr->burst
              <<  "\t\t" << (ptr->completion-ptr->arrival-ptr->burst)
              <<  "\t\t" << ptr->turnaround
              <<  "\t\t\t" << ptr->completion <<'\n';
        procCount+=1;
        waitTime+=(ptr->completion-ptr->arrival-ptr->burst);
        turnaround+=ptr->turnaround;
    }
    cout << endl;
cout<<"Total Processes: "<<procCount<<endl;
cout<<"Average Wait Time: "<<waitTime/procCount<<endl;
cout<<"Wait Time Standard Deviation: "<<calcStdDev(nodeMap,waitTime/procCount)<<endl;
cout<<"Average Turnaround Time: "<<turnaround/procCount<<endl;
}

bool compareProcs(Node *p1,Node *p2)
{
if(p1->arrival<p2->arrival)
    {   /*
        int PID=p1->PID;
        p1->PID=p2->PID;
        p2->PID=PID;*/
        return true;
    }
else
    return false;
}
void printVector(vector<Node*> nodeVector)
{
for (int i=0;i<nodeVector.size();i++)
    {
        Node *ptr=nodeVector[i];
        cout<<ptr->PID<<"::"<<ptr->arrival<<endl;

    }

}
int main()
{

    cout<<"Enter the No. of Processes ";
  int N;
  cin>>N;
  INITIAL_PROC_COUNT=N;
  vector<Node*> nodeVector;
 multimap<double,Node*,less<double> > nodeMap;
    double weightTotal=0.0;

    bool allProcInserted=false;double SYS_TIME=0;double minVrunTime=0.0;
    //cout<<"Choose Random Generator(0) or Manual Input(1) or Distribution Generator(2)"<<endl;
    cout<<"Choose Manual Input(1) or Distribution Generator(2)"<<endl;
  int option;
  cin>>option;
  if(option==1)
      weightTotal=initialize_manually(N,&nodeVector);
  else if(option==0){
      //AutoFill(P,N);
    }
  else if(option==2){
      weightTotal=initialize_randomly(N,&nodeVector);
  }
  else
      cout<<"Wrong Option choosen"<<endl;

    sort(nodeVector.begin(),nodeVector.end(),compareProcs);


    INITIAL_WEIGHT_TOTAL=weightTotal;
    SYS_TIME+=MIN_ARR_TIME;
    CURRENT_RUN_TIME+=MIN_ARR_TIME;
    nodeVector=updateNodeMap(&nodeMap,SYS_TIME,nodeVector,minVrunTime);
    weightTotal=printNodeMap(&nodeMap);
    while(statMap.size()!=INITIAL_PROC_COUNT)
    {
    SYS_TIME+=runSchedule(weightTotal,&nodeMap,nodeVector);
    Node *minPtr=findMinimum(&nodeMap);
    if(minPtr==NULL)
        minVrunTime=0.0;
    else
        minVrunTime=minPtr->vrunTime;
    nodeVector=updateNodeMap(&nodeMap,SYS_TIME,nodeVector,minVrunTime);
    weightTotal=printNodeMap(&nodeMap);


    }
    printStatMap(&statMap);
    return 0;


}
