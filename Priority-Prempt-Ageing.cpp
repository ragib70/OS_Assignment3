#include<bits/stdc++.h>
using namespace std;

struct PCB{
  int PID=0;
  int burst=0;
  int priority=0;
  int arrival=0;// Time Instant when a process arrives.
  int turnaround=0;// Time taken for the completion of the process from the instant it was submitted.
  int wait=0;//Time spent waiting in the ready queue.
  int completion=0;// Timestamp at which a particular process execution completes.###from the point of view of CPU.
  bool complete=false;
  int change=0;
};


void GenPID(PCB *P, int N){
  for(int i=0; i<N; i++)
    P[i].PID = i+1;
}

void RandGenA(PCB *P, int N){
  srand((unsigned) time(0));
  int randomNumber;
  for (int index = 0; index < N; index++) {
    randomNumber = (rand() % 1000) + 1;
    P[index].arrival = randomNumber;
  }
}

void RandGenB(PCB *P, int N){
  srand((unsigned) time(0));
  int randomNumber;
  for (int index = 0; index < N; index++) {
    randomNumber = (rand() % 100) + 1;
    P[index].burst = randomNumber;
  }
}

void RandGenP(PCB *P, int N){
  srand((unsigned) time(0));
  int randomNumber;
  for (int index = 0; index < N; index++) {
    randomNumber = (rand() % 5) + 1;
    P[index].priority = randomNumber;
  }
}


bool comparetwoP(PCB P1, PCB P2){
  if(P1.arrival == P2.arrival)
    return (P1.priority < P2.priority);

  return (P1.arrival < P2.arrival);
}

void fillWaitTime(PCB* P, int N,int Age){
  int remaining[N];

  for(int i=0; i<N; i++){
    remaining[i] = P[i].burst;
  }

  int completed=0, minPriority=INT_MAX, process=0, prevProcess=0;

  for(int time=P[0].arrival; completed!=N; time++){

    int i=0;
    int token = 0;
    for(i=0; i<N; i++){
      if(P[i].arrival<=time && remaining[i]>0 && P[i].priority<minPriority){
          process=i;
          if(prevProcess!=process){
            P[prevProcess].change=time;
            }
          minPriority = P[process].priority;
          token = 1;
        }
    }

    if(token==0){
      cout<<"At t = "<<time<<" , CPU is idle"<<endl;
      continue;
    }

    //Printing the last change time
    // for(int i=0; i<N; i++){
    //   cout<<"Process "<<P[i].PID<<" change = "<<P[i].change<<"---";
    // }

    prevProcess = process;
    cout<<"At t = "<<time<<", Highest Priority Job Till Now Executing = "<<P[process].PID<<endl;

    remaining[process]--;


    for(int j=0; j<N; j++){
        if(P[j].priority>1 && (time - P[j].change - P[j].arrival > Age) && remaining[j]>0)
          P[j].priority-=1;
    }

    minPriority = INT_MAX;

    if(remaining[process]==0){
      cout<<"----------XXX----------Job "<<P[process].PID <<" Completed----------XXX-----------"<<endl;
      completed+=1;
      P[process].completion = time+1;
      P[process].wait = P[process].completion - P[process].burst - P[process].arrival;
      if(P[process].wait<0)
        P[process].wait=0;

    }

  }

}

void findturnaroundtime(PCB* P,int N){
  for(int i=0; i<N; i++){
    P[i].turnaround = P[i].wait + P[i].burst;
  }
}

float findaveragewait(PCB* P,int N){
  float sum=0;
  for(int i=0; i<N; i++){
    sum+=P[i].wait;
  }
  return (sum/N);
}

float findaverageturnaround(PCB* P,int N){
  float sum=0;
  for(int i=0; i<N; i++){
    sum+=P[i].turnaround;
  }
  return (sum/N);
}


void ManualFill(PCB *P, int N){
  cout<<"PID\t\tArrival Time\t\tBurst Time\t\tPriority"<<endl;
  for(int i=0; i<N; i++){
    cin>>P[i].PID>>P[i].arrival>>P[i].burst>>P[i].priority;
  }
}

void AutoFill(PCB *P, int N){
  GenPID(P,N);
  RandGenA(P,N);
  RandGenB(P,N);
  RandGenP(P,N);
  cout<<"Process ID\t\tArrival Time\t\tBurst Time\t\tPriority"<<endl;
  for(int i=0; i<N; i++){
    cout<<P[i].PID<<"\t\t\t"<<P[i].arrival<<"\t\t\t"<<P[i].burst<<"\t\t\t"<<P[i].priority<<endl;
  }
}



int main(){

  cout<<"Enter the No. of Processes and Age";
  int N,Age;
  cin>>N;
  cin>>Age;
  struct PCB* P = new PCB[N];

  cout<<"Choose Random Generator(0) or Manual Input(1)"<<endl;
  int option;
  cin>>option;
  if(option==1)
      ManualFill(P,N);
  else if(option==0){
      AutoFill(P,N);
    }
  else
      cout<<"Wrong Option choosen"<<endl;
  sort(P,P+N,comparetwoP);

  cout<<"Simulation Results :"<<endl;
  fillWaitTime(P,N,Age);

  findturnaroundtime(P,N);

  cout<<"Process ID\t\tBurst Time\t\tPriority\t\tWait Time\t\tTurnaround Time\t\tCompletion Time"<<endl;
  for(int i=0; i<N; i++){
    cout<<P[i].PID<<"\t\t\t"<<P[i].burst<<"\t\t\t"<<P[i].priority<<"\t\t\t"<<P[i].wait<<"\t\t\t"<<P[i].turnaround<<"\t\t\t"<<P[i].completion<<endl;
  }

  cout<<"Average Wait Time = "<<findaveragewait(P,N)<<endl;

  cout<<"Average Turnaround Time = "<<findaverageturnaround(P,N)<<endl;

  free(P);



return 0;
}
