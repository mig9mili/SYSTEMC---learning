
#include<systemc>
using namespace sc_core;

SC_MODULE(moniter){
    sc_in<bool> m_din , m_s_out;

    SC_CTOR(moniter){
        SC_METHOD(monit);
        sensitive << m_s_out;
        dont_initialize();
    }

    void monit(void);
};