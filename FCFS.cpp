#include<bits/stdc++.h>
#include<time.h>
#include<math.h>
using namespace std;

struct PCB{
  int PID=0;
  int burst=0;
  int arrival=0;// Time Instant when a process arrives.
  int turnaround=0;// Time taken for the completion of the process from the instant it was submitted.
  int wait=0;//Time spent waiting in the ready queue.
  int completion=0;// Timestamp at which a particular process execution completes.###from the point of view of CPU.
};

void compute_stdev(PCB *P, int N, int meanwait, int meanturaround){
  double stdevwait = 0 , tempw = 0, temp = 0, stdevturnaround = 0;
  for (int i = 0 ; i < N ; i++) {
       tempw = pow((P[i].wait - meanwait),2);
       temp = pow((P[i].turnaround - meanturaround),2);
       stdevwait+=tempw;
       stdevturnaround+=temp;
  }

  stdevwait = stdevwait / N;
  stdevwait = sqrt(stdevwait);

  stdevturnaround = stdevturnaround / N;
  stdevturnaround = sqrt(stdevturnaround);

  cout << "Wait standard deviation : " << stdevwait << '\n';
  cout << "Turnaround standard deviation : " << stdevturnaround << '\n';

}

void initialize_randomly(int pcount, PCB *P){

    srand (time(NULL));
    default_random_engine arrival_generator, burst_generator;
    normal_distribution<double> arrival_distribution(6,4), burst_distribution(5,3);
    // uniform_int_distribution<int> arrival_distribution(20,25), burst_distribution(20,25);

    int pid, arrival, burst;
    cout<<"Process ID\t\tArrival Time\t\tBurst Time"<<endl;

    for (int i = 0; i < pcount; i++) {
      pid = i+1;
      // pid = i%20; // for recurring processes
      arrival = (int) arrival_distribution(arrival_generator) ;
      arrival = abs(arrival);

      burst =  (int) burst_distribution(burst_generator);
      burst = abs(burst);

      // added for MLQ

      P[i].PID = pid; P[i].arrival = arrival; P[i].burst = burst;
      cout<<P[i].PID<<"\t\t\t"<<P[i].arrival<<"\t\t\t"<<P[i].burst<<endl;
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





bool comparetwoP(PCB P1, PCB P2){
  return (P1.arrival < P2.arrival);
}

int findwaittime(int c0, int a1){
  int wait = c0 - a1;
  if(wait < 0)
    return 0;
  else
    return wait;
}

int findturnaroundtime(int a, int b){
  return (a+b);
}

int findcompletion(int a, int b){
  return (a+b);
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

void fill(PCB* P,int N){

  for(int i=1; i<N; i++){
    P[i].wait = findwaittime(P[i-1].completion,P[i].arrival);
    P[i].turnaround = findturnaroundtime(P[i].wait,P[i].burst);
    P[i].completion = findcompletion(P[i].arrival,P[i].turnaround);
  }
}


void ManualFill(PCB *P, int N){
  cout<<"PID\t\tArrival Time\t\tBurst Time"<<endl;
  for(int i=0; i<N; i++){
    cin>>P[i].PID>>P[i].arrival>>P[i].burst;
  }
}

void AutoFill(PCB *P, int N){
  GenPID(P,N);
  RandGenA(P,N);
  RandGenB(P,N);
  cout<<"Process ID\t\tArrival Time\t\tBurst Time"<<endl;
  for(int i=0; i<N; i++){
    cout<<P[i].PID<<"\t\t\t"<<P[i].arrival<<"\t\t\t"<<P[i].burst<<endl;
  }
}

void GanttChart(PCB *P, int N){
  int t=0;
  for(int i=0; i<N; i++){
    if(t < P[i].arrival){
      cout<<"From time = "<<t<<", CPU is idle"<<endl;
      t=P[i].arrival;
      cout<<"From time = "<<t<<", Process "<<P[i].PID<<" is executing."<<endl;
      t=P[i].completion;
      cout<<"At time = "<<t<<", Process "<<P[i].PID<<" completed."<<endl;
    }
    else{
      cout<<"From time = "<<t<<", Process "<<P[i].PID<<" is executing."<<endl;
      t=P[i].completion;
      cout<<"At time = "<<t<<", Process "<<P[i].PID<<" completed."<<endl;
    }
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

  P[0].wait = 0;
  P[0].turnaround = P[0].burst;
  P[0].completion = P[0].burst + P[0].arrival;

  fill(P,N);

  GanttChart(P,N);

  cout<<"Process ID\t\tBurst Time\t\tWait Time\t\tTurnaround Time\t\tCompletion Time"<<endl;
  for(int i=0; i<N; i++){
    cout<<P[i].PID<<"\t\t\t"<<P[i].burst<<"\t\t\t"<<P[i].wait<<"\t\t\t"<<P[i].turnaround<<"\t\t\t"<<P[i].completion<<endl;
  }

  cout<<"Average Wait Time = "<<findaveragewait(P,N)<<endl;

  cout<<"Average Turnaround Time = "<<findaverageturnaround(P,N)<<endl;

  compute_stdev(P,N,findaveragewait(P,N),findaverageturnaround(P,N));

  free(P);

  return 0;
}
