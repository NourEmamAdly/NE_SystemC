#include <unistd.h>
//#include <systemc.h>

#include "basic_module.h"

// ADD THINGS HERE

void basicModule::func1(){
    while(true)                                     // Since it is a thread
    {
        b.write(a.read() + c.read());
    }
}


void basicModule::func2(){
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


void basicModule::func3(){
    while(true)
    {
        c.write(d.read());
    } 
}

void basicModule::func4(){
    while(true)
    {
        c.write(d.read());
    } 
}

int sc_main(int, char**)
{
    basicModule bm("basic_module");
    sc_start();
    return 0;
}