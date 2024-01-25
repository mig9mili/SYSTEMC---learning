#include<systemc>
#include"driver.h"
#include"moniter.h"
#include"notgate.h"
#include<iostream>



using namespace sc_core;
using namespace std;


int sc_main(int ,char*[]){
    //signals
    sc_signal<bool> s_in,s_out;


    // instance created(i.e object created)

    NOT ng("not_gate");
    driver dr("driver");
    moniter mon("mon");

    //interconnects


    ng.in(s_in);
    dr.d_din(s_in);  //driver input
    
    mon.m_din(s_in);
    
    ng.out(s_out);
    mon.m_s_out(s_out);

    //tracing

    sc_trace_file *fp;
    fp = sc_create_vcd_trace_file("vcd_trace");
    fp->set_time_unit(1,SC_SEC);

    sc_trace(fp,s_in,"INPUT");
    sc_trace(fp,s_out,"OUTPUT");

    //simulation

    sc_start(20,SC_SEC);
    sc_close_vcd_trace_file(fp);

    return 0;
}