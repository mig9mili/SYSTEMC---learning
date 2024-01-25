#ifndef NANDGATE_H_
#define NANDGATE_H_
#include <systemc>

using namespace sc_core;

SC_MODULE(NAND){

    sc_in<bool> A,B; // input signal
    sc_out<bool> out; // out signal


    void nand_gate(void); 


    SC_CTOR(NAND){

        SC_METHOD(nand_gate); //register nand with kernel 
        sensitive << A << B; //static sensitive

    }
};

#endif /*NANDGATE_H_*/
