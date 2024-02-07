#include"tb.h"
using namespace std;



void tb::source(){
    //reset
    in.write(0);
    in_vld.write(0);
    rst.write(1);
    wait();
    rst.write(0);
    wait();

    sc_int<16> tmp ;   // tmp->>in

    //send input value to FIR
    for(int i =0;i<64;i++){
        if(i>23 && i<29)
          tmp = 256;
        else
           tmp=0;

     in_vld.write(1);
     in.write(tmp);  //write value
     do{
        wait(); 
     }while(!in_rdy.read()); // wait until true
     in_vld.write(0);  //assert vald signal
    }
}

void tb :: sink(){
    sc_int<16> indata;
    
    // intialize the  port
    outp_rdy.write(0);

    //read output coming form DUT
    for(int i =0 ; i<64;i++){
        outp_rdy.write(1);
        do{
            wait();
        }while(!outp_vld.read());
        indata = out.read();
        outp_rdy.write(0);
        wait();
        cout<<i<<":\t"<<indata.to_int()<<endl;
    }

    sc_stop();
}

//end simulation