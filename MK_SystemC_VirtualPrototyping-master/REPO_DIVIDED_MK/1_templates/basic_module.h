#ifndef BASIC_MODULE_H
#define BASIC_MODULE_H

#include <systemc.h>

SC_MODULE(basicModule)
// Above line is a macro to below line. Use line below if want to derive from several classes:
//class basicModule : public sc_module
{
private:
    // ADD THINGS HERE
    sc_in<int> in1;
    sc_in<int> in2;
    sc_out<int> in3;
    sc_fifo<unsigned int> a, b, c, d;                         // MK: 4 Fifo channels for communtication between processes
    sc_signal<unsigned int> e;
 

public:
    void func1();
    void func2();
    void func3();
    void func4();

    SC_CTOR(basicModule) : a(10), b(10), c(10), d(10), e("e")       // Initialize fifos with 10 // DFFFFFFFFFFFFF
    // Above line is a macro to below two lines. Use line below if you want to have constructor arguments for your SystemC module it is preferable not to use SC_CTOR, declare the normal constructor and use the SC_HASPROCESS instread.:
    //SC_HAS_PROCESS(basicModule);                                                                  // This is a macro for "typedef XYZ SC_CURRENT_USER_MODULE"
    //basicModule(const sc_module_name &name) : sc_module(name)
    // OR
    //SC_HAS_PROCESS(basicModule);
    //basicModule::basicModule( ::sc_core::sc_module_name ): a(10), b(10), c(10), d(10), e("e")

    {
        // ADD THINGS HERE
        SC_THREAD(func1);
        // dont_initialize();          // After each one // During Initialization, each process is executed once (for SC_METHOD) or until a synchronization point (i.e. wait()) is reached (for SC_THREAD). To turn off initialization for a process, we may call dont_initialize() after its SC_METHOD or SC_THREAD declaration inside the constructor.
        SC_THREAD(func2);
        SC_THREAD(func3);

        SC_METHOD(func4);

        //sensitive << in1 << in2 ;
        // OR 
        sensitive   << a.data_read_event() << a.data_written_event()       // MK: sensitiv to a read or write from each of the FIFOs
                    << b.data_read_event() << b.data_written_event()
                    << c.data_read_event() << c.data_written_event()
                    << d.data_read_event() << d.data_written_event();
        b.write(1);
        c.write(0);
    }
};

#endif // BASIC_MODULE_H