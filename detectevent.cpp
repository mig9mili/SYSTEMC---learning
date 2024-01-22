#include<iostream>
#include<systemc>

using namespace std;
using namespace sc_core;


SC_MODULE(SIGNAL_EVENT){
    sc_signal<int> s1,s2;
 SC_CTOR(SIGNAL_EVENT){
    SC_THREAD(producer1);
    SC_THREAD(producer2);
    SC_THREAD(consumer);
    sensitive<< s1 << s2;//static sensitive for thread -->consumer();
    dont_initialize();// for consumer()
    
 }

 void producer1(void){
    int v=1;
    while(true){
        s1.write(v++);
        wait(2,SC_SEC);
    }
 }

 void producer2(void){
    int v=1;
    while(true){
        //s2.write(v++);
        s2=v++;
        wait(3,SC_SEC);
    }
 }
//bool event():---> return true if and only if the value of the
// signal changed in the update phase of the immediately preceding delta cycle and at the current simulation time.
 void consumer(){
    while(true){
        if(s1.event()==true && s2.event()==true){
            cout<<sc_time_stamp()<<" s1 & s2 triggered"<<endl;
        }
        else if(s1.event()==true){
            cout<<sc_time_stamp()<<" s1 triggered "<<endl;
        }
        else {
            cout<<sc_time_stamp()<<" s2 triggered "<<endl;
        }
        wait();
    }
 }
};

int sc_main(int , char*[]){
    SIGNAL_EVENT signal_event("signal_event");
    sc_start(7,SC_SEC);
    return 0;
}