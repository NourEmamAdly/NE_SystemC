#include <systemc.h>

#include "stim.h"
#include "exor.h"
#include "mon.h"

int sc_main(int, char**)
{
//code for Setup Waveform tracing
sc_trace_file *tf =sc_create_vcd_trace_file("Signals Trace");


    sc_signal<bool> sigA, sigB, sigZ;

    stim Stim1("Stimulus");
    Stim1.A(sigA);
    Stim1.B(sigB);

    exor DUT("exor");
    DUT.A(sigA);
    DUT.B(sigB);
    DUT.Z(sigZ);

    Monitor mon("Monitor");
    mon.A(sigA);
    mon.B(sigB);
    mon.Z(sigZ);

    

//defining what you want to show on the waves
    sc_trace(tf,sigA,"In_Signal A");
    sc_trace(tf,sigA,"In_Signal B");
    sc_trace(tf,sigA,"Out_Signal Z");

    sc_start(50,SC_NS);  // run forever

// Fir GTKWave Simulator
 sc_close_vcd_trace_file(tf);

    return 0;
}
