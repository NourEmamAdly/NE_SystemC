#include <systemc.h>

#include "stim.h"
#include "exor.h"
#include "mon.h"

// For GTKWave Simulator
#include <sysc/datatypes/fx/sc_fixed.h>
#include <sysc/datatypes/fx/sc_ufixed.h>
#include <sysc/utils/sc_report.h>

sc_trace_file *tf = sc_create_vcd_trace_file("waves");
// Until here

int sc_main(int, char**)
{
    sc_signal<bool> sigA, sigB, sigZ;

    sc_clock clock("Clk", 10, SC_NS, 0.5);

    stim Stim1("Stimulus");
    Stim1.A(sigA);
    Stim1.B(sigB);
    Stim1.Clk.bind(clock);

    exor DUT("exor");
    DUT.A(sigA);
    DUT.B(sigB);
    DUT.Z(sigZ);

    Monitor mon("Monitor");
    mon.A(sigA);
    mon.B(sigB);
    mon.Z(sigZ);
    mon.Clk.bind(clock);

    //sc_start();  // run forever

    // For GTKWave Simulator:
    sc_trace(tf, clock, "Clk");
    sc_trace(tf, sigA, "signalA");
    sc_trace(tf, sigB, "signalB");
    sc_trace(tf, sigZ, "signalZ");
    
    sc_start(50, SC_NS); // run for 50ns

    // Fot GTKWave Simulator:
    sc_close_vcd_trace_file(tf);    //At the end of your main function, close the trace file
    
    return 0;
}
