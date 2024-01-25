#include<systemc>

using namespace sc_core;

SC_MODULE(moniter){
 sc_in<bool> m_dout , m_clr ,m_din ;

 void monita(void);

 SC_CTOR(moniter){
    SC_METHOD(monita);
    sensitive << m_din <<m_clr<<m_din;
    dont_initialize();

 }
 };