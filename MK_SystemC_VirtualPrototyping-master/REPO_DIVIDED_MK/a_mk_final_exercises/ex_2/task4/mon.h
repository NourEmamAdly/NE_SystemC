#ifndef MONITOR_H
#define MONITOR_H

#include <iostream>
#include <systemc.h>

// Sensitive to any change in A, B, Z and prints it
SC_MODULE(Monitor)
{
public:
    sc_in<bool> A, B, Z;
    sc_in<bool> Clk;        // MK_ADDED

    SC_CTOR(Monitor)
    {
        std::cout << std::endl <<  "time\tA\tB\tF" << std::endl;
        SC_METHOD(monitor);
        //sensitive << A << B << Z;       // MK_CHANGED
        sensitive << Clk.pos();         // MK_ADDED
        dont_initialize();
    }

private:
    void monitor()
    {
        std::cout << sc_time_stamp()  << "\t" << A << "\t" << B << "\t" << Z << std::endl;
    }
};

#endif
