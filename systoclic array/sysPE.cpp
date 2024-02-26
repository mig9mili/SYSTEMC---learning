/*
 * SysPE testbench for Harvard cs148/248 only
 */
/*
#include "SysPE.h"
#include <systemc.h>
#include <bits/stdc++.h>
using namespace std;

SC_MODULE (Source) {
  sc_fifo_out<SysPE::InputType> act_in;
  sc_fifo_out<SysPE::InputType> weight_in;
  sc_fifo_out<SysPE::AccumType> accum_in;
  sc_fifo_in<SysPE::AccumType>  accum_out;
  sc_fifo_in<SysPE::InputType>  act_out;
  sc_fifo_in<SysPE::InputType>  weight_out;
  sc_in <bool> clk;
  sc_in <bool> rst;
  
  bool start_src;  
  vector<SysPE::InputType> act_list{0, -1, 3, -7, 15, -31, 63, -127};
  vector<SysPE::AccumType> accum_list{0, -10, 30, -70, 150, -310, 630, -1270};
  SysPE::InputType weight_data = 10;
  SysPE::AccumType accum_init = 0;
  SysPE::InputType act_out_src;
  SysPE::AccumType accum_out_src;

  SC_CTOR(Source) {
    SC_THREAD(run);
    sensitive << clk.pos();
    async_reset_signal_is(rst, false);
    SC_THREAD(pop_result);
    sensitive << clk.pos();
    async_reset_signal_is(rst, false);
  }

  void run() {
    SysPE::InputType _act;
    SysPE::AccumType _acc;

    // Wait for initial reset
    wait(20.0, SC_NS);
    wait();
    // Write wait to PE
    weight_in.write(weight_data);
    wait();

    for (int i=0; i< (int) act_list.size(); i++) {
        _act = act_list[i];
        _acc = accum_list[i];
        act_in.write(_act);
        accum_in.write(_acc);
        wait(); 
    } // for  
    wait(5); 
  }// void run()

  void pop_result() {
    wait();

    unsigned int i = 0, j = 0;
    bool correct = 1;
    while (1) {    
        SysPE::InputType tmp;
        if (weight_out.nb_read(tmp)) {
           cout << sc_time_stamp() << ": Received Output Weight:" << " \t " << tmp << endl;
        }
        if (act_out.nb_read(act_out_src)) { 
           cout << sc_time_stamp() << ": Received Output Activation:" << " \t " << act_out_src << "\t| Reference \t" << act_list[i] << endl;
           correct &= (act_list[i] == act_out_src);
	   i++;
        }
        if (accum_out.nb_read(accum_out_src)) {
           int acc_ref = accum_list[j] + act_list[j]*weight_data;
           cout << sc_time_stamp() << ": Received Accumulated Output:" << "\t " << accum_out_src << "\t| Reference \t" << acc_ref << endl;
           correct &= (acc_ref == accum_out_src);
	   j++;
        }
        wait();
      if (i == act_list.size() && j == act_list.size()) break;  
    }// while
    
   if (correct == 1) cout << "Implementation Correct :)" << endl;
   else  cout << "Implementation Incorrect (:" << endl;
  }// void pop_result()
};

SC_MODULE (testbench) {
  sc_clock clk;
  sc_signal<bool> rst;

  sc_fifo<SysPE::InputType> act_in;
  sc_fifo<SysPE::InputType> act_out;
  sc_fifo<SysPE::InputType> weight_in;
  sc_fifo<SysPE::InputType> weight_out;
  sc_fifo<SysPE::AccumType> accum_in;
  sc_fifo<SysPE::AccumType> accum_out;

  SysPE PE;
  Source src;

  SC_HAS_PROCESS(testbench);
  testbench(sc_module_name name)
  : sc_module(name),
    clk("clk", 1, SC_NS, 0.5,0,SC_NS,true),
    rst("rst"),
    PE("PE"),
    src("src")
  {
    PE.clk(clk);
    PE.rst(rst);
    PE.act_in(act_in);
    PE.act_out(act_out);
    PE.weight_in(weight_in);
    PE.weight_out(weight_out);
    PE.accum_in(accum_in);
    PE.accum_out(accum_out);

    src.clk(clk);
    src.rst(rst);
    src.act_in(act_in);
    src.act_out(act_out);
    src.weight_in(weight_in);
    src.weight_out(weight_out);
    src.accum_in(accum_in);
    src.accum_out(accum_out);

    SC_THREAD(run);
  }

  void run() {
    rst = 1;
    wait(10.5, SC_NS);
    rst = 0;
    cout << "@" << sc_time_stamp() << " Asserting Reset " << endl ;
    wait(1, SC_NS);
    cout << "@" << sc_time_stamp() << " Deasserting Reset " << endl ;
    rst = 1;
    wait(10000,SC_NS);
    cout << "@" << sc_time_stamp() << " Stop " << endl ;
    sc_stop();
  }
};



int sc_main(int argc, char *argv[])
{
  testbench my_testbench("my_testbench");
  sc_start();
  return 0;
};
*/