#ifndef MONITOR_H
#define MONITOR_H

#include <iostream>
#include <systemc.h>

SC_MODULE(Monitor)
{
public:
    sc_in<bool> A, B, Z;
    sc_in_clk Clk;

    SC_CTOR(Monitor)
    {
        SC_THREAD(monitor_logic);
        sensitive << Clk.pos();
    }

private:
    void monitor_logic() {
        while (true) {
            cout << "@" << sc_time_stamp() << " - Input A: " << A.read()
                 << ", Input B: " << B.read()
                 << ", Output Z: " << Z.read() << endl;

            wait(); // Wait for the next clock edge
        }
    }
};

#endif
