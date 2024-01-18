#include<iostream>
#include<systemc>

using namespace std;
using namespace sc_core;

SC_MODULE(SENSITIVITY){
    sc_event e1 ,e2;
 SC_CTOR(SENSITIVITY){

    SC_THREAD(trigger1);
    SC_THREAD(trigger2);
    SC_THREAD(catch_1or2_dyn);
    SC_THREAD(catch_1or2_static);

    sensitive << e1 << e2; // static sensitive
 }
 void trigger1(void){
    wait(SC_ZERO_TIME); // delay trigger by delta cycle

    while(true){
        e1.notify();
        wait(2,SC_SEC);
    }  
 }
 void trigger2(void){
    wait(SC_ZERO_TIME);
    while(true){
        e2.notify();
        wait(3,SC_SEC);
    }
 }

 void catch_1or2_dyn(){
    while(true){
        wait(e1|e2); //dynamic   //also check with &  and get the difference  betweeen static and dynamic
        cout<<"dynamic sensitivity : e1 or e2 @"<<sc_time_stamp()<<endl;
    }
 }

 void catch_1or2_static(){
    while(true){
        wait(); //static
        cout<<"static sensitivity: e1 or e2 @"<<sc_time_stamp()<<endl;

    }
 }
};


int sc_main(int , char*[]){
    SENSITIVITY sensitivity("sensitivity");
    sc_start(7,SC_SEC);
    return 0;
}
