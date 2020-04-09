#include<bits/stdc++.h>
using namespace std;

struct PCB{
  int PID=0;
  int burst=0;
  int arrival=0;
  int turnaround=0;
  int wait=0;
  int completion=0;
};

bool comparetwoP(PCB P1, PCB P2){
  return (P1.arrival <= P2.arrival);
}

int main(){

  cout<<"Enter the No. of Processes ";
  int N;
  cin>>N;
  PCB* P = new PCB[N];
  cout<<"PID\t\tArrival Time\t\tBurst Time"<<endl;
  for(int i=0; i<N; i++){
    cin>>P[i].PID>>P[i].arrival>>P[i].burst;
  }

  sort(P,P+N,comparetwoP);



  return 0;
}
