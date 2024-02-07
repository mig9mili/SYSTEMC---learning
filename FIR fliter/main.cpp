#include<systemc>
#include"fir.h"
#include"tb.h"


using namespace sc_core;
using namespace std;

SC_MODULE(SYSTEM){
    //lower lvl module;  //name of module  followed by pointer to its reference
    tb *tb0;
    fir *fir0;
 

 //instances 
    sc_signal<bool> rst_sig;

    sc_signal<sc_int<16>> inp_sig;
    sc_signal<bool> inp_sig_vld;
    sc_signal<bool> inp_sig_rdy;


    sc_signal<sc_int<16>> out_sig;
    sc_signal<bool> out_sig_vld;
    sc_signal<bool> out_sig_rdy;
 
    sc_clock clk_sig;

    SC_CTOR(SYSTEM): clk_sig("clk_sig",10,SC_NS){  // copy constructor

        tb0 = new tb("tb0"); //dynamic  allocation
        tb0->clk(clk_sig);   
        tb0->rst(rst_sig);

        tb0->in(inp_sig);
        tb0->in_vld(inp_sig_vld);
        tb0->in_rdy(inp_sig_rdy);

        tb0->out(out_sig);
        tb0->outp_vld(out_sig_vld);
        tb0->outp_rdy(out_sig_rdy);


       fir0 = new fir("fir0");
       fir0->clk(clk_sig);
       fir0->rst(rst_sig);
       fir0->in(inp_sig);  
       fir0->in_vld(inp_sig_vld);
       fir0->in_rdy(inp_sig_rdy);
       
       fir0->out(out_sig);
       fir0->outp_vld(out_sig_vld);
       fir0->outp_rdy(out_sig_rdy);
        
    }
    ~SYSTEM(){
        
        delete tb0;
        delete fir0;

    }


};

SYSTEM *top = NULL;

int sc_main(int , char*[]){
    top = new SYSTEM("top"); 
    sc_start();
    return 0;
}



