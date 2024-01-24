#ifndef NOTGATE_H_
#define NOTGATE_H_
#include<systemc>
// declarations in header
using namespace sc_core;


SC_MODULE(NOT){
    sc_in<bool> in;//input to notgate
    sc_out<bool> out;//output from notgate

    

   //constructor: where the processes are bound to simulation kernel
    SC_CTOR(NOT){
        SC_METHOD(not_gate);
        sensitive << in;  //static sensitive
    }

    void not_gate(void); //function call
};

#endif  //NOTGATE_H_
