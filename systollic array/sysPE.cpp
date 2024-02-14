 #include"sysPE.h"
 #include<iostream>
 using namespace std;

 
 
 void SYSPE :: PERUN(){


//intializinting the handshacking
  wt_in_rdy.write(0);
  act_in_rdy.write(0);
  acc_in_rdy.write(0);
  wt_outp_vld.write(0);
  act_outp_vld.write(0);
  acc_outp_vld.write(0);
  wt_out.write(0);
  act_out.write(0);
  acc_out.write(0);
  wait();

  bool is_wt_in,is_act_in,is_acc_in;

 // stores the current input value and accumalator value
    sc_int<8> wt_reg,wt_out_reg;
    sc_int<8> act_reg;
    sc_int<32> acc_reg;
    sc_int<32> acc_out_reg;
    
   is_wt_in=0;
   is_act_in=0;
   is_acc_in=0;
 

  wt_reg =0;
  wt_out_reg =0;
  act_reg =0;
  acc_reg=0;
  acc_out_reg;

  while(1){

  // ready for new data;
   wt_in_rdy.write(1);
        do {
            wait();
        }while(!wt_in_vld.read());
    if((wt_in.read())){ //trying to read form sc_out
     // cout<<sc_time_stamp()<<endl;
      wt_reg= wt_in.read();
      is_wt_in=1;
      wt_out_reg = wt_reg; 
    }
    wt_in_rdy.write(0);

    act_in_rdy.write(1);
        do {
            wait();
        }while(!act_in_vld.read());
      if(act_in.read()){
     //cout<<sc_time_stamp()<<endl;
       act_reg= act_in.read();
       //cout<<act_reg;
       is_act_in=1;
      }
      act_in_rdy.write(0);

    acc_in_rdy.write(1);
        do {
            wait();
        }while(!acc_in_vld.read());
    if(acc_in.read()){
      acc_reg=acc_in.read();
      is_acc_in = 1;
    }
    acc_in_rdy.write(0);


    if(is_act_in && is_acc_in){
      is_act_in=0;
      is_acc_in=0;

      
      acc_out_reg=(act_reg * wt_reg) + acc_reg;

      act_outp_vld.write(1);
        act_out.write(act_reg);
        do{
            wait();
        }while(!act_outp_rdy.read());
       act_outp_vld.write(0);
      

      acc_outp_vld.write(1);
        acc_out.write(acc_out_reg);
        do{
            wait();
        }while(!acc_outp_rdy.read());
        acc_outp_vld.write(0);
      
    }

    if (is_wt_in == 1){
      is_wt_in =0 ;
      wt_outp_vld.write(1);
       wt_out.write(wt_out_reg);
        do{
            wait();
        }while(!wt_outp_rdy.read());
        wt_outp_vld.write(0);
    }
      
    }

}
