#include<systemc.h>

SC_MODULE(moniter){
    sc_in<bool> m_A,m_B,m_out;  // to read  so sc_in

    void monita(void);

    SC_CTOR(moniter){
        SC_METHOD(monita);
        sensitive << m_A << m_B << m_out;
        dont_initialize();
    } 
};