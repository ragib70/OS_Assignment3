#include<bits/stdc++.h>
using namespace std;

struct PCB{
  int PID=0;
  int burst=0;
  int arrival=0;// Time Instant when a process arrives.
  int turnaround=0;// Time taken for the completion of the process from the instant it was submitted.
  int wait=0;//Time spent waiting in the ready queue.
  int completion=0;// Timestamp at which a particular process execution completes.
};

bool comparetwoP(PCB P1, PCB P2){
  return (P1.arrival < P2.arrival);
}

void findwaittime(PCB* P, int N){
  P[0].wait=P[0].arrival;
  for(int i=1; i<N; i++){
    int temp = P[i-1].wait + P[i-1].burst;
    if(temp > P[i].arrival)
      P[i].wait = temp;
    else
      P[i].wait = 0;
  }
}

void findturnaroundtime(PCB* P, int N){
  for(int i=0; i<N; i++){
    P[i].turnaround = P[i].wait + P[i].burst;
  }
}

void findcompletion(PCB* P, int N){
  for(int i=0; i<N; i++){
    P[i].completion = P[i].turnaround + P[i].arrival;
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
  struct PCB* P = new PCB[N];
  cout<<"PID\t\tArrival Time\t\tBurst Time"<<endl;
  for(int i=0; i<N; i++){
    cin>>P[i].PID>>P[i].arrival>>P[i].burst;
  }

  sort(P,P+N,comparetwoP);

  findwaittime(P,N);

  findturnaroundtime(P,N);

  findcompletion(P,N);

  cout<<"Process ID\t\tBurst Time\t\tWait Time\t\tTrunaround Time\t\tCompletion Time"<<endl;
  for(int i=0; i<N; i++){
    cout<<P[i].PID<<"\t\t\t"<<P[i].burst<<"\t\t\t"<<P[i].wait<<"\t\t\t"<<P[i].turnaround<<"\t\t\t"<<P[i].completion<<endl;
  }

  cout<<"Average Wait Time = "<<findaveragewait(P,N)<<endl;

  cout<<"Average Turnaround Time = "<<findaverageturnaround(P,N)<<endl;

  free(P);

  return 0;
}
