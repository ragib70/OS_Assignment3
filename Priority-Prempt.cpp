#include<bits/stdc++.h>
#include<time.h>
#include<math.h>
using namespace std;


struct PCB{
  int PID=0;
  int burst=0;
  int priority=0;
  int arrival=0;// Time Instant when a process arrives.
  int turnaround=0;// Time taken for the completion of the process from the instant it was submitted.
  int wait=0;//Time spent waiting in the ready queue.
  int completion=0;// Timestamp at which a particular process execution completes.###from the point of view of CPU.
};

void initialize_randomly(int pcount, PCB *P){

    srand (time(NULL));
    default_random_engine arrival_generator, burst_generator;
    normal_distribution<double> arrival_distribution(6,4), burst_distribution(5,3);
    // uniform_int_distribution<int> arrival_distribution(20,25), burst_distribution(20,25);

    int pid, arrival, burst, pr;
    cout<<"Process ID\t\tArrival Time\t\tBurst Time\t\tPriority"<<endl;

    for (int i = 0; i < pcount; i++) {
      pid = i+1;
      // pid = i%20; // for recurring processes
      arrival = (int) arrival_distribution(arrival_generator) ;
      arrival = abs(arrival);

      burst =  (int) burst_distribution(burst_generator);
      burst = abs(burst);

      // added for MLQ
      pr = rand()%5;
      if(pr==0)
        pr=1;

      P[i].PID = pid; P[i].arrival = arrival; P[i].burst = burst; P[i].priority=pr;
      cout<<P[i].PID<<"\t\t\t"<<P[i].arrival<<"\t\t\t"<<P[i].burst<<"\t\t\t"<<P[i].priority<<endl;
    }
}


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

void fillWaitTime(PCB* P, int N){
  int remaining[N];

  for(int i=0; i<N; i++){
    remaining[i] = P[i].burst;
  }

  int completed=0, minPriority=INT_MAX, process=0;

  for(int time=P[0].arrival; completed!=N; time++){

    int i=0;
    int token = 0;
    for(i=0; i<N; i++){
      if(P[i].arrival<=time && remaining[i]>0 && P[i].priority<minPriority){
          process=i;
          minPriority = P[process].priority;
          token = 1;
        }
    }

    if(token==0){//Earlier I kept i==N and it failed make it sure to correct if any test case fails.
      cout<<"At t = "<<time<<" , CPU is idle"<<endl;
      continue;
    }

    cout<<"At t = "<<time<<", Highest Priority Job Till Now Executing = "<<P[process].PID<<endl;

    remaining[process]--;

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

  cout<<"Enter the No. of Processes ";
  int N;
  cin>>N;
  struct PCB* P = new PCB[N];

  cout<<"Choose Random Generator(0) or Manual Input(1) or Distribution Generator(2)"<<endl;
  int option;
  cin>>option;
  if(option==1)
      ManualFill(P,N);
  else if(option==0){
      AutoFill(P,N);
    }
  else if(option==2){
      initialize_randomly(N,P);
  }
  else
      cout<<"Wrong Option choosen"<<endl;

  sort(P,P+N,comparetwoP);

  cout<<"Simulation Results :"<<endl;
  fillWaitTime(P,N);

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
