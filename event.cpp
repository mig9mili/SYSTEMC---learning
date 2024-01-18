#include<systemc>
#include<iostream>

using namespace std;
using namespace sc_core;


SC_MODULE(EVENT){
    sc_event e;
 SC_CTOR(EVENT){
    SC_THREAD(trigger);
    SC_THREAD(catcher);
 }
 void trigger(){
    while(true){
        //trigger the event  (create an immediate notification)
        e.notify(1,SC_SEC);
        if(sc_time_stamp() == sc_time(4,SC_SEC)){  //at time =4 cancel the event
            
            //delete any pending notification for this event
            e.cancel();
        }
        wait(2,SC_SEC);
    }

    
 }
 void catcher(){
    while(true){
        // wait for event  and catch
        wait(e);
        cout<<"Event catched @"<< sc_time_stamp() <<endl;
    }
 }
};

int sc_main(int ,char*[]){
    EVENT event("event");
    sc_start(8,SC_SEC);
    return 0;
}

