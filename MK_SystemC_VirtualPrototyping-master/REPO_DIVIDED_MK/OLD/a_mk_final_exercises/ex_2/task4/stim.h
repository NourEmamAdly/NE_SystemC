#ifndef STIMULUS_H
#define STIMULUS_H

#include <systemc.h>
// Code to simulate some inputs after each other
SC_MODULE(stim)
{
public:
    sc_out<bool> A, B;
    sc_in<bool> Clk;        // MK_ADDED

    SC_CTOR(stim)
    {
        SC_THREAD(StimGen);
        sensitive << Clk.pos();     // MK_ADDED
    }
// IMPPP: SC_THREAD is used and not SC_METHOD because:
// 1. methods are executed atomically, need to execute the entire method all at once, 
//    will not work here as we are changing signals and waiting to see the change and 
//    then repeat. They complete entire method code then return control to scheduler.
// 2. Threads can be suspended by waiting statements and therefore other modules can
//    execute and are not blocked. Here there is no infinite loop as we want to execute
//    this sequence only once and then the thread dies.
private:
    void StimGen()
    {
        //wait(SC_ZERO_TIME);       // MK_CHANGED
        A.write(false);
        B.write(false);
        //wait(10, SC_NS);          // MK_CHANGED
        wait();                     // MK_ADDED
        A.write(false);
        B.write(true);
        //wait(15, SC_NS);          // MK_CHANGED
        wait();                     // MK_ADDED
        A.write(true);
        B.write(false);
        //wait(10, SC_NS);          // MK_CHANGED
        wait();                     // MK_ADDED
        A.write(true);
        B.write(true);
        //wait(10, SC_NS);          // MK_CHANGED
        wait();                     // MK_ADDED
        A.write(false);
        B.write(false);
        //wait(10, SC_NS);          // MK_CHANGED
        wait();                     // MK_ADDED
        //sc_stop();                // MK_CHANGED
    }
};

#endif
