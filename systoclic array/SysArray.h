/*
 * VLSI Systolic Array Module to include N*N of SysPEs
 *
 */

#ifndef __SYSARRAY_H__
#define __SYSARRAY_H__

#include <systemc.h>
#include "SysPE.h"
#include "string"

SC_MODULE(SysArray)
{
  public:
  sc_in_clk     clk;
  sc_in<bool>   rst;
  
  // Array Size 
  const static int N = 3;

   
  SysPE* pe_array[N][N];

  // I/O
  // FIXME: another weight input module to reduce input channels
  sc_fifo_in<SysPE::InputType>   weight_in_vec[N];
  sc_fifo_in<SysPE::InputType>   act_in_vec[N];
  sc_fifo_out<SysPE::AccumType>  accum_out_vec[N];

  // Interconnection Channels 
  sc_fifo<SysPE::InputType> weight_inter[N][N];
  sc_fifo<SysPE::InputType> act_inter[N][N]; 
  sc_fifo<SysPE::AccumType> accum_inter[N][N];

  SC_HAS_PROCESS(SysArray);
  SysArray(sc_module_name name_) : sc_module(name_) {
    for (int i = 0; i < N; i++) {    // rows
      for (int j = 0; j < N; j++) {  // cols
        pe_array[i][j] = new SysPE(sc_gen_unique_name("pe"));
        pe_array[i][j]->clk(clk);
        pe_array[i][j]->rst(rst);
        
        // Weight Connections
        if (i == 0) {    // from Array weight (row 0)
          pe_array[i][j]->weight_in(weight_in_vec[j]);
          pe_array[i][j]->weight_out(weight_inter[i][j]);
        }
        else {           // from Array weight (row 1, 2, N-1)
          pe_array[i][j]->weight_in(weight_inter[i-1][j]);
          pe_array[i][j]->weight_out(weight_inter[i][j]);
        }
        // Act Connections
        if (j == 0) {  // from Array input (col 0)
          pe_array[i][j]->act_in(act_in_vec[i]);
          pe_array[i][j]->act_out(act_inter[i][j]);
        }
        else {         // from Array input (col 1, 2...)
          pe_array[i][j]->act_in(act_inter[i][j-1]);
          pe_array[i][j]->act_out(act_inter[i][j]);
        }
        // Accum Connections
        if (i != N-1) {  // from Array Accum (row 0, 1, N-2)
          pe_array[i][j]->accum_in(accum_inter[i][j]);
          pe_array[i][j]->accum_out(accum_inter[i+1][j]);
        }
        else {           // from Array Accum (row N-1)
          pe_array[i][j]->accum_in(accum_inter[i][j]);
          pe_array[i][j]->accum_out(accum_out_vec[j]);
        }
      }  
    }
 
    SC_THREAD (WeightOutRun);
    sensitive << clk.pos();
    reset_signal_is(rst, true);
    
    SC_THREAD (ActOutRun);
    sensitive << clk.pos();
    reset_signal_is(rst, true);
    
    SC_THREAD (AccumInRun);
    sensitive << clk.pos();
    reset_signal_is(rst, true);
  }
   
  // Receive act_out from col N-1 (PopNB, every cycle)
  // use ResetRead to indicate the output port of channel
  void WeightOutRun() {
    while(1) {
      for (int j = 0; j < N; j++) {
        SysPE::InputType weight_tmp; 
        weight_inter[N-1][j].read(weight_tmp);
      }
      wait();
    }
  }
  // Receive act_out from col N-1 (PopNB, every cycle)
  // use ResetRead to indicate the output port of channel
  void ActOutRun() {
    while(1) {
      for (int i = 0; i < N; i++) {
        SysPE::InputType act_tmp; 
        act_inter[i][N-1].read(act_tmp);
      }
      wait();
    }
  }

  // Send 0 to accum_in of row 0 (PushNB, every cycle)
  // use ResetWrite to indicate the input port of channel
  void AccumInRun() {
    while(1) {
      for (int j = 0; j < N; j++) {
        accum_inter[0][j].write(0);
      }
      wait();
    }
  }
  
};

#endif
