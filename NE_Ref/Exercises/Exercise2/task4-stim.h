#ifndef STIMULUS_H
#define STIMULUS_H

#include <systemc.h>
/****************************************************************************************************************************************
 *                           Implementing the Stim Module which is sensitive to clck change on positive edge                            *
 ****************************************************************************************************************************************/
SC_MODULE(stim)
{
public:
    sc_out<bool> A, B;
//we define an input clck to the stim module that will make the module generate outputs to the exor inputs every pos clck edge
    sc_in<bool> clk;
    SC_CTOR(stim)
    {
// here the stimGen Function is sensitive to the positive clck edge of the clck we have created as input for the stim module
        SC_THREAD(StimGen);
        sensitive<<clk.pos();
    }

private:
// this function will be called every time the clck value is positive edge
    void StimGen()
    {   
        wait();
        A.write(false);
        B.write(false);
        wait();
        A.write(false);
        B.write(true);
        wait();
        A.write(true);
        B.write(false);
        wait();
        A.write(true);
        B.write(true);
        wait();
        A.write(false);
        B.write(false);
        wait();
        sc_stop();
    }
};

#endif
