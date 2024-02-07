#include"fir.h"



// coefficients for each fir 

const sc_uint<8> coef[5] = {18,77,107,77,18};

//fir main thread

void fir :: fir_main(void){

    sc_int<16> taps[5];
    
   
   //reset protocol
    for(int i =0;i<5;i++){
        taps[i]=0;
    }

    //intialize the handshake
    in_rdy.write(0);
    outp_vld.write(0);
    out.write(0);
    wait();

    // fir algorithm
    while(true){
        sc_int<16> in_val;
        sc_int<16> out_val;
        
        //ready for new data
        in_rdy.write(1);
        do {
            wait();
        }while(!in_vld.read());
        in_val = in.read();
        in_rdy.write(0);


    /// read  input  data  into shift register
        for(int i =5-1;i>0;i--){
            taps[i] = taps[i-1];
        }

        taps[0] = in_val;
   // perform  multiply and accumulate
        for(int i =0;i<5;i++){
          out_val += coef[i]*taps[i];
        }
 


        outp_vld.write(1);
        out.write(out_val);
        do{
            wait();
        }while(!outp_rdy.read());
        outp_vld.write(0);
    }
}

