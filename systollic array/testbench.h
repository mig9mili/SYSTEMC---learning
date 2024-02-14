
#include<iostream>
#include<systemc>

using namespace std;
using namespace sc_core;
using namespace sc_dt;

//source part
SC_MODULE(tb){
    //input with handshacking var
    sc_out<sc_int<8>> wt_in;
    sc_out<bool> wt_in_vld;
    sc_in<bool> wt_in_rdy;

    sc_out<sc_int<8>> act_in;
    sc_out<bool> act_in_vld;
    sc_in<bool> act_in_rdy;

   sc_out<sc_int<32>> acc_in;
   sc_out<bool> acc_in_vld;
   sc_in<bool> acc_in_rdy;

    //output with handshacking var
    sc_in<sc_int<8>> wt_out;
    sc_in<bool> wt_outp_vld;
    sc_out<bool> wt_outp_rdy;

    sc_in<sc_int<8>> act_out;
    sc_in<bool> act_outp_vld;
    sc_out<bool> act_outp_rdy;

   sc_in<sc_int<32>>acc_out;
   sc_in<bool> acc_outp_vld;
   sc_out<bool> acc_outp_rdy;

    sc_in<bool>clk;
    sc_out<bool>rst;

  void source();
  void sink();

SC_CTOR(tb){
    SC_CTHREAD(source,clk.pos());
    reset_signal_is(rst,true);
    SC_CTHREAD(sink,clk.pos());
    reset_signal_is(rst,true);
}
};
