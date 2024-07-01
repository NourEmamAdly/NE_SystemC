#include <systemc.h>

#include "place.h"
#include "transition.h"
#include "subnet.h"

// Toplevel:
// TODO
SC_MODULE(toplevel){
    
    placeChannel p1;
    placeChannel p2;
    transition t1;
    transition t2;

    
    SC_CTOR(toplevel) : t1("t1"), t2("t2"), p1(1), p2(0)  //DF to define the number of tokens
    {
        t1.in.bind(p1);                 // DF the bindings
        t1.out.bind(p2);
        t2.in.bind(p2);
        t2.out.bind(p1);
        SC_THREAD(process);
        //sensitive << t1 << t2 << p1 << p2;
    }

    void process()
    {
        while (true)
        {
            wait(10, SC_NS);
            t1.fire();
            wait(10, SC_NS);
            t1.fire();
            wait(10, SC_NS);
            t2.fire();
            sc_stop();
        }
    }
};

int sc_main(int, char**)
{
    // TODO
    toplevel t("t");

    sc_start();
    return 0;
}
