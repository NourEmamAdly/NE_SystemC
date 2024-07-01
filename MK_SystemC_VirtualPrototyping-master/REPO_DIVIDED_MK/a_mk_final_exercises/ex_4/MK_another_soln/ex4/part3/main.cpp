#include <systemc.h>

#include "place.h"
#include "transition.h"
#include "subnet.h"

// Toplevel:
// TODO
SC_MODULE(toplevel)
{

    public:
        SC_CTOR(toplevel): t1("t1"), t2("t2"), t3("t3"), p1(1), p2(0), p3(0), p4(0)
        {
            t1.in.bind(p1);
            t1.out.bind(p2);
            t1.out.bind(p3);
            
            t2.in.bind(p2);
            t2.in.bind(p4);
            t2.out.bind(p1);
            
            t3.in.bind(p3);
            t3.out.bind(p4);
            SC_THREAD(process);
            // sensitive << A << B << Z;
        }

    private:
        transition<1,2> t1;
        transition<2,1> t2;
        transition<1,1> t3;
        place<1,1> p1;
        place<1,1> p2;
        place<1,1> p3;
        place<1,1> p4;

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
                t2.fire();
                wait(10, SC_NS);
                t3.fire();
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
