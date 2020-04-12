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
  int mark=0;//To keep track of the the processes inside the queue so that they don't get inserted again in the queue.
};

bool comparetwoP(PCB P1, PCB P2){
  return (P1.arrival < P2.arrival);
}

void fillWaitTime(PCB* P, int N, int tq){
  int remaining[N];

  for(int i=0; i<N; i++)
    remaining[i]=P[i].burst;

  int completed=0, time=0, current=0, token=0;
  queue<int> q;
  q.push(0);
  P[0].mark=1;

  while(completed!=N){
    current=q.front();

    if(time<P[current].arrival){
      cout<<"At t = "<<time<<", CPU is idle"<<endl;
      time+=1;
      continue;
    }

    q.pop();

    if(tq < remaining[current]){
      cout<<"At t = "<<time<<", Job Executing = "<<P[current].PID<<endl;
      time+=tq;
      remaining[current]-=tq;
      token=1;
    }
    else{
      cout<<"At t = "<<time<<", Job Executing = "<<P[current].PID<<endl;
      completed+=1;
      time+=remaining[current];
      cout<<"----------XXX----------"<<"At t = "<<time<<", Job "<<P[current].PID <<" Completed----------XXX-----------"<<endl;
      token=0;
      P[current].mark=0;
      remaining[current]=0;
      P[current].completion = time;
      P[current].wait = P[current].completion - P[current].burst - P[current].arrival;
      if(P[current].wait<0)
        P[current].wait=0;
    }

    for(int i=0; i<N; i++){
      if(P[i].arrival<=time && remaining[i]>0 && !P[i].mark){
        q.push(i);
        P[i].mark=1;
      }
    }

    if(token==1)
      q.push(current);
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

int main(){

  cout<<"Enter the No. of Processes ";
  int N;
  cin>>N;
  cout<<"Enter Time Quanta ";
  int tq;
  cin>>tq;
  struct PCB* P = new PCB[N];
  cout<<"PID\t\tArrival Time\t\tBurst Time"<<endl;
  for(int i=0; i<N; i++){
    cin>>P[i].PID>>P[i].arrival>>P[i].burst;
  }

  sort(P,P+N,comparetwoP);

  cout<<"Simulation Results :"<<endl;

  fillWaitTime(P,N,tq);

  findturnaroundtime(P,N);

  cout<<"Process ID\t\tBurst Time\t\tWait Time\t\tTurnaround Time\t\tCompletion Time"<<endl;
  for(int i=0; i<N; i++){
    cout<<P[i].PID<<"\t\t\t"<<P[i].burst<<"\t\t\t"<<P[i].wait<<"\t\t\t"<<P[i].turnaround<<"\t\t\t"<<P[i].completion<<endl;
  }

  cout<<"Average Wait Time = "<<findaveragewait(P,N)<<endl;

  cout<<"Average Turnaround Time = "<<findaverageturnaround(P,N)<<endl;

  free(P);

return 0;
}
