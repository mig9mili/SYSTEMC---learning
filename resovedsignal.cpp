/*Resolution table for sc_signal_resolved:
  | 0 | 1 | Z | X |
0 | 0 | X | 0 | X |
1 | X | 1 | 1 | X |
Z | 0 | 1 | Z | X |
X | X | X | X | X |*/


//resolved signal may be written by multiple processes, conflicting values being resolved within the channel.

/*1. sc_signal_resolved is a predefined primitive channel derived from class sc_signal.
  2. sc_signal_rv is a predefined primitive channel derived from class sc_signal.
  
  The difference is that the argument to the base class template sc_signal is type sc_dt::sc_lv<W> instead of type sc_dt::sc_logic.
  */

#include<bits/stdc++.h>
#include<systemc>

using namespace std;
using namespace sc_core;
using namespace sc_dt;   //sc_logic defined here // namespace for  resolved signal

SC_MODULE(RESOLVED_SIG){
 sc_signal_resolved rv; //a resolved signal channel
 vector<sc_logic> levels; //declares a vector of possible 4-level logic values

  SC_CTOR(RESOLVED_SIG): levels(vector<sc_logic>{sc_logic_0,sc_logic_1,sc_logic_Z,sc_logic_X}){

    SC_THREAD(writer1);
    SC_THREAD(writer2);
    SC_THREAD(consumer);

  }
  
	 void writer1(){
    int idx =0;
    while(true){
      rv.write(levels[idx++%4]); //0,1,Z,X  0,1,Z,X  0,1,Z,X
      wait(1,SC_SEC);
    }
   }

   void writer2(){
    int idx = 0;
    while(true){
      rv.write(levels[(idx++/4)%4]); // 0,0,0,0 1,1,1,1, Z,Z,Z,Z  X,X,X,X
      wait(1,SC_SEC);
    }
   }

   void consumer(){
    wait(1, SC_SEC);
    int idx =0;
    while(true){
      cout<<" "<<rv.read()<<"|";
      if(++idx % 4==0){
        cout<<endl;
      }
      wait(1,SC_SEC);
    }
   }
};

int sc_main(int , char*[]){

  RESOLVED_SIG resolved("resolved");
  sc_start(17,SC_SEC);
  return 0;
  
  
}
