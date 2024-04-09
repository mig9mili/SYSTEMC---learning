#ifndef _TRAGET_H_
#define _TRAGET_H_

#include "utilities.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"

// target module can handle  two pipelined transcations

// you can define additional, custom phases using the DECLARE_EXTENDED_PHASE macro,it is inherits from tlm::tlm_phase
// It defines a static get_phase method that returns a reference to an instance of the new phase
//  TLM_DECLARE_EXTENDED_PHASE(internal_ph); is the macro that is used to define a new phase called internal_ph

// DECLARE_EXTENDED_PHASE(internal_ph);

SC_MODULE(TARGET)
{
  int n_trans;
  bool response_in_progress;
  tlm::tlm_generic_payload *next_response_pending;
  tlm::tlm_generic_payload *end_req_pending;
  tlm_utils::peq_with_cb_and_phase<TARGET> m_peq;
  tlm_utils::simple_target_socket<TARGET> socket;

  SC_CTOR(TARGET) : socket("socket"), n_trans(0), response_in_progress(false), next_response_pending(0), end_req_pending(0), m_peq(this, &TARGET::peq_cb)
  {

    socket.register_nb_transport_fw(this, &TARGET::nb_transport_fw); // registering the callback funtion
  }

  virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload & trans, tlm::tlm_phase & phase, sc_time & delay)
  {
    sc_dt::uint64 adr = trans.get_address();
    unsigned int len = trans.get_data_length();
    unsigned char *byt = trans.get_byte_enable_ptr();
    unsigned int wid = trans.get_streaming_width();

    // checking the transaction
    if (byt != 0)
    {
      trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
      return tlm::TLM_COMPLETED;
    }

    if (len > 4 || wid < len)
    {
      trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
      return tlm::TLM_COMPLETED;
    }
    // now qeue the transaction until the annotated time has elapsed
    m_peq.notify(trans, phase, delay);
    return tlm::TLM_ACCEPTED;
  }

  void peq_cb(tlm::tlm_generic_payload & trans, const tlm::tlm_phase &phase)
  {
    tlm::tlm_sync_enum status; // this holds -- > TLM_ACCEPTED, TLM_UPDATED, TLM_COMPLETED
    sc_time delay;

    switch (phase)
    {
    case tlm::BEGIN_REQ:
#ifdef DEBUG
      fout << hex << trans.get_address() << "BEGIN_REQ at" << sc_time_stamp() << endl;
#endif
      // Increment the transaction reference count
      trans.acquire();
      // Put back-pressure on initiator by deferring END_REQ until pipeline is clear

      // In the given code, n_trans is used to keep track of the number of ongoing transactions. When a new transaction begins, n_trans is incremented. When a transaction ends, n_trans is decremented.
      // The check if (n_trans == 2) is used to apply back-pressure on the initiator by deferring the END_REQ phase until the pipeline is clear.
      // This means that if there are already two transactions being processed (n_trans == 2), the target will not accept any more transactions until one of the ongoing transactions is completed.
      if (n_trans == 2)
      {
        end_req_pending = &trans;
      }
      else
      {
        status = send_end_req(trans);
        if (status == tlm::TLM_COMPLETED)
        {
          break;
        }
      }
      break;

    case tlm::END_RESP:
      // // On receiving END_RESP, the target can release the transaction
      // and allow other pending transactions to proceed

#ifdef DEBUG
      fout << hex << trans.get_address() << "END_RESP_at" << sc_time_stamp() << endl;
#endif
      //If response_in_progress is false when the END_RESP phase is received, it means that the target is not currently sending a response. This is considered an illegal state, as the END_RESP phase should only be received when a response is in progress. Therefore, a fatal error is reported in this case.
      if (!response_in_progress)
        SC_REPORT_FATAL("TLM-2", "Illegal transaction phase END_RESP received  by target");
      trans.release();
      n_trans--;

      // Target itself is now clear to issue  the next BEGIN_RESP
      response_in_progress = false;
      if (next_response_pending)
      {
        send_response(*next_response_pending);
        next_response_pending = 0;
      }

      if (end_req_pending)
      {
        status = send_end_req(*end_req_pending);
        end_req_pending = 0;
      }
      break;

    case tlm::END_REQ:
    case tlm::BEGIN_RESP:
      SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received by target");
    }
  }


  //the send_end_req function is used to send the END_REQ phase of a transaction from the target to the initiator in a Transaction Level Modeling 
   tlm::tlm_sync_enum send_end_req(tlm::tlm_generic_payload& trans){
    tlm::tlm_sync_enum status;
    tlm::tlm_phase bw_phase;
    sc_time delay;
    
    //// Queue the acceptance and the response with the appropriate latency
    response_in_progress = false;
    bw_phase = tlm::END_REQ;
    delay = SC_ZERO_TIME;
    status = socket->nb_transport_bw(trans,bw_phase,delay);
    if(status == tlm::TLM_COMPLETED){
      // Transaction aborted by the initiator
      // (TLM_UPDATED cannot occur at this point in the base protocol, so need not be checked)
      trans.release();
      return status;
    } 
    return status;

   }

   void send_response(tlm::tlm_generic_payload& trans){
      tlm::tlm_sync_enum status;
      tlm::tlm_phase bw_phase;
      sc_time delay;

      response_in_progress = true;
      bw_phase = tlm::BEGIN_RESP;
      delay =SC_ZERO_TIME;
      status = socket->nb_transport_bw(trans,bw_phase,delay);

      if(status == tlm::TLM_UPDATED){
        // The timing annotation must be honored
        m_peq.notify(trans,bw_phase,delay);
      }
      else if(status == tlm::TLM_COMPLETED){
        // The initiator has terminated the transaction
        trans.release();
        n_trans -- ;
        response_in_progress =false;
      }

   }

};

#endif