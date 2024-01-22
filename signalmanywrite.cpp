#include<iostream>
#include<systemc.h>

using namespace std;
using namespace sc_core;

/*template <class T, sc_writer_policy WRITER_POLICY = SC_ONE_WRITER> class sc_signal: public sc_signal_inout_if<T>, public sc_prim_channel {}
1. If WRITER_POLICY == SC_ONE_WRITER, it shall be an error to write to a given signal instance from more than one process instance at any time during simulation.
2. If WRITER_POLICY == SC_MANY_WRITERS:
  a) it shall be an error to write to a given signal instance from more than one process instance during any given evaluation phase,
  b) but different process instances may write to a given signal instance during different delta cycles.*/


SC_MODULE(MULTI)
{
    sc_signal<int> s1;  // singal writer sig
    sc_signal<int ,SC_MANY_WRITERS> s2;  // multi writer signal 
    //If WRITER_POLICY == SC_MANY_WRITERS: different process instances may write to a given signal instance during different delta cycles

    SC_CTOR(MULTI){
        SC_THREAD(writer1);
        SC_THREAD(writer2);
        SC_THREAD(consumer1);
        sensitive << s1; // static sensitive
        dont_initialize();
        SC_THREAD(consumer2);
        sensitive << s2; //static sensitive
        dont_initialize();
    }

    void writer1(){
        int v= 1;
        while(true){
            s1.write(v); // single write
            s2.write(v);
            cout<<sc_time_stamp()<<" writer1 write "<<v++<<endl;
            wait(1,SC_SEC);
        }
    }

    void writer2(){
        int v= -1;
        while(true){
            wait(SC_ZERO_TIME);  //many writes :- needed to offset the write time. Otherwise runtime error: conflicting write in delta cycle 0
            s2.write(v); // s2 many writes
            cout<<sc_time_stamp()<<" writer2 writes"<<v--<<endl;
            wait(1,SC_SEC);
        } 
    }

    void consumer1(){
        while(true){
            cout<<sc_time_stamp()<<" consumer1 reads s1 = "<<s1.read()<<" s2 = "<<s2.read()<<endl;
            wait();

        }
    }

    void consumer2(){

        while(true){
            cout<<sc_time_stamp()<<" consumer2 reads s1 = "<<s1.read()<<" s2 = "<<s2.read()<<endl;
            wait();
        }

    }
};


int sc_main(int , char*[]){
    MULTI multi("multi");
    sc_start(2,SC_SEC);
    return 0;
}