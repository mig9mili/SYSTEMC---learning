#include<systemc>
using namespace sc_core;

SC_MODULE(driver){
    sc_out <bool> d_A,d_B;

    void drive(void);

    SC_CTOR(driver){
        
        SC_THREAD(drive);
    }
};