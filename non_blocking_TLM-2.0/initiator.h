#ifndef INTIATOR_H_
#define INTIATOR_H_

#include"utilities.h"
#include"tlm_utils/simple_initiator_socket.h"
#include"tlm_utils/peq_with_cb_and_phase.h"


SC_MODULE(Initiator){
    mm m_mm;
    int data[16];
    tlm::tlm_generic_payload *request_in_progress;
    sc_event end_request_event;
    tlm_utils::peq_with_cb_and_phase<Initiator> m_peq;  ///"Payload Event Queue with Callback and Phase" 
    tlm_utils::simple_initiator_socket<Initiator> socket;

    SC_CTOR(Initiator): socket("socket"),request_in_progress(0),m_peq(this, &Initiator::peq_cb){
        //the non-blocking transport interface is used for communication between components where the sender does not wait for the receiver 
        //to finish processing the transaction
       
       //this --> this refers to the current instance of the Initiator class.
       //&Initiator::peq_cb --> his is a pointer to the peq_cb member function of the Initiator class. This function will be called when a non-blocking transaction is initiated from the target to the initiator.
       // (this is the callback function that is called by target when the some information to send to the intiator )
        socket.register_nb_transport_bw(this, &Initiator::nb_trnasport_bw);//registering a callback function for the non-blocking transport interface in the backward direction (from target to initiator).
        SC_THREAD(thread_process);
    }

    void thread_process(){
        tlm::tlm_generic_payload * trans;
        tlm::tlm_phase phase;
        sc_time delay;

        for(int i =0 ;i< 1000;i++){
            int adr = rand();
            tlm::tlm_command cmd =static_cast<tlm :: tlm_command> (rand()%2);

            if(cmd == tlm::TLM_WRITE_COMMAND){
                data[i%16] = rand();
            }
             // grab a new trans from memory manager 
            trans = m_mm.allocate();
            //acquire () funtion checks if the m_mm is not null and increments the reference count m_ref_count varibale.
            // it seems  to be apart memory management system, aquire() is called to indiacte that a new reference to resouce is being created.
            trans->acquire();  // must be called before initilizing the transaction to correctly set the reference count 


            trans->set_command(cmd);
            trans->set_address(adr);
            trans->set_data_ptr(reinterpret_cast<unsigned char*> (&data[i%16]));
            trans->set_data_length(4);
            trans->set_streaming_width(4);
            trans->set_byte_enable_ptr(0);
            trans->set_dmi_allowed(false);
            trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

            //initiator must honor BEGIN_REQ AND END_REQ exclusion rule 
            if(request_in_progress){ // if there is a request in progress 
                wait(end_request_event); // wait state until the end request event is triggered
            }
            request_in_progress = trans;
            phase = tlm::BEGIN_REQ;
          
          //timing annotation models processing time of intiator prior to call

          delay = sc_time(rand_ps(),SC_PS);
          fout << hex << adr <<"new , cmd= " << (cmd ? 'W':'R')<<", data= "<<hex << data[i%16]<<"at time "<<sc_time_stamp() <<endl;

          // non-blocking transport call on forward path
          tlm :: tlm_sync_enum status;
          status = socket->nb_transport_fw(*trans , phase,delay);

          //check  value returned from nb transport_fw;

          if(status == tlm::TLM_UPDATED){

            //timing  annotation  must be honored

            m_peq.notify(*trans , phase , delay);

          }

          else if(status == tlm :: TLM_COMPLETED){
             
             // completion  of transcation necessarly ends the BEGIN_REQ phase
             request_in_progress = 0;
             check_transaction (*trans);
          }
          
              wait(sc_time(rand_ps(), SC_PS));
        }
         wait(100, SC_NS);
    }


  //it allows the transaction to be sent from the target to the initiator in a non-blocking manner.
   virtual tlm::tlm_sync_enum nb_trnasport_bw(tlm::tlm_generic_payload &trans,tlm::tlm_phase& phase,sc_time& delay){
          // The timing annotation must be honored
         m_peq.notify(trans,phase,delay);
         return tlm::TLM_ACCEPTED;
   }
  
     // Transaction could have arrived through return path or backward
    // Transaction could have arrived through return path or backward path

  void peq_cb(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase){
        #ifdef DEGUB
        if(phase == tlm::END_REQ)
        fout<< hex << trans.get_address() << "END_REQ at "<< sc_time_stamp() << endl;
        else if(phase == tlm::BEGIN_RESP)
        fout << hex <<trans.get_address() << "BEGIN_RESP at"<<sc_time_stamp()<< endl;
        #endif
       
       if(phase == tlm::END_REQ || (&trans == request_in_progress && phase == tlm::BEGIN_RESP)){
          request_in_progress = 0;
          end_request_event.notify();
       }
       else if (phase ==tlm::BEGIN_REQ || phase ==tlm::END_RESP){   // the phase is BEGIN_REQ (beginning of request) or END_RESP (end of response), it reports a fatal error. These phases should not be received by the initiator in this context.
          SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received by initiator");
       }

       if(phase == tlm::BEGIN_RESP){  //If the phase is BEGIN_RESP, it first checks the transaction for errors using the check_transaction function. Then, it sends an END_RESP (end of response) phase to the target, signifying that it has received and processed the response. The delay for this transaction is randomly generated using the rand_ps function.

         check_transaction(trans);

        // send final phase transition to target
        tlm::tlm_phase fw_phase = tlm::END_RESP;
        sc_time delay = sc_time(rand_ps() , SC_PS);
        socket->nb_transport_fw(trans,fw_phase, delay);
        //ignore return value
       }
  }
 
 void check_transaction(tlm::tlm_generic_payload& trans){
    if(trans.is_response_error()){
        char txt[100];
        sprintf(txt,"Transaction return with error, response status = %s", trans.get_response_string().c_str());
        SC_REPORT_ERROR("TLM_-2", txt);
    }
    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64 adr = trans.get_command();
    int*   ptr = reinterpret_cast<int*>(trans.get_data_ptr());
    fout << hex <<  adr <<"check cmd = "<<(cmd ? 'W':'R')<<" data= " << hex << *ptr << " at the time "<< sc_time_stamp()<< endl;

     trans.release();
 }

};

#endif