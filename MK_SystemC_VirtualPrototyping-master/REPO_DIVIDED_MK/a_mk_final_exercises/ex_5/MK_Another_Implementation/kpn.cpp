#include <unistd.h>
#include <systemc.h>

#include "kpn.h"

// ADD THINGS HERE
SC_HAS_PROCESS(kpn);
kpn::kpn( ::sc_core::sc_module_name ): a(10), b(10), c(10), d(10), e("e")
{
    b.write(1);
    c.write(0);
    SC_THREAD(Delay);
    SC_THREAD(Split);
    SC_THREAD(Add);
}

void kpn::Add(){
    while(true){
        b.write(a.read()+c.read());
    }
}

void kpn::Split() {
    int i = 0;
    while (i < 10) {
        // wait(10, SC_NS);
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

void kpn::Delay() {
    while(true){
        c.write(d.read());
    }
}