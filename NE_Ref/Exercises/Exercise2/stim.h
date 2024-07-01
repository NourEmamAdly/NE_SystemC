#ifndef STIMULUS_H
#define STIMULUS_H

#include <systemc.h>
/****************************************************************************************************************************************
 *                     Implementing Stim Module for stimulating values to the Inputs of the exor module to test it                      *
 ****************************************************************************************************************************************/
SC_MODULE(stim)
{
public:
    //those will be considered as Input values for the SC_IN A and SC_IN B of the exor module to test it
    sc_out<bool> A, B;

    SC_CTOR(stim)
    {
        // defining a thread that will generate values on the I/P ports A & B of the exor module to test it
        SC_THREAD(StimGen);
    }

private:
    // Implementing the stimuli generation function
    // we used threads here as we cant use the wait statement with the System C methods
    void StimGen()
    {
        //here we write values to port A and B for testing
        wait(SC_ZERO_TIME);
        A.write(false);
        B.write(false);
        wait(10, SC_NS);
        A.write(false);
        B.write(true);
        wait(15, SC_NS);
        A.write(true);
        B.write(false);
        wait(10, SC_NS);
        A.write(true);
        B.write(true);
        wait(10, SC_NS);
        A.write(false);
        B.write(false);
        wait(10, SC_NS);
        sc_stop();
    }
};

#endif
