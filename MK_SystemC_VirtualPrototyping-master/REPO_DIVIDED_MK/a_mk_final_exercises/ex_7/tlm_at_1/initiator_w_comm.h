#ifndef INITIATOR_H
#define INITIATOR_H

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

#define LENGTH 100

/*
MK_GPT: Explanation of the code from the exercise description:
The example consists of an AT Initiator and Target, which both follow the Four Phase
Handshake. The initiator will randomly send reads and writes to a random address
and uses this random address also as data (also note how a reinterpret_cast
can be used in TLM). The target will negate the data in the case of a read command.
The intention is to show the concept and there is no deeper meaning behind the
example. The target is only able to handle one incoming transaction and uses the
backpressure mechanism (deferring END_REQ) to stop the initiator from sending new
requests. Later we will see how targets could handle multiple incoming transactions.
Note that in general there are several ways to implement solutions that are standard
compliant, this is just one way to do it.
In order to check if programmed models are standard compliant, the example uses
the TLM2 Base Protocol Checker from John Aynsley:
https://github.com/TUK-SCVP/SCVP.artifacts/tree/master/tlm_protocol_checker
This checker stops the simulation immediately if there is a non-standard-compliant
behavior in the current simulation.
Please study the code of the AT initiator and target by following the four phase handshake
shown in Figure 1.
*/

class Initiator: public sc_module, public tlm::tlm_bw_transport_if<>
{
    public:
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    tlm::tlm_initiator_socket<> socket;

    protected:
    MemoryManager mm; // MK_GPT: Memory manager instance
    int data[16]; // MK_GPT: Data array to store transactions 
    tlm::tlm_generic_payload* requestInProgress; // MK_GPT: Pointer to the current request in progress
    sc_event endRequest; // MK_GPT: Event to signal end of a request
    tlm_utils::peq_with_cb_and_phase<Initiator> peq; // MK_GPT: Payload event queue with callback and phase

    public:
    SC_CTOR(Initiator): socket("socket"), // MK_GPT: Constructor for Initiator module
                        requestInProgress(0),
                        peq(this, &Initiator::peqCallback) // MK_GPT: Initialize payload event queue
    {
        socket.bind(*this); // MK_GPT: Bind socket to this module

        SC_THREAD(process); // MK_GPT: Define process thread

        for(int i=0; i<16; i++)
        {
            data[i] = 0; // MK_GPT: Initialize data array
        }
    }

    protected:
    void process()
    {
        tlm::tlm_generic_payload* trans;
        tlm::tlm_phase phase;
        sc_time delay;

        // Generate a sequence of random transactions
        for (int i = 0; i < LENGTH; i++)
        {
            int adr = rand(); // MK_GPT: Generate random address
            tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2); // MK_GPT: Generate random command
            if (cmd == tlm::TLM_WRITE_COMMAND) data[i % 16] = adr; // MK_GPT: Update data array if command is WRITE

            // Grab a new transaction from the memory manager
            trans = mm.allocate(); // MK_GPT: Allocate new transaction
            trans->acquire(); // MK_GPT: Acquire transaction

            // Set transaction attributes
            trans->set_command(cmd); // MK_GPT: Set command
            trans->set_address(adr); // MK_GPT: Set address
            trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data[i%16])); // MK_GPT: Set data pointer
            trans->set_data_length(4); // MK_GPT: Set data length
            trans->set_streaming_width(4); // MK_GPT: Set streaming width
            trans->set_byte_enable_ptr(0); // MK_GPT: Set byte enable pointer
            trans->set_dmi_allowed(false); // MK_GPT: Set DMI allowed
            trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // MK_GPT: Set response status

            // Initiator must follow the BEGIN_REQ/END_REQ exclusion rule:
            if (requestInProgress)
            {
                wait(endRequest); // MK_GPT: Wait for end of request
            }
            requestInProgress = trans; // MK_GPT: Set current request in progress
            phase = tlm::BEGIN_REQ; // MK_GPT: Set phase to BEGIN_REQ

            // Timing annot. models processing time of initiator prior to call
            delay = randomDelay(); // MK_GPT: Generate random delay

            // Print transaction information
            cout << "\033[1;31m"
                 << "(I) @"  << setfill(' ') << setw(12) << sc_time_stamp()
                 << ": " << setw(12) << (cmd ? "Write to " : "Read from ")
                 << "Addr = 0x" << setfill('0') << setw(8) << hex << adr
                 << " Data = " << "0x" << setfill('0') << setw(8)
                 << hex << data[i%16] << "\033[0m" << endl;

            // Non-blocking transport call on the forward path
            tlm::tlm_sync_enum status;

            // Call [1.0]:
            status = socket->nb_transport_fw( *trans, phase, delay ); // MK_GPT: Non-blocking transport call

            // Check value returned from nb_transport_fw
            if (status == tlm::TLM_UPDATED) // [2.0] or [4.0]
            {
                // The timing annotation must be honored
                peq.notify(*trans, phase, delay); // MK_GPT: Notify payload event queue
            }
            else if (status == tlm::TLM_COMPLETED) // [3.0]
            {
                // The completion of the transaction
                // necessarily ends the BEGIN_REQ phase
                requestInProgress = 0; // MK_GPT: Clear request in progress

                // The target has terminated the transaction
                checkTransaction(*trans); // MK_GPT: Check transaction

                // Allow the memory manager to free the transaction object
                trans->release(); // MK_GPT: Release transaction
            }
            // In the case of TLM_ACCEPTED [1.1] we
            // will recv. a BW call in the future [1.2, 1.4]

            wait(randomDelay()); // MK_GPT: Wait for random delay
        }
    }

    // [1.2, 1.4]
    virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans,
                                               tlm::tlm_phase& phase,
                                               sc_time& delay)
    {
        // Queue the transaction into the peq until
        // the annotated time has elapsed
        peq.notify(trans, phase, delay); // MK_GPT: Notify payload event queue

        // HINT: a Return Path shortcut can be implemented here [2.1]

        return tlm::TLM_ACCEPTED; // [1.3, 1.5]
    }

    // Payload event queue callback
    void peqCallback(tlm::tlm_generic_payload& trans,
                     const tlm::tlm_phase& phase)
    {
        if (phase == tlm::END_REQ // <-- [1.2, 2.0]
            // or [4.0] --V
                || (&trans == requestInProgress && phase == tlm::BEGIN_RESP))
        {
            // The end of the BEGIN_REQ phase
            requestInProgress = 0; // MK_GPT: Clear request in progress
            endRequest.notify(); // MK_GPT: Notify end of request
        }
        else if (phase == tlm::BEGIN_REQ || phase == tlm::END_RESP)
        {
            SC_REPORT_FATAL(name(), "Illegal transaction phase received"); // MK_GPT: Report fatal error for illegal transaction phase
        }

        if (phase == tlm::BEGIN_RESP) // [1.4]
        {
            checkTransaction(trans); // MK_GPT: Check transaction

            // Send final phase transition to target
            tlm::tlm_phase fw_phase = tlm::END_RESP; // MK_GPT: Set forward phase to END_RESP
            sc_time delay = sc_time(randomDelay()); // MK_GPT: Generate random delay
            // [1.6]
            socket->nb_transport_fw( trans, fw_phase, delay ); // MK_GPT: Non-blocking forward transport call, ignore return

            // Allow the memory manager to free the transaction object
            trans.release(); // MK_GPT: Release transaction
        }
    }

    // Called on receiving BEGIN_RESP or TLM_COMPLETED
    void checkTransaction(tlm::tlm_generic_payload& trans)
    {
        if (trans.is_response_error())
        {
            SC_REPORT_ERROR(name(), "Transaction returned with error!"); // MK_GPT: Report error if transaction has error
        }

        tlm::tlm_command cmd = trans.get_command(); // MK_GPT: Get command
        sc_dt::uint64    adr = trans.get_address(); // MK_GPT: Get address
        int*             ptr = reinterpret_cast<int*>(trans.get_data_ptr()); // MK_GPT: Get data pointer

        // Print transaction information
        cout << "\033[1;31m"
             << "(I) @"  << setfill(' ') << setw(12) << sc_time_stamp()
             << ": " << setw(12) << (cmd ? "Check Write " : "Check Read ")
             << "Addr = 0x" << setfill('0') << setw(8) << hex << adr
             << " Data = " << "0x" << setfill('0') << setw(8) << hex << *ptr
             << "\033[0m" << endl;

        if (cmd == tlm::TLM_READ_COMMAND) // Check if Target did the right thing
        {
            assert(*ptr == -int(adr)); // MK_GPT: Assert if data read is not as expected
        }
    }

    // TLM-2 backward DMI method
    virtual void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                           sc_dt::uint64 end_range)
    {
        // Dummy method
    }

};


#endif // INITIATOR_H
