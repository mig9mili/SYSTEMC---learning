#include<systemc>
using namespace sc_core;
using namespace std;
using namespace sc_dt;

SC_MODULE(fir){
 sc_in<bool> clk; //clock
 sc_in<bool> rst;  //reset
 sc_in<sc_int<16>> in; //input
  sc_in<bool> in_vld;
   sc_out<bool> in_rdy;

 sc_out<sc_int<16>>  out; //output
   sc_out<bool> outp_vld;
   sc_in<bool> outp_rdy;

 void fir_main();

 SC_CTOR(fir){
    SC_CTHREAD(fir_main, clk.pos());
    reset_signal_is(rst,true);  //true  is high  , flase is low
 }
};

