#include <systemc.h>

#include "stim.h"
#include "exor.h"
#include "mon.h"
/****************************************************************************************************************************************
 *                             Implementing Main Function using Stim & Monitor for testing the exor                                     *
 ****************************************************************************************************************************************/
/****************************************************************************************************************************************
 *                              NOTE : Also here we used the Waveform to show the change of the signals                                 *
 ****************************************************************************************************************************************/
int sc_main(int, char**)
{
//code for Setup Waveform tracing
sc_trace_file *tf =sc_create_vcd_trace_file("Signals Trace");

// defining 3 signals 
    sc_signal<bool> sigA, sigB, sigZ;

//Inistintiating stimulus from stim class and connect its output ports to the I/P and O/P signals we created
    stim Stim1("Stimulus");
    Stim1.A(sigA);
    Stim1.B(sigB);
// binding the Input and output signals to the exor Inputs and ouptut ports
    exor DUT("exor");
    DUT.A(sigA);
    DUT.B(sigB);
    DUT.Z(sigZ);
// binding the monitor to the Input and output signals to monitor any change that happens 
    Monitor mon("Monitor");
    mon.A(sigA);
    mon.B(sigB);
    mon.Z(sigZ);

//defining what you want to show on the waves
    sc_trace(tf,sigA,"In_Signal A");
    sc_trace(tf,sigA,"In_Signal B");
    sc_trace(tf,sigA,"Out_Signal Z");

    sc_start(50,SC_NS);  // run forever

// For GTKWave Simulator
 sc_close_vcd_trace_file(tf);

    return 0;
}
