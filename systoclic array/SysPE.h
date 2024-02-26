#ifndef __SYSPE_H__
#define __SYSPE_H__

#include <systemc.h>

SC_MODULE(SysPE)
{
  public:
  sc_in_clk     clk;
  sc_in<bool>   rst;

  typedef sc_int<8>   InputType;
  typedef sc_int<32>  AccumType;

  // Interface Ports
  sc_fifo_in<InputType>   weight_in;
  sc_fifo_in<InputType>   act_in;
  sc_fifo_in<AccumType>   accum_in;
  
  sc_fifo_out<InputType>  weight_out;
  sc_fifo_out<InputType>  act_out;
  sc_fifo_out<AccumType>  accum_out;

  SC_HAS_PROCESS(SysPE);
  SysPE(sc_module_name name_) : sc_module(name_) {
    SC_THREAD (PERun);
    sensitive << clk.pos();
    reset_signal_is(rst, true);
  }

  bool is_weight_in, is_act_in, is_accum_in;

  InputType weight_reg, weight_out_reg;
  InputType act_reg;
  AccumType accum_reg;

  void PERun() {
    is_weight_in = 0;
    is_act_in = 0;
    is_accum_in = 0;
    weight_reg = 0;
    weight_out_reg = 0;
    act_reg = 0;
    accum_reg = 0;
  
    while(1) {
      InputType tmp;
      if (weight_in.nb_read(tmp)){
        is_weight_in = 1;
        weight_out_reg = weight_reg;
        weight_reg = tmp;
      }
      if (!is_act_in) {
        is_act_in = act_in.nb_read(act_reg);
      }
      if (!is_accum_in) {
        is_accum_in = accum_in.nb_read(accum_reg);
      }
    
      if (is_act_in && is_accum_in) {
        is_act_in = 0;
        is_accum_in = 0;

        AccumType accum_out_reg = act_reg*weight_reg + accum_reg;

        act_out.write(act_reg);
        accum_out.write(accum_out_reg);
      }
      if (is_weight_in == 1) {
        is_weight_in = 0;
        weight_out.write(weight_out_reg);
      }
      wait();
    }
  }
};

#endif
