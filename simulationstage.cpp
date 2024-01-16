#include<systemc>
#include<iostream>
using namespace std;
using namespace sc_core;


SC_MODULE(STAGE){
    SC_CTOR(STAGE){
        //elaboration
     cout<< sc_time_stamp()<<" "<<"elaboration constructor"<<endl;
         // intialization + simulation 
    SC_THREAD(thread);
    }
   // clean up
    ~STAGE(){
        cout<<sc_time_stamp()<<" "<<" cleanup :  desctructor"<<endl;
    }

    void thread(){

        //intialization
        cout<<sc_time_stamp()<<" "<<"execution intialization"<<endl;
      

      //simulation 
       
       int i =0;
       while(true){
        wait(1,SC_SEC);// advance time
        cout<<sc_time_stamp()<<" "<<"execution simulation"<<endl;//evaluaton

        if (++i >= 2){
            sc_stop();
        }
       }

    }
   

   void before_end_of_elaboration(){
    // called after the construction of the module hierarchy
    cout<<"before end of elaboration"<<endl;
    }

    void end_of_elaboration(){
    // called  after all callback to before before_end_of_elaboration have completed and 
    //after the completion of any instantiation or port binding performed by those callbacks and 
    //before starting simulation.
    cout<<"end of elaboration"<<endl;
    }

    void start_of_simulation(){
    // called immediately when the application calls sc_startfor the first time 
    //simulation is initiated under the direct control of the kernel
    // if an application makes multiple calls to sc_start, start_of_simulation is called on the first call to sc_start.
    //called after the callbacks to end_of_elaboration and before invoking the initialization phase of the scheduler.
    cout<<"start_of_simulation"<<endl;
    }

    void end_of_simulation(){

    //called when the scheduler halts because of sc_stop
    //if simulation is initiated under the direct control of the kernel.
    //b) called only once even if sc_stop is called multiple times
    cout<<"end_of_simulation"<<endl;
    }
};


int sc_main(int, char*[]){
    STAGE stage("Stage");
    sc_start();
    return 0;
}