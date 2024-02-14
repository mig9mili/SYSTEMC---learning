#include<systemc>
#include"sysPE.h"
#include"testbench.h"

using namespace sc_core;
using namespace std;



SC_MODULE(SYSTEM){
    tb *tb0;
    SYSPE *pe0;

//instances  
    sc_signal<bool> rst_sig;

    sc_signal<sc_int<8>> wt_in;
    sc_signal<sc_int<8>> act_in;
    sc_signal<sc_int<32>> acc_in;
    sc_signal<bool> wt_inp_sig_vld;
    sc_signal<bool> wt_inp_sig_rdy;

    sc_signal<sc_int<8>> wt_out;
    sc_signal<sc_int<8>> act_out;
    sc_signal<sc_int<32>> acc_out;
    sc_signal<bool> wt_out_sig_vld;
    sc_signal<bool> wt_out_sig_rdy;


    sc_clock clk_sig;
    
    SC_CTOR(SYSTEM):clk_sig("clk",10, SC_NS){
     
        tb0 = new tb("tb0"); // dynamic allocation
        tb0->clk(clk_sig);
        tb0->rst(rst_sig);

        tb0->wt_in(wt_in);
        tb0->act_in(act_in);
        tb0->acc_in(acc_in);
        tb0->wt_in_vld(wt_inp_sig_vld);
        tb0->wt_in_rdy(wt_inp_sig_rdy);
        
        tb0->wt_out(wt_out);
        tb0->act_out(act_out);
        tb0->acc_out(acc_out);
        tb0->wt_outp_vld(wt_out_sig_vld);
        tb0->wt_outp_rdy(wt_out_sig_rdy);

        pe0 = new SYSPE("pe0");
        pe0->clk(clk_sig);
        pe0->rst(rst_sig);

        pe0->wt_in(wt_in);
        pe0->act_in(act_in);
        pe0->acc_in(acc_in);
        pe0->wt_in_vld(wt_inp_sig_vld);
        pe0->wt_in_rdy(wt_inp_sig_rdy);

        
    
        pe0->wt_out(wt_out);
        pe0->act_out(act_out);
        pe0->acc_out(acc_out);
        pe0->wt_outp_vld(wt_out_sig_vld);
        pe0->wt_outp_rdy(wt_out_sig_rdy);

      


       // SC_THREAD(run);
       //reset_signal_is(rst_sig,true);


    }
    /*
     void run(){
        rst_sig = 1;
        wait(10,SC_NS);
        rst_sig = 0;
        cout << "@" << sc_time_stamp() << " arrerting reset" << endl;
        wait(10,SC_NS);
        cout << "@"<< sc_time_stamp() << " dearrerting reset"<<endl;
        rst_sig =1;
        wait(1000000,SC_NS);
        cout<<"@"<<sc_time_stamp() << "stop"<<endl;
        sc_stop();
    }
    */
    
};

SYSTEM *top = NULL;

int sc_main(int,char*[]){
    top = new  SYSTEM("top");
    sc_start();
    return 0;  
}