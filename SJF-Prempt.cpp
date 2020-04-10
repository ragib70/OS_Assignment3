#include<bits/stdc++.h>
using namespace std;

struct PCB{
  int PID=0;
  int burst=0;
  int arrival=0;// Time Instant when a process arrives.
  int turnaround=0;// Time taken for the completion of the process from the instant it was submitted.
  int wait=0;//Time spent waiting in the ready queue.
  int completion=0;// Timestamp at which a particular process execution completes.###from the point of view of CPU.
};

bool comparetwoP(PCB P1, PCB P2){
  if(P1.arrival == P2.arrival)
      return (P1.burst < P2.burst);

  return (P1.arrival < P2.arrival);
}

void findwaittime(PCB* P, int N){
  int remaining[N];

  for(int i=0; i<N; i++){
    remaining[i] = P[i].burst;
  }

  int completed=0, short_remain=INT_MAX, shortest=0;

  for(int time=P[0].arrival; completed!=N; time++){

    int i=0;
    for(i=0; i<=N; i++){
      if(P[i].arrival<=time && remaining[i]>0 && remaining[i]<short_remain){
          shortest=i;
          short_remain = remaining[i];
        }
    }

    if(i==N)
      continue;

    cout<<"Shortest Job Executing = "<<P[shortest].PID<<endl;

    remaining[shortest]--;

    short_remain = remaining[shortest];

    if(short_remain==0)
       cout<<"----------XXX----------Job "<<P[shortest].PID <<" Completed----------XXX-----------"<<endl;
       short_remain = INT_MAX;

    if(remaining[shortest]==0){
      completed+=1;
      P[shortest].completion = time+1;
      P[shortest].wait = P[shortest].completion - P[shortest].burst - P[shortest].arrival;
      if(P[shortest].wait<0)
        P[shortest].wait=0;

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

  cout<<"Process ID\t\tBurst Time\t\tWait Time\t\tTrunaround Time\t\tCompletion Time"<<endl;
  for(int i=0; i<N; i++){
    cout<<P[i].PID<<"\t\t\t"<<P[i].burst<<"\t\t\t"<<P[i].wait<<"\t\t\t"<<P[i].turnaround<<"\t\t\t"<<P[i].completion<<endl;
  }

  cout<<"Average Wait Time = "<<findaveragewait(P,N)<<endl;

  cout<<"Average Turnaround Time = "<<findaverageturnaround(P,N)<<endl;

  free(P);


  return 0;
}
