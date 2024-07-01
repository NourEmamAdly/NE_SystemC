/**********************************************************************************************************************************
*                                    Implementing standard  TLM Initiator and Target Concept                                      *
*                                      Code for Blocking Loosely Timed Initator and Target                                        *
***********************************************************************************************************************************/

#include <iostream>
#include <systemc.h>
#include <tlm.h>                                                                        // MK: DF we need to add this now

using namespace std;
/*********************************************************************************************************************************************************
 *                                                         Implementing Initiator                                                                        *
 *********************************************************************************************************************************************************/
//Deriving Initiator from SC Module and the TLM backward transport Interface(as the initiator implement the functions of the backward interface)
// NOTE : Initiator implement the bw transport interface virtual functions that we may use by the target
class exampleInitiator: sc_module, tlm::tlm_bw_transport_if<>                           // MK: Inherit from these. Initiator takes from backward (bw_)
{
    public:
//Defining the Initiator Port which called socket in TLM
    tlm::tlm_initiator_socket<> iSocket;                                                 // MK: Create a port, but in tlm it is an initiator socket
// Defining the Instructor and initializing the socket             
    SC_CTOR(exampleInitiator) : iSocket("iSocket")
    {
    //connect the port of the initiator to the interface that we use with here
        iSocket.bind(*this);                                                            // MK: Connection between socket and the interface, we aren't using templated, but rather bind. Will see another simple way later.
        SC_THREAD(process);
    }

//Implementing the Process
    private:
    void process()
    {
        /***********************************************************************************************************************
         *                                      Write from Initiator to memory Target:                                         *
         ***********************************************************************************************************************/
        for (int i = 0; i < 4; i++) {
            tlm::tlm_generic_payload trans; // our transaction object that we will send to the target
            unsigned char data = rand(); // we assign one byte of random numbers to the data field in the 
            trans.set_address(i); // set the address that we will send to 
            trans.set_data_length(1);    // set the length of the data we send             // MK: send 1 byte
            trans.set_streaming_width(1); 
            trans.set_command(tlm::TLM_WRITE_COMMAND);// we set the command to write
            trans.set_data_ptr(&data); // we set a pointer to this object
            trans.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );
       
            // we define a variable called delay with data type time to make sure that we wait this time
            //we send out the data and the target say i will process this in some seconds thats the value we wait for here in the initiator
            sc_time delay = sc_time(0, SC_NS);                                              // MK: must add this not the wait statement
        
            // we transport the data (struct)
            iSocket->b_transport(trans, delay);                                             // MK: Now we need to transport this.
           
            // Checking response error
            if ( trans.is_response_error() )
              SC_REPORT_FATAL(name(), "Response error from b_transport");
            
       // this is the delay that we will wait until the target start recieving
            wait(delay);               
       // prininting out time stamp with relating to data
            std::cout << "@" << sc_time_stamp() << "\tWrite Data: "
                      << (unsigned int)data << std::endl;
        }

        /***********************************************************************************************************************
         *                                      Read from Memory (Target) in the initiator Target:                             *
         ***********************************************************************************************************************/
        for (int i = 0; i < 4; i++) {
            tlm::tlm_generic_payload trans;
            unsigned char data;
            trans.set_address(i);
            trans.set_data_length(1);
            trans.set_streaming_width(1);
            trans.set_command(tlm::TLM_READ_COMMAND);
            trans.set_data_ptr(&data);
            trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
            sc_time delay = sc_time(0, SC_NS);

            iSocket->b_transport(trans, delay);

            if ( trans.is_response_error() )
              SC_REPORT_FATAL(name(), "Response error from b_transport");

            wait(delay);

            std::cout << "@" << sc_time_stamp() << "\tRead Data: "
                      << (unsigned int)data << std::endl;
        }
    }

/*=====================================================================================================================================================================*/
    // Dummy method aka Stub:
    void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,                               // MK: Need to implement the PVFs (In backward it is two)
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
};
/*=====================================================================================================================================================================*/




/*********************************************************************************************************************************************************
 *                                                         Implementing Target                                                                        *
 *********************************************************************************************************************************************************/

// Deriving target from sc module and forward transport interface ,,,, 
// NOTE : Target implement the fw transport interface virtual functions which is here the b transport that we use in data transmission
class exampleTarget : sc_module, tlm::tlm_fw_transport_if<>
{
    private:
// we create memory space
    unsigned char mem[1024];

    public:
// we create target port called tsocket
    tlm::tlm_target_socket<> tSocket;
// defining the constructor and link the port to the fw interface
    SC_CTOR(exampleTarget) : tSocket("tSocket")
    {
        tSocket.bind(*this);
    }

/***********************************************************************************************************************
 * Implementing the B Transport function that handle the recieving of the payload that is sent by the initiator        *
 ***********************************************************************************************************************/
    void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay)
    {
        if (trans.get_address() >= 1024)
        {
             trans.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
             return;
        }

        if (trans.get_data_length() != 1)
        {
             trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
             return;
        }
// check the recieving command if its write or read
        if(trans.get_command() == tlm::TLM_WRITE_COMMAND) // writing in target memory
        {
            memcpy(&mem[trans.get_address()], // destination                                // In exercise did not work so changed to "mem + trans.get_address()"
                   trans.get_data_ptr(),      // source
                   trans.get_data_length());  // size
        }
        else // (trans.get_command() == tlm::TLM_READ_COMMAND) reading from target memory
        {
            memcpy(trans.get_data_ptr(),      // destination
                   &mem[trans.get_address()], // source
                   trans.get_data_length());  // size
        }
// this is the time that will be returned to initiator to wait for this time until target starts
        delay = delay + sc_time(40, SC_NS);

        trans.set_response_status( tlm::TLM_OK_RESPONSE );
    }
/*=====================================================================================================================================================================*/
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

    // Dummy method
    unsigned int transport_dbg(tlm::tlm_generic_payload& trans)
    {
        SC_REPORT_FATAL(this->name(),"transport_dbg is not implemented");
        return 0;
    }
/*=====================================================================================================================================================================*/
};

int sc_main (int __attribute__((unused)) sc_argc,
             char __attribute__((unused)) *sc_argv[])
{
// Instentiating an initiator and a target 
    exampleInitiator * cpu = new exampleInitiator("cpu");
    exampleTarget * memory = new exampleTarget("memory");

// bind the connection between Initiator and Target
    cpu->iSocket.bind(memory->tSocket);

    sc_start();

    return 0;
}
