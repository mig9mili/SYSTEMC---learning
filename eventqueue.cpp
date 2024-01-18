#include<iostream>
#include<systemc>
  // event queue :-- is a hierarchical channel and can have multiple notifications pending, this differs from an event which can have only one outstanding notificaiton scheduled.
using namespace std;
using namespace sc_core;

SC_MODULE(QUEUE){
    sc_event e ;
    sc_event_queue eq;
    SC_CTOR(QUEUE){
        SC_THREAD(trigger);
        SC_THREAD(catch_e);
        

        sensitive << e; //static sensitive

        dont_initialize(); //don't run catch_e during initialization phase


        SC_THREAD(catch_eq);
        sensitive << eq;

        dont_initialize();  //don't run catch_eq() during initialization phase

    }

    void trigger(){
        while(true){
            e.notify(2,SC_SEC);
            e.notify(1,SC_SEC);  //trigger e after 1 s, replaces previous trigger

            eq.notify(2,SC_SEC);  //trigger eq after 2 s
            eq.notify(1,SC_SEC);    //trigger eq after 1 s, both triggers available

            wait(10, SC_SEC) ; // check once commenting this 
        }
    }
    
    void catch_e(){
        while(true){
            cout<<sc_time_stamp()<<"catch e"<<endl;
            wait(); //static sensitive
        }
    }
   
    void catch_eq(){
        while(true){
            cout<<sc_time_stamp()<<" catch eq"<<endl;
            wait(); // static sensitive       
    }
    }

    
};

int sc_main(int,char*[]){
   QUEUE queue("queue");
   sc_start(20,SC_SEC);
   return 0;
}