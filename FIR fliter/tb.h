#include<systemc>
using namespace sc_core;
using namespace sc_dt;

SC_MODULE(tb){
   sc_in<bool> clk;
   sc_out<bool> rst;
   
   sc_out<sc_int<16>> in;
   sc_out<bool> in_vld;
   sc_in<bool> in_rdy;

   sc_in<sc_int<16>> out;
   sc_in<bool> outp_vld;
   sc_out<bool> outp_rdy;

   void source();
   void sink();

   SC_CTOR(tb){
    SC_CTHREAD(source,clk.pos());
    SC_CTHREAD(sink,clk.pos());
   }
};