#ifndef MONITOR_H
#define MONITOR_H

#include <iostream>
#include <systemc.h>
/****************************************************************************************************************************************
 *                  Implementing Monitor Module that prints any change that happens for the I/P and O/P ports of the exor module        *
 ****************************************************************************************************************************************/
//Senstive to any change in A,B,Z and it prints it.
SC_MODULE(Monitor)
{
public:
    // defining 3 input signals that enter to the enter module to show the change that happens within the I/P and O/P of the Exor module
    sc_in<bool> A, B, Z;

    SC_CTOR(Monitor)
    {
        // here we print the initial values of the I/P and O/P of the exor module
        std::cout << std::endl <<  "time\tA\tB\tF" << std::endl;
        //defining sc method called monitor that is sensitive to any change and we dont run it in the beggining of simulation.
        SC_METHOD(monitor);
        sensitive << A << B << Z;
        dont_initialize();
    }

private:
    void monitor()
    {
        //everytime a change happen with the I/P and O/P Signals it will be printed
        std::cout << sc_time_stamp()  << "\t" << A << "\t" << B << "\t" << Z << std::endl;
    }
};

#endif
