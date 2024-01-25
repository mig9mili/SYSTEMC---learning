#include<systemc>
#include"dff.h"
#include"driver.h"
#include"moniter.h"

int sc_main(int , char*[]){
    //ports and signals ;
    sc_signal<bool> s_din , s_dout, clear;
    sc_clock clock("clk",10,SC_NS,0.5,false);

    //instance
    dff cell("dflipflop");
    driver dr("driver");
    moniter mon("moniter");


    cell.clk(clock);

    cell.clr(clear);
    dr.d_clr(clear);
    mon.m_clr(clear);

    dr.d_din(s_din);
    cell.din(s_din);
    mon.m_din(s_din);


    cell.dout(s_dout);
    mon.m_dout(s_dout);

    //tracking

 sc_trace_file *fp;
 fp=sc_create_vcd_trace_file("vcd_trace");
 fp->set_time_unit(100,SC_PS);

 sc_trace(fp,s_din,"INPUT");
 sc_trace(fp,clock,"clk");
 sc_trace(fp,clear,"clr");
 sc_trace(fp,s_dout,"output");

 //simulation starts

 sc_start(80,SC_NS);
 sc_close_vcd_trace_file(fp);
 return 0;

}