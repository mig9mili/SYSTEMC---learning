#include<iostream>
#include<systemc>

using namespace std;
using namespace sc_core;

SC_MODULE(PROCESS){
 SC_CTOR(PROCESS){
    SC_THREAD(thread);
    SC_METHOD(method);
 }
 void thread(){
    int id=0;
    while(true){
        cout<<"thread"<<id++<<"@"<<sc_time_stamp()<<endl;
        wait(1,SC_SEC);
    }
 }

 void method(){
    int idx=0;
    cout<<"method"<<idx++<<"@"<<sc_time_stamp()<<endl;
    next_trigger(1,SC_SEC);
 }
};

int sc_main(int,char*[]){
    PROCESS process("process");
    sc_start(7,SC_SEC);
    return 0;
}
