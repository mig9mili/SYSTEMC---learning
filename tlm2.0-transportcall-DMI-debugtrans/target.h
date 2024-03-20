#ifndef TARGET_H_
#define TARGET_H_


#include<systemc>
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

SC_MODULE(Memory)
{
    tlm_utils::simple_target_socket<Memory> socket;
    enum{SIZE = 256};
    const sc_time LATENCY;
  SC_CTOR(Memory):socket("socket"),LATENCY(10,SC_NS){
    
    //register callback for incoming interface method calls

    socket.register_b_transport(this, &Memory::b_transport);
    socket.register_get_direct_mem_ptr(this, &Memory::get_direct_mem_ptr);
    socket.register_transport_dbg(this,&Memory::transport_dbg);
    
    //initialize memory with random data;
    for(int i=0;i<SIZE;i++){
        mem[i] = 0xAA000000|(rand()%256);   
    }

    SC_THREAD(invalidation_process);

  }
  

  // tlm-2 blocking transport method
  virtual void b_transport(tlm::tlm_generic_payload &trans,sc_time& delay){
    tlm::tlm_command cmd = trans.get_command();
    uint64 adr =  trans.get_address()/4;
    unsigned char* ptr = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();
    unsigned char* byt =trans.get_byte_enable_ptr();
    unsigned int  wid = trans.get_streaming_width();


    // Obliged to check address range and check for unsupported features,
    //   i.e. byte enables, streaming, and bursts
    // Can ignore extension
    // Generate the appropriate error response

    if (adr >= uint64(SIZE)){
        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        return;
    }
    if(byt != 0){
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }
    if(len >4 || wid <len){
        trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
        return ;
    }

     //obliged to implement read aand write command
     if(cmd == tlm::TLM_READ_COMMAND){
        memcpy(ptr,&mem[adr],len);
     }
     else if(cmd == tlm::TLM_WRITE_COMMAND){
        memcpy(&mem[adr],ptr,len);
     }
      

      //illustartes that b_transport may block
     wait(delay);

     //reset timing  annotation after waiting 
     delay = SC_ZERO_TIME;

     //Set DMI hint to indicated that DMI is supported
     trans.set_dmi_allowed(true);

    trans.set_response_status(tlm::TLM_OK_RESPONSE);

  }

  //tlm-2 forward DMI Mmethod
  virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload &trans,tlm::tlm_dmi &dmi_data){
    //permit read and wirte access
    dmi_data.allow_read_write();

    //set other details of dmi region
    dmi_data.set_dmi_ptr(reinterpret_cast<unsigned char *>(&mem[0]));
    dmi_data.set_start_address(0);
    dmi_data.set_end_address(SIZE*4-1);
    dmi_data.set_write_latency(LATENCY);
    dmi_data.set_read_latency(LATENCY);

    return true;
  }
  
  void invalidation_process(){
    //invalidate dmi pointer periodically
    for(int i =0 ;i<4;i++){
        wait(LATENCY*8);
        socket->invalidate_direct_mem_ptr(0,SIZE-1);
    }
  }

// TLM-2 debug transport method
  virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans){
    tlm::tlm_command cmd = trans.get_command();
    uint64 adr = trans.get_address()/4;
    unsigned char* ptr = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();

    //calculate the number of bytes to be actually copied 

    unsigned int num_bytes =(len < (SIZE-adr)*4)? len : (SIZE-adr)*4;

    if(cmd == tlm::TLM_READ_COMMAND){
        memcpy(ptr,&mem[adr],num_bytes);
    }
    else if (cmd == tlm::TLM_WRITE_COMMAND){
        memcpy(&mem[adr],ptr,num_bytes);
    }

    return num_bytes;


}

int mem[SIZE];

};

#endif

