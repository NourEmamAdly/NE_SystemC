#ifndef TARGET_H
#define TARGET_H

#include <iomanip>
#include <systemc>
#include <tlm.h>
#include <tlm_utils/peq_with_cb_and_phase.h>
#include "../tlm_memory_manager/memory_manager.h"
#include "../tlm_protocol_checker/tlm2_base_protocol_checker.h"
#include "util.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

class Target: public sc_module, public tlm::tlm_fw_transport_if<>
{
    public:
    tlm::tlm_target_socket<> socket; // MK_GPT: Define a TLM target socket

    protected:
    tlm::tlm_generic_payload* transactionInProgress; // MK_GPT: Pointer to the current transaction being processed
    sc_event targetDone; // MK_GPT: Event to signal completion of a transaction
    bool responseInProgress; // MK_GPT: Flag indicating if a response is in progress
    tlm::tlm_generic_payload* nextResponsePending; // MK_GPT: Pointer to the next response pending
    tlm::tlm_generic_payload* endRequestPending; // MK_GPT: Pointer to the end request pending
    tlm_utils::peq_with_cb_and_phase<Target> peq; // MK_GPT: Payload event queue for handling transaction phases

    public:
    SC_CTOR(Target) : socket("socket"), // MK_GPT: Constructor for the Target module
        transactionInProgress(0),
        responseInProgress(false),
        nextResponsePending(0),
        endRequestPending(0),
        peq(this, &Target::peqCallback) // MK_GPT: Initialize the payload event queue
    {
        socket.bind(*this); // MK_GPT: Bind the target socket to this module

        SC_METHOD(executeTransactionProcess); // MK_GPT: Define a SystemC method
        sensitive << targetDone; // MK_GPT: Sensitivity list for the method
        dont_initialize(); // MK_GPT: Do not initialize the method

    }

    virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) // MK_GPT: Blocking transport method
    {
        executeTransaction(trans); // MK_GPT: Execute the transaction
    }

    // MK_GPT: Non-blocking transport method
    virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& delay)
    {
        peq.notify( trans, phase, delay); // MK_GPT: Queue the transaction into the payload event queue
        return tlm::TLM_ACCEPTED; // MK_GPT: Return status indicating acceptance of the transaction
    }

    // MK_GPT: Callback function for the payload event queue
    void peqCallback(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase)
    {
        sc_time delay;

        if(phase == tlm::BEGIN_REQ) // MK_GPT: If the phase is BEGIN_REQ
        {
            trans.acquire(); // MK_GPT: Increment the transaction reference count

            if (!transactionInProgress) // MK_GPT: If there is no transaction in progress
            {
                sendEndRequest(trans); // MK_GPT: Send end request
            }
            else // MK_GPT: If a transaction is in progress
            {
                endRequestPending = &trans; // MK_GPT: Put back-pressure on initiator by deferring END_REQ
            }
        }
        else if (phase == tlm::END_RESP) // MK_GPT: If the phase is END_RESP
        {
            if (!responseInProgress) // MK_GPT: If response is not in progress
            {
                SC_REPORT_FATAL("TLM-2", "Illegal transaction phase END_RESP received by target");
            }

            transactionInProgress = 0; // MK_GPT: Clear transaction in progress
            responseInProgress = false; // MK_GPT: Clear response in progress

            if (nextResponsePending) // MK_GPT: If there is a next response pending
            {
                sendResponse(*nextResponsePending); // MK_GPT: Send the response
                nextResponsePending = 0; // MK_GPT: Clear next response pending
            }

            if (endRequestPending) // MK_GPT: If there is an end request pending
            {
                sendEndRequest(*endRequestPending); // MK_GPT: Send the end request
                endRequestPending = 0; // MK_GPT: Clear end request pending
            }
        }
        else // MK_GPT: If phase is not BEGIN_REQ or END_RESP
        {
            SC_REPORT_FATAL(name(), "Illegal transaction phase received");
        }
    }

    // MK_GPT: Method to send end request
    void sendEndRequest(tlm::tlm_generic_payload& trans)
    {
        tlm::tlm_phase bw_phase;
        sc_time delay;

        bw_phase = tlm::END_REQ; // MK_GPT: Set phase to END_REQ
        delay = randomDelay(); // MK_GPT: Generate delay

        tlm::tlm_sync_enum status;
        status = socket->nb_transport_bw( trans, bw_phase, delay ); // MK_GPT: Non-blocking transport call
        // MK_GPT: Queue internal event to mark beginning of response
        delay = delay + randomDelay();
        targetDone.notify( delay ); // MK_GPT: Notify target done event

        assert(transactionInProgress == 0);
        transactionInProgress = &trans; // MK_GPT: Set transaction in progress
    }

    // MK_GPT: Method that runs on targetDone event
    void executeTransactionProcess()
    {
        executeTransaction(*transactionInProgress); // MK_GPT: Execute the transaction

        if (responseInProgress) // MK_GPT: If response is in progress
        {
            if (nextResponsePending) // MK_GPT: If next response is pending
            {
                SC_REPORT_FATAL(name(), "Attempt to have two pending responses in target");
            }
            nextResponsePending = transactionInProgress; // MK_GPT: Set next response pending
        }
        else // MK_GPT: If response is not in progress
        {
            sendResponse(*transactionInProgress); // MK_GPT: Send response
        }
    }

    // MK_GPT: Common method to execute a transaction
    void executeTransaction(tlm::tlm_generic_payload& trans)
    {
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    adr = trans.get_address();
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();
        unsigned char*   byt = trans.get_byte_enable_ptr();
        unsigned int     wid = trans.get_streaming_width();

        if (byt != 0) {
            trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
            return;
        }
        if (len > 4 || wid < len) {
            trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
            return;
        }

        if (cmd == tlm::TLM_READ_COMMAND)
        {
            *reinterpret_cast<int*>(ptr) = -int(adr);
        }
        else if (cmd == tlm::TLM_WRITE_COMMAND)
        {
            assert( *reinterpret_cast<unsigned int*>(ptr) == adr );
        }

        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }

    // MK_GPT: Method to send response
    void sendResponse(tlm::tlm_generic_payload& trans)
    {
        tlm::tlm_sync_enum status;
        tlm::tlm_phase bw_phase;
        sc_time delay;

        responseInProgress = true; // MK_GPT: Set response in progress
        bw_phase = tlm::BEGIN_RESP; // MK_GPT: Set phase to BEGIN_RESP
        delay = SC_ZERO_TIME;

        status = socket->nb_transport_bw( trans, bw_phase, delay ); // MK_GPT: Non-blocking transport call

        if (status == tlm::TLM_UPDATED) // MK_GPT: If updated
        {
            // MK_GPT: The timing annotation must be honored
            peq.notify( trans, bw_phase, delay);
        }
        else if (status == tlm::TLM_COMPLETED) // MK_GPT: If completed
        {
            // MK_GPT: The initiator has terminated the transaction
            transactionInProgress = 0;
            responseInProgress = false;
        }
        // MK_GPT: In the case of TLM_ACCEPTED we will receive a FW call

        trans.release();
    }

    // MK_GPT: TLM-2 forward DMI method
    virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data)
    {
        // MK_GPT: Dummy method
        return false;
    }

    // MK_GPT: TLM-2 debug transport method
    virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans)
    {
        // MK_GPT: Dummy method
        return 0;
    }
};

#endif // TARGET_H
