#include<systemc>
#include<iostream>

using namespace std;
using namespace sc_core;

/*
 ex:-
if current value of a "signal" == 1: write 1 to it won't trigger value update event.
if current value of a "buffer" == 1: write 1 to it will trigger value update event.
*/

SC_MODULE(BUFFER){
    sc_signal<int> s;
    sc_buffer<int> b;

    SC_CTOR(BUFFER){
        SC_THREAD(writer);
        SC_THREAD(conusmer1);
        sensitive << s;
        dont_initialize();
        SC_THREAD(conusmer2);
        sensitive << b;
        dont_initialize();
    }

    void writer(){
        int val = 1;
        while(true){
            for(int i = 0; i<2 ; ++i){  //write same value to channel twice

                s.write(val);
                b.write(val);
                wait(1,SC_SEC);
            
            }
            val++;
        }
    }

    void conusmer1(){

        while(true){
            cout << sc_time_stamp()<<" consumer1 receives "<<s.read()<<endl;
            wait(); //recevies from signal
        }
    }

    void conusmer2(){
        while(true){
            cout<<sc_time_stamp()<<" consumer2 receives "<<b.read()<<endl;
            wait(); // recevies from buffer
        }
    }
};

int sc_main(int , char*[]){
    BUFFER buffer ("buffer");
    sc_start(4,SC_SEC);
    return 0;
}

