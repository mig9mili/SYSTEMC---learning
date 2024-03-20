#ifndef INITIATOR_H_
#define INITIATOR_H_


#include<systemc>
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include<tlm.h>
#include<tlm_utils/simple_initiator_socket.h>

using namespace tlm_utils;

SC_MODULE(Initiator){

    simple_initiator_socket<Initiator>socket;
    SC_CTOR(Initiator):socket("socket"){
        SC_THREAD(thread_process);  
    }
    void thread_process(){
    tlm :: tlm_generic_payload *trans = new tlm::tlm_generic_payload;
    sc_time delay = sc_time(10,SC_NS);

    for(int i=0 ; i<96 ; i+=4){

        tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand()%2);
        if(cmd == tlm::TLM_WRITE_COMMAND) data = 0xFF000000 | i;

        trans -> set_command(cmd);//sets the command for the transaction. The cmd variable is of type tlm::tlm_command and can be either TLM_READ_COMMAND or TLM_WRITE_COMMAND, indicating a read or write operation respectively.
        trans -> set_address(i); //sets the address for the transaction. This is the memory address that the read or write operation will act upon.
        trans -> set_data_ptr(reinterpret_cast<unsigned char*>(&data));//sets the data pointer for the transaction. This is a pointer to the data that will be written in a write operation, or where the data will be stored in a read operation. The reinterpret_cast is used to convert the int* pointer to an unsigned char*, as TLM transactions work with byte arrays.
        trans -> set_data_length(4); //sets the data length for the transaction. This is the number of bytes that the read or write operation will act upon. Here it's set to 4, indicating a 4-byte or 32-bit operation.
        trans -> set_streaming_width(4); // sets the streaming width for the transaction. This is used for burst transactions, where multiple data items are transferred in one go. Setting it equal to the data length indicates that there's no streaming, i.e., it's a single data item transaction.
        trans -> set_byte_enable_ptr(0); //sets the byte enable pointer for the transaction. This can be used to selectively enable or disable bytes in the data payload for the transaction. Setting it to 0 indicates that all bytes are enabled.
        trans -> set_dmi_allowed(false); //sets the Direct Memory Access (DMI) hint for the transaction. This can be used to indicate that the initiator module (the one sending the transaction) allows the target module (the one receiving the transaction) to return a DMI pointer. Setting it to false indicates that DMI is not allowed.
        trans -> set_response_status(tlm :: TLM_INCOMPLETE_RESPONSE); //sets the initial response status for the transaction. This is used to indicate whether the transaction has been successfully completed or not. Setting it to TLM_INCOMPLETE_RESPONSE indicates that the transaction is yet to be completed.
        

        socket->b_transport(*trans,delay);

        if (trans -> is_response_error()){
            SC_REPORT_ERROR("TLM-2","response error from b_transport");
        }

        cout<< " trans = { " << (cmd ? 'w':'r')<<", "<< hex << i << " } , data = " << hex <<data << "at time " << sc_time_stamp()<<"delay = "<< delay <<endl;
       // Realize the delay annotated onto the transport call
      wait(delay);
    }
}
  int data;
};
#endif