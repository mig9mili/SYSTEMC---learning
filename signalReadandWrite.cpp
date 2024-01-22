#include<iostream>
#include<systemc>

using namespace sc_core;
using namespace std;

SC_MODULE(SIGNAL)
{
  sc_signal<int> s;
  SC_CTOR(SIGNAL){
    SC_THREAD(readwrite);
  }

  void readwrite(){
     //s.write():-
    //modifies the value of the signal 
    //such that the signal appears to have the new value (as returned by member function read) in the next delta cycle but not before then
    s.write(3);
    cout<<"s= "<<s<<";"<<s.read()<<endl;
    wait(SC_ZERO_TIME); //  1st delta cycle
    cout<<"after delta_cycle ,s = "<<s<<endl;
    
    s=4;
    s=5;
   
    int tmp = s;
    cout<<"s = "<<tmp<<endl;

    wait(SC_ZERO_TIME); // 2nd delta cycle

    cout<<"after delta_cycle, s= "<<s.read()<<endl;

  }
};


int sc_main(int , char*[]){
    SIGNAL signal("signal");
    signal.s = -1;
    sc_start();
    return 0;
}