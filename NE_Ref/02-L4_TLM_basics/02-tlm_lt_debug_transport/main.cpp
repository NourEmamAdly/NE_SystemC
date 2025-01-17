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

#include <iostream>
#include <iomanip>
#include <systemc.h>
#include <tlm.h>

using namespace std;

// Search for keyword "dbg" in file

// From Slides:
/*
◼ Gives an initiator debug access to memory in a target
◼ Similar to b_transport
    ◼ Different: delay free, no waits, no event notifications
    ◼ Uses generic payload
    ◼ Same routing as b_transport
◼ Used for initialization, e.g. for bootloading
*/
/************************************************************************************************************************************************
 *                                                          Defining Initiator                                                                  *
 ************************************************************************************************************************************************/
class exampleInitiator: sc_module, tlm::tlm_bw_transport_if<>
{
    public:
    // We create Initiator Socket using Tlm and we call our socket isocket
    tlm::tlm_initiator_socket<> iSocket;
    SC_CTOR(exampleInitiator) : iSocket("iSocket")
    {
        //this is the pointer that points to the object of this class itself ,now we bind our socket to the object of this class itself
        iSocket.bind(*this);
        SC_THREAD(process);
    }

/************************************************************************************************************************************************
 *                                                  Target Process for mEMORY REad AND WRITE                                                    *
 ************************************************************************************************************************************************/
    private:
    void process()
    {
        // Write to memory:
        for (int i = 0; i < 4; i++) {
            //our transaction object called tlm generic payload and we call this trans,this the packet we send
            tlm::tlm_generic_payload trans;
            //we set data as random data,this basically 1 byte of random number
            unsigned char data = rand();
            //we set address and data length,width
            trans.set_address(i);
            trans.set_data_length(1);
            trans.set_streaming_width(1);
            //we define the command
            trans.set_command(tlm::TLM_WRITE_COMMAND);
            //nw we set our pointer to the data variable
            trans.set_data_ptr(&data);
           
            trans.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );
            //here we define sc time variable called delay  ,this the wait until target response to the initiator
            sc_time delay = sc_time(0, SC_NS);
           //then we transport this packet
            iSocket->b_transport(trans, delay);

            if ( trans.is_response_error() )
              SC_REPORT_FATAL(name(), "Response error from b_transport");

            wait(delay);

            std::cout << "@" << sc_time_stamp() << "\tWrite Data: "
                      << (unsigned int)data << std::endl;
        }

        // Read from memory:
        for (int i = 0; i < 4; i++) {
            tlm::tlm_generic_payload trans;
            unsigned char data;
            trans.set_address(i);
            trans.set_data_length(1);
            trans.set_streaming_width(1);
            trans.set_command(tlm::TLM_READ_COMMAND);
            trans.set_data_ptr(&data);
            trans.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );
            sc_time delay = sc_time(0, SC_NS);

            iSocket->b_transport(trans, delay);

            if ( trans.is_response_error() )
              SC_REPORT_FATAL(name(), "Response error from b_transport");

            wait(delay);

            std::cout << "@" << sc_time_stamp() << "\tRead Data: "
                      << (unsigned int)data << std::endl;
        }
        // End of simulation:
        dumpMemory();
    }

/************************************************************************************************************************************************
 *                                                      Initiator Dummy MEthods                                                                 *
 ************************************************************************************************************************************************/

    // Dummy method:
    void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                   sc_dt::uint64 end_range)
    {
        SC_REPORT_FATAL(this->name(),"invalidate_direct_mem_ptr not implement");
    }

    // Dummy method:
    tlm::tlm_sync_enum nb_transport_bw(
            tlm::tlm_generic_payload& trans,
            tlm::tlm_phase& phase,
            sc_time& delay)
    {
        SC_REPORT_FATAL(this->name(),"nb_transport_bw is not implemented");
        return tlm::TLM_ACCEPTED;
    }




    // I think that is new 
    void dumpMemory()
    {
        std::cout << std::endl;
        std::cout << "Dump Memory Content of Target:" << std::endl;
        std::cout << std::endl;
        unsigned char buffer[64];

        tlm::tlm_generic_payload trans;
        trans.set_address(0);
        trans.set_read();
        trans.set_data_length(64);
        trans.set_data_ptr(buffer);

        unsigned int n = iSocket->transport_dbg(trans);

        for(unsigned int i = 0; i < n; i++)
        {
            std::cout << std::hex
                      << std::setfill('0')
                      << std::setw(2)
                      << (unsigned int)buffer[i];

            if((i+1)%8 == 0)
            {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }
};
/************************************************************************************************************************************************
 *                                                             Defining Target                                                                  *
 ************************************************************************************************************************************************/
class exampleTarget : sc_module, tlm::tlm_fw_transport_if<>
{
    private:
    unsigned char mem[1024];

    public:
    tlm::tlm_target_socket<> tSocket;

    SC_CTOR(exampleTarget) : tSocket("tSocket")
    {
        tSocket.bind(*this);
    }

    void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay)
    {
        if (trans.get_address() >= 1024) //ADDRESS AN ERROR OF OUT OF RANGE
        {
             trans.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
             return;
        }

        if (trans.get_data_length() != 1)
        {
             trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
             return;
        }

        if(trans.get_command() == tlm::TLM_WRITE_COMMAND)
        {
            memcpy(&mem[trans.get_address()], // destination
                   trans.get_data_ptr(),      // source
                   trans.get_data_length());  // size
        }
        else // (trans.get_command() == tlm::TLM_READ_COMMAND)
        {
            memcpy(trans.get_data_ptr(),      // destination
                   &mem[trans.get_address()], // source
                   trans.get_data_length());  // size
        }

        delay = delay + sc_time(40, SC_NS);

        trans.set_response_status( tlm::TLM_OK_RESPONSE );
    }

    // Dummy method
    virtual tlm::tlm_sync_enum nb_transport_fw(
            tlm::tlm_generic_payload& trans,
            tlm::tlm_phase& phase,
            sc_time& delay )
    {
        SC_REPORT_FATAL(this->name(),"nb_transport_fw is not implemented");
        return tlm::TLM_ACCEPTED;
    }

    // Dummy method
    bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans,
                            tlm::tlm_dmi& dmi_data)
    {
        SC_REPORT_FATAL(this->name(),"get_direct_mem_ptr is not implemented");
        return false;
    }

    unsigned int transport_dbg(tlm::tlm_generic_payload& trans)
    {
        if (trans.get_address() >= 1024)
        {
             return 0;
        }

        if(trans.get_command() == tlm::TLM_WRITE_COMMAND)
        {
            memcpy(&mem[trans.get_address()], // destination
                   trans.get_data_ptr(),      // source
                   trans.get_data_length());  // size
        }
        else // (trans.get_command() == tlm::TLM_READ_COMMAND)
        {
            memcpy(trans.get_data_ptr(),      // destination
                   &mem[trans.get_address()], // source
                   trans.get_data_length());  // size
        }

        return trans.get_data_length();

    }

};

int sc_main (int __attribute__((unused)) sc_argc,
             char __attribute__((unused)) *sc_argv[])
{

    exampleInitiator * cpu = new exampleInitiator("cpu");
    exampleTarget * memory = new exampleTarget("memory");

    cpu->iSocket.bind(memory->tSocket);

    sc_start();

    return 0;
}
