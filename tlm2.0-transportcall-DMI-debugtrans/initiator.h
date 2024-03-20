#ifndef INITIATOR_H_
#define INITIATOR_H_

#include<systemc>
using namespace sc_core;
using namespace sc_dt;
using namespace std;
#include<tlm.h>
#include<tlm_utils/simple_initiator_socket.h>



//initiator module  generators generic payload transaction
SC_MODULE(Initiator){
    // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_initiator_socket<Initiator> socket;
  SC_CTOR(Initiator):socket("socket"),dmi_ptr_valid(false){
      // Register callbacks for incoming interface method calls
      socket.register_invalidate_direct_mem_ptr(this,&Initiator::invalidate_direct_mem_ptr); //This means that it's telling the socket object: "Hey, when you need to invalidate the direct memory pointer, call the invalidate_direct_mem_ptr method on this instance of the Initiator class (this)."
      
      SC_THREAD(thread_process);
    
  }

  void thread_process(){
    //TLM-2 generic payload transaction, reused across calls to b_transport, DMI and debug
    tlm :: tlm_generic_payload * trans = new tlm :: tlm_generic_payload;
    sc_time delay = sc_time(10, SC_NS);

    // Generate a random sequence of reads and writes
    for(int i=0;i<128;i+=4){
      int data;
      //static_cast in C++. A Cast operator is a unary operator which forces one data type to be converted into another data type
      //static_cast <dest_type> (source);
      tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand()%2);
      if (cmd == tlm::TLM_WRITE_COMMAND) data = 0xFF000000|i;

     // Use DMI if it is available, reusing same transaction object

     if(dmi_ptr_valid){
      // Bypass transport interface and use direct memory interface
      // Implement target latency
      if(cmd == tlm:: TLM_READ_COMMAND){
        //assert macro in C++ is used for debugging purposes to check a condition that should always be true at that point in the code. If the condition is false, assert will cause the program to terminate immediately, printing an error message that includes the file name and line number where the assert statement is located.
        assert(dmi_data.is_read_allowed());
        //void *memcpy(void *to, const void *from, size_t numBytes);
        memcpy(&data,dmi_data.get_dmi_ptr()+i,4);
        wait(dmi_data.get_read_latency());
      }
      else if(cmd = tlm::TLM_WRITE_COMMAND){
        assert(dmi_data.is_write_allowed());
        memcpy(dmi_data.get_dmi_ptr()+i,&data,4);
        wait(dmi_data.get_write_latency());
      }
      cout<<"dmi = {"<<(cmd ? 'W':'R')<< " , " << hex << i << " } , data = "<< hex << data << "at time " <<endl;
     }
     else { 
      //transport call
        trans -> set_command(cmd);
        trans -> set_address(i);
      // reinterpret_cast:- It is used to convert a pointer of some data type into a pointer of another data type, even if the data types before and after conversion are different.
        trans -> set_data_ptr(reinterpret_cast<unsigned char*> (&data));
        trans -> set_data_length(4);
        trans -> set_streaming_width(4);// = data_length to indicate no streaming
        trans -> set_byte_enable_ptr(0);// 0 indicates unused
        trans -> set_dmi_allowed(false); //mand
        trans -> set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);//mand

        #ifdef INJECT_ERROR
             if(i>90) trans ->set_streaming_width(2);
        #endif
        // Other fields default: byte enable = 0, streaming width = 0, DMI_hint = false, no extensions
         
        socket ->b_transport(*trans , delay); //blocking transport call

        if(trans -> is_response_error()){
         char txt[100];
         //instead the  print to console it will store it to the txt.
         sprintf(txt, "error from b_tranport , response status = %s ",trans ->get_response_string().c_str());
         SC_REPORT_ERROR("TLM-2",txt);
        }


           // Check DMI hint  
           // why ? So, even though DMI access was not available at the current transaction, if the DMI hint suggests it might become available soon, it could be beneficial to try to switch to DMI access later
        if(trans -> is_dmi_allowed()){
          // Re-user transaction object for DMI

          //If DMI is allowed for the transaction, the code then initializes the DMI data structure dmi_data by calling its init() method.
          //This is likely resetting the dmi_data to its initial state, ready to be used for a new DMI transaction.
          dmi_data.init(); 
          //If DMI is allowed for the transaction, the code then initializes the DMI data structure dmi_data by calling its init() method.
          //This is likely resetting the dmi_data to its initial state, ready to be used for a new DMI transaction.
          //by calling the get_direct_mem_ptr() method on the socket object, passing in the transaction object trans and the dmi_data structure. 
          //The get_direct_mem_ptr() method is expected to fill dmi_data with the details of the direct memory access and return a boolean indicating whether the operation was successful.
          //This return value is stored in the dmi_ptr_valid variable.
          //So, if dmi_ptr_valid is true, it means that the DMI pointer was successfully retrieved and can be used for direct memory access. If it's false, 
          //it means that the operation was not successful, and the DMI pointer should not be used
          dmi_ptr_valid = socket ->get_direct_mem_ptr(*trans , dmi_data);
        }

        cout << "trans = { " <<(cmd ? 'W ':'R ')<< hex << i <<"} , data = "<< hex << data<< "at time "<<  sc_time_stamp()<<"delay = "<< delay <<endl;

     }

    } 

    // Use debug transaction interface to dump memory contents, reusing same transaction object
    trans->set_address(0);
    trans->set_read();
    trans->set_data_length(128);

    unsigned char* data = new unsigned char[128];
    trans-> set_data_ptr(data);

    unsigned int n_bytes = socket->transport_dbg(*trans);

    for(unsigned int i =0 ; i <n_bytes;i+=4){
      cout<<"mem["<<i<<"] = " <<*(reinterpret_cast<unsigned int*>(&data[i]))<<endl;
    
    }
  }
    

    //The purpose of the invalidate_direct_mem_ptr function in the initiator.h 
    //file is to invalidate all direct memory pointers (DMI) regardless of the specified range.
  virtual void invalidate_direct_mem_ptr(uint64 start_range,uint64 end_range){
    // Ignore range and invalidate all DMI pointers regardless
    dmi_ptr_valid= false;

  }
  bool dmi_ptr_valid;
  tlm::tlm_dmi dmi_data;

}; 


#endif