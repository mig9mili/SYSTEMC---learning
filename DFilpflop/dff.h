#ifndef DFF_H_
#define DFF_H_
#include<systemc>
using namespace sc_core;
#include"nandgate.h"
#include"notgate.h"

SC_MODULE(dff){

    sc_in<bool> clk{"clock"} , clr{"clear"}, din{"Input"};  //input to d flip flop
    sc_out<bool> dout{"output"};

    NAND n1 ,n2,n3 ,n4;
    NOT  ng;
    sc_signal<bool> s_ng_2,s1,s2,s3,s4;

    void diff_process(void){
        if(clr){
            dout = 0;
        }
        else{
            dout = n3.out;
        }
    }

    ~dff(){

    }

    SC_CTOR(dff): n1("nand1"),n2("nand2"),n3("nand3"),n4("nand4"),ng("NOT"){
        //din      
        n1.A(din); // input to nand1
        ng.in(din); // input to not


        //s_ng_n2
        ng.out(s_ng_2);  // op to not 
        n2.A(s_ng_2);    // input to nand2

        n1.B(clk);  
        n2.B(clk);


        n1.out(s1);
        n3.A(s1);

        n2.out(s2);
        n4.A(s2);

        n3.out(s3);
        n4.B(s3);

        n4.out(s4);
        n3.B(s4);




       SC_METHOD(diff_process);
       sensitive << clr << clk.pos();


    }
};

#endif /*DFF_H_*/


