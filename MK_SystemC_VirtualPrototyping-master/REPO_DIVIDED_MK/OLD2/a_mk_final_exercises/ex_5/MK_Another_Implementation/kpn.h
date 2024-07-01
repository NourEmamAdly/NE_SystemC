#ifndef KPN_H
#define KPN_H

#include <systemc.h>
#include <iostream>

SC_MODULE(kpn)
{
private:
    sc_fifo<unsigned int> a;
    sc_fifo<unsigned int> b;
    sc_fifo<unsigned int> c;
    sc_fifo<unsigned int> d;
    sc_signal<unsigned int> e;

public:
    /*SC_CTOR(kpn) // : ADD THINGS HERE
    {
        // ADD THINGS HERE
    }*/
    kpn( ::sc_core::sc_module_name );
    void Add();
    void Split();
    void Delay();
};

#endif // KPN_H
