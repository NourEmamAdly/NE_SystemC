#include <unistd.h>
#include <systemc.h>

#include "kpn.h"

// ADD THINGS HERE

void kpn::kpn_add(){
    while(true)
    {
        b.write(a.read() + c.read());
    }
}


void kpn::kpn_split(){
    int i = 0;
    while (i < 10) {
        unsigned int bt;
        bt = b.read();
        a.write(bt);
        d.write(bt);
        e.write(bt);
        cout << "@" << sc_delta_count() << "δ: e = " << bt << endl;
        i++;
    }
    sc_stop();
}


void kpn::kpn_delay(){
    while(true)
    {
        c.write(d.read());
    } 
}
