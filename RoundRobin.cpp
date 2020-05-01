#include<bits/stdc++.h>
using namespace std;

struct PCB{
  int PID=0;
  int burst=0;
  int arrival=0;// Time Instant when a process arrives.
  int turnaround=0;// Time taken for the completion of the process from the instant it was submitted.
  int wait=0;//Time spent waiting in the ready queue.
  int completion=0;// Timestamp at which a particular process execution completes.###from the point of view of CPU.
  int mark=0;//To keep track of the the processes inside the queue so that they don't get inserted again in the queue.
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

void fillWaitTime(PCB* P, int N, int tq){
  int remaining[N];

  for(int i=0; i<N; i++)
    remaining[i]=P[i].burst;

  int completed=0, time=0, current=0, token=0;
  queue<int> q;
  q.push(0);
  P[0].mark=1;//Mark the initial process because it will be visited at first.

  while(completed!=N){

    if(!q.empty())//There might be that the queue is empty and still completed !=n which can happen like arrival time of any process is very late like 100 or 1000
      current=q.front();
    else{//that condition is handled here
      for(int i=0; i<N; i++){
        if(P[i].mark==0){//we will search that process which is very late and since it has not got control of the CPU yet now its mark will be 0
          q.push(i);//we will find that and push it.
          current=q.front();
          break;
        }
      }
    }

    if(time<P[current].arrival){//arrival time of current process is greater that the clock time we have to wait as other process arrival time will be more than current process arrival as processes are sorted according to the arrival time
      cout<<"At t = "<<time<<", CPU is idle"<<endl;
      time+=1;
      continue;
    }

    q.pop();
    P[current].mark=1;//since the process has got control of the processor we mark it visited

    if(tq < remaining[current]){//execute the current process and save the remaining time
      cout<<"At t = "<<time<<", Job Executing = "<<P[current].PID<<endl;
      time+=tq;
      remaining[current]-=tq;
      token=1;//to indicate this process has to be scheduled again
    }
    else{
      cout<<"At t = "<<time<<", Job Executing = "<<P[current].PID<<endl;
      completed+=1;
      time+=remaining[current];
      cout<<"----------XXX----------"<<"At t = "<<time<<", Job "<<P[current].PID <<" Completed----------XXX-----------"<<endl;
      token=0;//to indicate that process execution is completed and need not be scheduled again
      remaining[current]=0;
      P[current].completion = time;
      P[current].wait = P[current].completion - P[current].burst - P[current].arrival;
      if(P[current].wait<0)//for those process whose arrival time is very late there wait will be 0
        P[current].wait=0;
    }

    for(int i=0; i<N; i++){
      if(P[i].arrival<=time && remaining[i]>0 && !P[i].mark){
        q.push(i);
        P[i].mark=1;//adding those process which have not been visited yet and have arrived
      }
    }

    if(token==1)
      q.push(current);//pushing the current process after the all non visited processes as the current process has some remianing time left since the token is 1
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




int main(){

  cout<<"Enter the No. of Processes ";
  int N;
  cin>>N;
  cout<<"Enter Time Quanta ";
  int tq;
  cin>>tq;
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

  fillWaitTime(P,N,tq);

  findturnaroundtime(P,N);

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
