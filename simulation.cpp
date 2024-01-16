#include<iostream>
#include<systemc>


using namespace sc_core;
using namespace std;


SC_MODULE(PROCESS){
 //declare clk
 sc_clock clk;


// instantiate the clock with  1 sec  periodicity
 SC_CTOR(PROCESS):clk("clk",1,SC_SEC){

    //registeration
    SC_METHOD(method);
    SC_THREAD(thread);
    SC_CTHREAD(cthread,clk);
 }

 void method( void)
{
   cout<<"method triggered @"<<" "<<sc_time_stamp()<<endl;
   next_trigger(sc_time(1,SC_SEC));//trigger after 1 sec
   /*while(true){
    // cannot do with while loop bez  METHOD  cannot call code that calls wait
   }*/
}

void thread(){
     //1.
    while(true){

        cout<<"thread triggered @"<<" "<<sc_time_stamp()<<endl;
        wait(1,SC_SEC);
    }
    //2.
   //cout<<"thread triggered @"<<" "<<sc_time_stamp()<<endl;
   // next_trigger(sc_time(1,SC_SEC))
}


void cthread(){

    while(true){
        //only have a static sensitivity of a clock edge event
        cout<<"cthread triggered @"<<" "<<sc_time_stamp()<<endl;
        wait();

    }
}
};


int sc_main(int , char*[]){
    PROCESS process("process"); // module init
    cout<<"execution phase begin @"<<" "<< sc_time_stamp()<<endl;

    // run simulation for 2 sec

    sc_start(2, SC_SEC);

    cout<<"execution phase ends @"<<" "<<sc_time_stamp()<<endl;
    
    return 0;
}



