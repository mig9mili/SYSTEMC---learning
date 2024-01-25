#include<systemc>

using namespace sc_core;




SC_MODULE(driver){
  
  sc_out <bool> d_din,d_clr;

  void drive(void);

  SC_CTOR(driver){
    SC_THREAD(drive);
    sensitive << d_din;
  }
};