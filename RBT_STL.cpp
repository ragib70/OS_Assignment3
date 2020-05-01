
/** C++ implementation for Red-Black Tree Insertion
   This code is adopted from the code provided by
   Dinesh Khandelwal in comments **/
#include <bits/stdc++.h>
using namespace std;
#define TARGET_LATENCY 20
#define MININUM_GRANULARITY 4
#define INC_COUNT 20

int INITIAL_PROC_COUNT=0;
double CURRENT_RUN_TIME=0.0;
double INITIAL_WEIGHT_TOTAL=0.0;
static const double prio_to_weight[40] = {
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
    Node *left, *right, *parent;
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
       left = right = parent = NULL;
 \
    }

};

multimap<double,Node*,less<double> > statMap;

int printNodeMap( multimap<double,Node*,less<double> >* nodeMap)
{
if(nodeMap->size()==0 )
    {
        if(statMap.size()==INITIAL_PROC_COUNT)
            cout<<"--------All processes completed-----\n";
        return 0;
    }

cout<<"Current Status: "<<endl;
multimap <double,Node*> :: iterator itr;
cout  <<  "\tPID \tVRUNTIME \tACT_RUNTIME \n";
int weightTotal=0;
for (itr = nodeMap->begin(); itr != nodeMap->end(); ++itr)
    {
        cout  <<  '\t' << (itr->second)->PID
              <<  '\t' << itr->first
              <<  "\t\t" << (itr->second)->actRunTime << '\n';
              weightTotal+=prio_to_weight[(itr->second)->nice_value+INC_COUNT];
    }
    cout << endl;
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
    variance+=pow((itr->second)->wait-mean,2);
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
multimap <double,Node*,less<double> > :: iterator itr=nodeMap->begin();
return itr->second;
}
void deleteByKey(multimap<double,Node*,less<double> >* nodeMap,double key)
{
    nodeMap->erase(key);
}
double runSchedule(double weightTotal,multimap<double,Node*,less<double> >* nodeMap)
{   //cout<<"weight_total: "<<weightTotal<<endl;
    int normCount=TARGET_LATENCY/MININUM_GRANULARITY;
    int schedulerPeriod=0;
    double schedulerRunTime=0;
    int procCount=nodeMap->size();
    if(procCount==0)
    {
      CURRENT_RUN_TIME+=TARGET_LATENCY;
      return TARGET_LATENCY;
    }
    if(procCount<=normCount)
    {
    schedulerPeriod=TARGET_LATENCY;
    }
    else
    {
    schedulerPeriod=procCount*MININUM_GRANULARITY;
    }
    cout<<"----Scheduler Runtime--------";
    cout<<"Scheduler Period: "<<schedulerPeriod<<"----Proc Count: "<<procCount<<"-----"<<endl;
    while(1)
    {
        //cout<<"CURRENT_RUN_TIME: "<<CURRENT_RUN_TIME<<endl;
        if(nodeMap->size()==0)
            break;
        Node *ptr=findMinimum(nodeMap);

        //cout<<"minimum: "<<ptr->vrunTime<<" id: "<<ptr->PID<<endl;
        double vrunTime=ptr->vrunTime;
        double actRunTime=ptr->actRunTime;
        double timeSlice=0.0;
        timeSlice=schedulerPeriod*(prio_to_weight[ptr->nice_value+INC_COUNT]/weightTotal);
        double remTime=(ptr->burst)-(ptr->actRunTime);
        if(remTime<timeSlice)
        {
            timeSlice=remTime;
            //schedulerPeriod+=(timeSlice-remTime);
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

        actRunTime+=timeSlice;
        vrunTime=actRunTime*(1024/prio_to_weight[ptr->nice_value+INC_COUNT]);
        schedulerRunTime+=timeSlice;
        CURRENT_RUN_TIME+=timeSlice;
        cout<<"Process="<<tempPtr->PID<<" Time_Slice="<<timeSlice<<" Act_Run_Time="<<actRunTime<<endl;
        tempPtr->vrunTime=vrunTime;
        tempPtr->actRunTime=actRunTime;
        if((tempPtr->actRunTime)>=(tempPtr->burst))
            {
                cout<<"******Process="<<tempPtr->PID<<" completed at "<<CURRENT_RUN_TIME<<"ms******"<<endl;
                tempPtr->completion=CURRENT_RUN_TIME;
                tempPtr->turnaround=tempPtr->completion-tempPtr->arrival;
                statMap.insert(pair<double,Node*>(tempPtr->PID,tempPtr));
            }
        else
        {
        nodeMap->insert(pair<double,Node*>(vrunTime,tempPtr));
        }
        if(schedulerRunTime>=schedulerPeriod)
            break;
    }
        cout<<endl;
        return schedulerPeriod;
}
void printStatMap(multimap<double,Node*,less<double> >* nodeMap)
{
multimap <double,Node*> :: iterator itr;
cout<<"\t\tPID\t\tBurst Time\t\tWait Time\t\tTurnaround Time\t\tCompletion Time"<<endl;
int procCount=0;
double waitTime=0;
double turnaround=0;
for (itr = nodeMap->begin(); itr != nodeMap->end(); ++itr)
    {
        Node *ptr=itr->second;
        cout  <<  "\t\t" << ptr->PID
              <<  "\t\t" << ptr->burst
              <<  "\t\t\t" << ptr->wait
              <<  "\t\t\t" << ptr->turnaround
              <<  "\t\t\t" << ptr->completion <<'\n';
        procCount+=1;
        waitTime+=ptr->wait;
        turnaround+=ptr->turnaround;
    }
    cout << endl;
cout<<"Total Processes: "<<procCount<<endl;
cout<<"Average Wait Time: "<<waitTime/procCount<<endl;
cout<<"Wait Time Standard Deviation: "<<calcStdDev(nodeMap,turnaround/procCount)<<endl;
cout<<"Average Turnaround Time: "<<turnaround/procCount<<endl;
}

vector<Node*> updateNodeMap(multimap<double,Node*,less<double> >* nodeMap,double SYS_TIME,vector<Node*> nodeVector,double minVrunTime)
{
multimap <double,Node*> :: iterator itr;
vector<Node*> tempNodeVector;
for (int i=0;i<nodeVector.size();i++)
    {
        Node *ptr=nodeVector[i];
        double arrival=ptr->arrival;
        //cout<<arrival<<":"<<SYS_TIME<<endl;
        if(arrival<=SYS_TIME)
        {
          nodeMap->insert(pair<double,Node*>(minVrunTime,ptr));
        }
        else
            tempNodeVector.push_back(ptr);
    }
    cout << endl;
    return tempNodeVector;
}

// Driver Code
int main()
{

    cout<<"Enter the No. of Processes ";
  int N;
  cin>>N;
  INITIAL_PROC_COUNT=N;
  vector<Node*> nodeVector;
 multimap<double,Node*,less<double> > nodeMap;
    int weightTotal=0.0;

    bool allProcInserted=false;double minimumArrTime=INT_MAX;double SYS_TIME=0;double minVrunTime=0.0;

  cout<<"Arrival Time\t\tBurst Time\t\tNice Value"<<endl;
  for(int i=0; i<N; i++){
    int tempPID,tempArrTime,tempBurst;
    int tempNiceValue;
    cin>>tempArrTime>>tempBurst>>tempNiceValue;
    tempPID=i;
    if(tempArrTime<minimumArrTime)
        minimumArrTime=tempArrTime;
    nodeVector.push_back(new Node(0.0,tempBurst,tempPID,tempArrTime,tempNiceValue));
    weightTotal+=(prio_to_weight[tempNiceValue+INC_COUNT]);
  }
    INITIAL_WEIGHT_TOTAL=weightTotal;
    SYS_TIME+=minimumArrTime;
    CURRENT_RUN_TIME+=minimumArrTime;
    nodeVector=updateNodeMap(&nodeMap,SYS_TIME,nodeVector,minVrunTime);
    weightTotal=printNodeMap(&nodeMap);
    while(statMap.size()!=N)
    {
    SYS_TIME+=runSchedule(weightTotal,&nodeMap);
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
