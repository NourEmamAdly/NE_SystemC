#include <systemc.h>
/****************************************************************************************************************************************
 *        Implementing Top Level Module that contains more than one tranistion module and each transition module consists of            *
 *               array of inputs and array of outputs connected to each other using the place channel we drived.                        *
 *                                                       Check Fig 2 Simple Petri Net                                                   *
 ****************************************************************************************************************************************/
#include "place.h"
#include "transition.h"
#include "subnet.h"

// Toplevel:
SC_MODULE(toplevel)
{
    
private:
//Inestantiating transition module with 1 input and 2 outputs
transition<1,2> t1;
//Inestantiating transition module with 2 inputs and 1 output
transition<2,1> t2;
//Inestantiating transition module with 1 input and 1 output  
transition<1,1> t3;
// Inistentiating 4 channels to connect the 4 modules as shown in the Fig 2 Simple Petri Net   
place p1,p2,p3,p4;

public:
SC_CTOR(toplevel): t1("t1"),t2("t2"),t3("t3"),p1(3),p2(0),p3(0),p4(0)
{
    //check figure 2 Simple Petri Net for corresponding  bindings
    t1.in.bind(p1);
    t1.out.bind(p3);
    t1.out.bind(p2);

    t2.in.bind(p2);
    t2.in.bind(p4);
    t2.out.bind(p1);

    t3.in.bind(p3);
    t3.out.bind(p4);

   SC_THREAD(process);
}



void process()
{

        while(true)
        {
            wait(10,SC_NS);
            t1.fire();
            cout << "p1: "<< p1.testTokens() <<" p2: " << " p3: "<< p3.testTokens() <<" P4: " << p4.testTokens() << endl;
            wait(10,SC_NS);
            t2.fire();
            cout << "p1: "<< p1.testTokens() <<" p2: " << " p3: "<< p3.testTokens() <<" P4: " << p4.testTokens() << endl;
            wait(10,SC_NS);
            t3.fire();
            cout << "p1: "<< p1.testTokens() <<" p2: " << " p3: "<< p3.testTokens() <<" P4: " << p4.testTokens() << endl;
             wait(10,SC_NS);
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
