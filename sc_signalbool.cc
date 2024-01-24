#include<systemc>
#include<iostream>


using namespace std;
using namespace sc_core;

SC_MODULE(SIGNAL_BOOL){
    sc_signal<bool> b;
 SC_CTOR(SIGNAL_BOOL){
    SC_THREAD(writer);
    SC_THREAD(consumer);
    sensitive << b; //triggered by every value change
    dont_initialize();
    SC_THREAD(consumer_pos);
    sensitive << b.posedge_event(); //notified whenever the value of the channel changes and the new value of the channel is true or '1'.

    dont_initialize();
    SC_THREAD(consumer_neg);
    sensitive << b.negedge_event(); //notified whenever the value of the channel changes and the new value of the channel is false or '0'.
    dont_initialize();
 }

 void writer(){
    bool v=true;
    while(true){
        b.write(v);
        v= !v;
        wait(1,SC_SEC);
    }
 }

 void consumer(){
    while(true){
        if(b.posedge()){
            cout<<sc_time_stamp()<<" consumer receives posedege , b= "<< b << endl;
        }
        else{
            cout<<sc_time_stamp()<<" consumer recevies negedge ,  b = "<< b <<endl;

        }

        wait();
        
    }
 }

 void consumer_pos(){
    while(true){
        cout << sc_time_stamp()<<" consumer_pos receives posedge , b = "<< b <<endl;
        wait();
    }
 }

 void consumer_neg(){
    while(true){
        cout<<sc_time_stamp()<<" consumer_neg recevies negedge, b  = "<< b <<endl;
        wait();
    }
 }
};

int sc_main(int , char*[]){
    SIGNAL_BOOL signal_bool("signal_bool");
    sc_start(4,SC_SEC);
    return 0;
}