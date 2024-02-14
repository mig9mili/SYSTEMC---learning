#include"testbench.h"

vector<sc_int<8>> act_list{0,-1,3,-7,15,-31,63,-127};
vector<sc_int<32>> acc_list{0,-10,30,-70,150,-310,630,-1270};
sc_int<8> weight_data =10;

void tb :: source(){

    sc_int<8> _act;
    sc_int<32>_acc;

    //reset 
    wt_in.write(0);
    wt_in_vld.write(0);
    /*rst.write(1);
    wait();
    rst.write(0);
    wait();*/


    act_in.write(0);
    act_in_vld.write(0);
    /*rst.write(1);
    wait();
    rst.write(0);
    wait();*/


    acc_in.write(0);
    acc_in_vld.write(0);
    /*rst.write(1);
    wait();
    rst.write(0);
    wait();/*/

   

 // write waight to PE 

    //hand check weight
    wt_in_vld.write(1);
     wt_in.write(weight_data);  //write value
     do{
      wait(); 
     }while(!wt_in_rdy.read()); // wait until true
     wt_in_vld.write(0);  //assert vald signal
    
    
   
    for(int i=0;i<act_list.size();i++){

        _act=act_list[i];
        _acc=acc_list[i];
            

            //hand check for activation
            act_in_vld.write(1);
            act_in.write(_act);  //write value
            do{
             wait(); 
            }while(!act_in_rdy.read()); // wait until true
            act_in_vld.write(0);
            
            //hand check for accumalation
            acc_in_vld.write(1);
            acc_in.write(_acc); //write value
            do{
             wait(); 
            }while(!acc_in_rdy.read()); // wait until true
            acc_in_vld.write(0);
    
        
    }
}


void tb :: sink(){
    sc_int<32> acc_init =0;
    sc_int<8> act_out_src;
    sc_int<32> acc_out_src;
    unsigned int i=0,j=0;
    bool correct =1;
   
   //intialize the port
    wt_outp_rdy.write(0);
    act_outp_rdy.write(0);
    acc_outp_rdy.write(0);
    while(1){

        sc_int<8> tmp;

        //hand check wt
        wt_outp_rdy.write(1);
        do{
            wait();
        }while(!wt_outp_vld.read());
        if(wt_out.read()){
        tmp=wt_out.read();
            cout<<sc_time_stamp()<< " recived output weight"<<"\t"<<tmp<<endl;
        }
        wt_outp_rdy.write(0);


        //hand check act
        act_outp_rdy.write(1);
        do{
            wait();
        }while(!act_outp_vld.read());
        if(act_out.read()){
            act_out_src=act_out.read();
            cout<<sc_time_stamp()<< "recived output activation"<<"/t"<<act_out_src<<"\t reference \t"<<act_list[i]<<endl;
            correct &= (act_list[i]== act_out_src);
            i++;
        }
        act_outp_rdy.write(0);

     // hand check for acc
      acc_outp_rdy.write(1);
        do{
            wait();
        }while(!acc_outp_vld.read());
         if(acc_out.read()){
            acc_out_src=acc_out.read();
            int acc_ref = acc_list[j]+act_list[j]*weight_data;
            cout<<sc_time_stamp()<<": recevied accumalated output: "<<"\t"<<acc_out_src<<"\t |reference \t"<<acc_ref<<endl;
            correct &= (acc_ref == acc_out_src);
            j++;
        }
        acc_outp_rdy.write(0);
       
       
        wait();
        if(i == act_list.size() && j==act_list.size()) break;
    }
    if(correct ==1 )cout << "implementation correct :)"<<endl;
    else cout << "implentation Incorrect (:"<<endl;
  sc_stop();
}