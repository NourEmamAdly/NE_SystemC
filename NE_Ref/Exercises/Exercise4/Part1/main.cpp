#include <systemc.h>

#include "place.h"
#include "transition.h"
#include "subnet.h"
/****************************************************************************************************************************************
 * Implementing Top Level Module that contains more than one transition module connected to each other using the place channel we drived*
 *                                            Check the figure 1 Simple Petri Net                                                       *
 ****************************************************************************************************************************************/
// Toplevel:
SC_MODULE(toplevel)
{
//Inistentiting two transition objects called t1 and t2  and place channels p1 and p2 to connect between the two objects
private:
transition t1;
transition t2;
place p1,p2;

public:
SC_CTOR(toplevel): t1("t1"),t2("t2"),p1(1),p2(0)
{
// bind our transition modules to the place channels as shown in the fig 1 (Simple Petri Net)
    t1.in.bind(p1);
    t1.out.bind(p2);
    t2.in.bind(p2);
    t2.out.bind(p1);
  SC_THREAD(process);
}


// Implementing a process to test our transistion modules that is connected to the custom channels that we designed 
void process()
{

        while(true)
        {
            wait(10,SC_NS);
            t1.fire();
            wait(10,SC_NS);
            t1.fire();
            wait(10,SC_NS);
            t2.fire();
            sc_stop();
        }

}


};


// Instintiating a top level module to test our compination

int sc_main(int, char**)
{
    // TODO
toplevel t("t");
 
    sc_start();
    return 0;
}
