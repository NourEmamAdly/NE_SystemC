#include <systemc.h>

#include "place.h"
#include "transition.h"
#include "subnet.h"

// Toplevel:
// TODO
SC_MODULE(toplevel)
{

    public:
        SC_CTOR(toplevel): t1("t1"), t2("t2"), p1(1), p2(0)
        {
            t1.in.bind(p1);
            t1.out.bind(p2);
            t2.in.bind(p2);
            t2.out.bind(p1);
            SC_THREAD(process);
            // sensitive << A << B << Z;
        }

    private:
        transition t1;
        transition t2;
        place p1, p2;

/*
        sc_port <placeInterface> in_t1;
        sc_port <placeInterface> out_t1;
        sc_port <placeInterface> in_t2;
        sc_port <placeInterface> out_t2;
*/
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
