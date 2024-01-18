#include<iostream>
#include<systemc>

using namespace std;
using namespace sc_core;

SC_MODULE(COMBINED){

    sc_event e1,e2,e3,e4,e5,e6,e7,e8,e9,e10;
 SC_CTOR(COMBINED){
  SC_THREAD(trigger);
  SC_THREAD(catcher0);
  SC_THREAD(catcher1);
  SC_THREAD(catcher2and3);
  SC_THREAD(catcher4or5);
  SC_THREAD(catcher_timeout_or_6);  //for 5 sec timeout
  SC_THREAD(catcher_timeout_or_7or8);  //for 20 sec timeout
  SC_THREAD(catcher_timeout_or_9and10); //for 20 sec timeout

 }

 // trigger event;
 void trigger(void);

 // catcher_0

void catcher0(void){
  wait(2,SC_SEC);
  cout<<sc_time_stamp()<<":2 sec timeout"<<endl;
}

void catcher1(void){
    wait(e1);
    cout<<sc_time_stamp()<<": catch e1"<<endl;
}

void catcher2and3(void){
    wait(e2&e3);
    cout<<sc_time_stamp()<<": catch e2 and e3"<<endl;
}

void catcher4or5(void)
{
    wait(e4|e5);
    cout<<sc_time_stamp()<<"catch e4 or  e5"<<endl;
}

void catcher_timeout_or_6(void){

    wait(sc_time(5,SC_SEC),e6);  // 5 sec timeout
    cout<<sc_time_stamp()<<" 5sec timeout or catch event 6 "<<endl;

}

void catcher_timeout_or_7or8(void){
    wait(sc_time(20,SC_SEC),e7|e8);
    cout<<sc_time_stamp()<<"20 sec timeout or catch e7 or e8"<<endl;
}

void catcher_timeout_or_9and10(void){
    wait(sc_time(20,SC_SEC),e9&e10);
    cout<<sc_time_stamp()<<"20 sec timeout or catch e7 and e10"<<endl;
}
};

void COMBINED::trigger(void){

    e1.notify(1,SC_SEC);
    e2.notify(2,SC_SEC);
    e3.notify(3,SC_SEC);
    e4.notify(4,SC_SEC);
    e5.notify(5,SC_SEC);
    e6.notify(6,SC_SEC);
    e7.notify(7,SC_SEC);
    e8.notify(8,SC_SEC);
    e9.notify(9,SC_SEC);
    e10.notify(10,SC_SEC);
}


int sc_main(int , char*[]){
    COMBINED combined("combined");
    sc_start();
    return 0;
}