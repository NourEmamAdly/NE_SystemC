/*
 * Copyright 2017 Matthias Jung
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors:
 *     - Matthias Jung
 */

#include <iomanip>
#include <systemc>
#include <tlm.h>
#include <tlm_utils/peq_with_cb_and_phase.h>
#include "../tlm_memory_manager/memory_manager.h"
#include "../tlm_protocol_checker/tlm2_base_protocol_checker.h"
#include "../tlm_at_1/target.h"
#include "../tlm_at_1/initiator.h"
#include "../tlm_at_1/util.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*
MK: 
3. Early Completion
The code for example 3 is available here:
https://github.com/TUK-SCVP/SCVP.artifacts/tree/master/tlm_at_3
In this example the target ends the transaction by an early completion, which is similar
to the b_transport of LT coding style. Please study the code by following the Early
Completion path in Figure 1.
*/

class EarlyTarget: public Target
{
    public:
    SC_HAS_PROCESS(EarlyTarget);
    EarlyTarget(sc_module_name name) : Target(name)
    {
    }

    // [1.0, 1.6]
    tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans,
                                               tlm::tlm_phase& phase,
                                               sc_time& delay)
    {
        // Lets do early completion here (No PEQ!):
        cout << "\033[1;35m"
           << "(T) @"  << setfill(' ') << setw(12) << sc_time_stamp()
           << ": " << "Early Completion \033[0m" << endl;
        Target::executeTransaction(trans);
        delay += randomDelay();
        return tlm::TLM_COMPLETED; // [3.0]
    }
};

int sc_main (int __attribute__((unused)) sc_argc,
             char __attribute__((unused)) *sc_argv[])
{
    cout << std::endl;

    Initiator* initiator = new Initiator("initiator");
    EarlyTarget* target = new EarlyTarget("target");

    tlm_utils::tlm2_base_protocol_checker<> *chk =
        new tlm_utils::tlm2_base_protocol_checker<>("chk");

    // Binding:
    initiator->socket.bind(chk->target_socket);
    chk->initiator_socket.bind(target->socket);

    sc_start();
    return 0;
}
