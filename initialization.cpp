#include<systemc>
#include<iostream>

using namespace std;
using namespace sc_core;

SC_MODULE(INTIAL){
    sc_event e;
 SC_CTOR(INTIAL){

    SC_THREAD(trigger);
    SC_THREAD(catcher1);
    SC_THREAD(catcher2);
    SC_THREAD(catcher3);

    sensitive << e;//static sensitve;
   dont_initialize(); ///used to prevent the scheduler from executing a thread or method process during the initialization phase. Applies to the last declared process.  // not get any result for @0s for catch 3 however catch works at 0s and catch wait event e
  //check commenting  dont_initialize   u will find catch 3  @ sec0
 }

 void trigger(void){
    while(true){
        e.notify(1,SC_SEC);
        wait(2, SC_SEC);
    }
 }
 void catcher1(void){
    while(true){
    cout<<sc_time_stamp()<<"catcher1 triggered"<<endl;
    wait(e);//dynamic sensitive
    }
 }

 void catcher2(void){
    wait(e);  // wait for event e and nxt step are not  executed till event e is triggered
  while(true){
    cout<<sc_time_stamp()<<"catcher2 triggered"<<endl;
    wait(e); //dynamic sensitivity
  }
 }

 void catcher3(void){
    while(true){
        cout<<sc_time_stamp()<<"catcher3 triggered"<<endl;
        wait(e);//dynamic sensitive
    }
 }
};


int sc_main(int,char*[]){
    INTIAL init("init");
    sc_start(4,SC_SEC);
    return 0;
}