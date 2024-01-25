#include<systemc>
#include"driver.h"
#include"moniter.h"
#include"nandgate.h"
using namespace sc_core;
using namespace std;

int sc_main(int , char*[]){

    //test
    //ports and signals
    sc_signal<bool> s_A,s_B,s_out;

   
   //instance created (i.e objects)
    NAND ng("nand_gate");
    driver dr("sriver");
    moniter mon("moniter");
  

  //input 1 to nand
    ng.A(s_A);
    dr.d_A(s_A);
    mon.m_A(s_A);

  // input 2 to nand

   
   dr.d_B(s_B);
   ng.B(s_B);
   mon.m_B(s_B);

 //output  
 ng.out(s_out);
 mon.m_out(s_out);

 //tracking

 sc_trace_file *fp;
 fp=sc_create_vcd_trace_file("vcd_trace");
 fp->set_time_unit(1,SC_NS);

 sc_trace(fp,s_A,"INPUT_A");
 sc_trace(fp,s_B,"INPUT_B");
 sc_trace(fp,s_out,"OUTPUT");

 //simulation starts

 sc_start(20,SC_NS);
 sc_close_vcd_trace_file(fp);
 return 0;
 
}