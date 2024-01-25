#ifndef NOTGATE_H_
#define NOTGATE_H_
#include<systemc>

// declarations in header
using namespace sc_core;


SC_MODULE(NOT){
    sc_in<bool> in;
    sc_out<bool> out;

   //constructor: where the processes are bound to simulation kernel
    SC_CTOR(NOT){
        SC_METHOD(not_gate);
        sensitive << in;  //static sensitive
       // dont_initialize();
    }

    void not_gate(void);
};

#endif  //NOTGATE_H_