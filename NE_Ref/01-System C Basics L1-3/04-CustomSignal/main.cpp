

/**************************************************************************************************************
 *                                                                                                            *
 *                      How to create a custome Signal as the one we have SC_Signal                           *      
 *                                                                                                            *
 *                                                                                                            *
 * ************************************************************************************************************/
 
#include <systemc.h>
//TO PRINT OUT
#include <iostream>


using namespace std;

/**************************************************************************************************************
 *                                NOW WE DEFINE sIGNAL AND DERIVE IT FROM SC Interface                        *
 * ************************************************************************************************************/
template <class T>
//we create simple signal interface class where we drive it from sc_interface
class  SignalInterface : public sc_interface
{
public:
//WE DEFINE HERE 2 VIRTUAL FUNCTIONS FOR READING AND WRITING of the signal
// INTERFACE DESCRIBES NOW HOW WE CAN COMMUNICATE WITH FIFO CHANNEL
virtual T read()= 0;
virtual void write(T)= 0;


};


/**************************************************************************************************************
 *                              NOW WE WILL create a sIGNAL Channel as the sc_signal channel                  *     
 *          WE DRIVE A signal CHANNEL FROM THE signal INTERFACE WE CREATED AND IMPLEMENT THE VIRTUAL FUNTIONs  * 
 * ************************************************************************************************************/

template <class T>
//creating a signal channel that drives from the signal interface and also we want to be able to add to our signal sinsitivity list
// so we drive also from SC primitive channel THAT WILL MAKE USE THE SENSITIVITY LIST AS THE SC_SIGNAL CHANNEL
class signal : public SignalInterface <T>,public sc_prim_channel
{

//we define current value of signal ,new value of signal
private:
T currentValue;
T newValue;

//we create this event ,this is the event that will be later added to the sensitivity list and everytime the value changes we fire this event
//Then system c kernel knows that signal changes 
sc_event valuechangedEvent;

public:
//defining cONSTRUCTOR
signal()
{
currentValue= 0;
newValue = 0;
}


//implementing virtual functions of the signal for read and write
T read()
{

    return currentValue;
}


//There are three things you have to do when you write primitive channel :
// 1- You have to call the request update to notify the kernel
// 2- you have to implement the update function
// 3- you have to build the default event 
//val is the value you want to write to the signal
void write(T val)
{
newValue= val;
//we check if the new value and the current value is not same so we call the kernel and inform it that there is an update our signal changes
if(newValue != currentValue)
{
//notify systemc kernel and then the kernel will check what is sensitive to this signals
request_update();

}

}
//system kernel apply update and notify that the value changed
void update()
{
    if(newValue != currentValue)
    {
        currentValue = newValue;
        valuechangedEvent.notify(SC_ZERO_TIME);
    }
}



//this the key for sensitivity list when you write sensitivity list its the shortcut of this fuunction it ret
const sc_event& default_event() const
{
    return valuechangedEvent;
}


};

/**************************************************************************************************************
 *                        NOW WE Create Two Modules and create ports using our Signal interface               *
 *                         WE USE THREADS AS WE USE WAIT ON EVENT AND METHODS WILL GET AN ERROR               *
 * ************************************************************************************************************/

SC_MODULE(PRODUCER)
{
//producer will have a port and the port is described by our interface
//here we inestantiated a port from the infterface class that we created
sc_port<SignalInterface<int>> master;

SC_CTOR(PRODUCER)
{
SC_THREAD(process);

}

void process()
{
    master->write(10);
    wait(10,SC_NS);
    master->write(20);
    wait(20,SC_NS);
    sc_stop();


}


};



SC_MODULE(CONSUMER)
{
sc_port<SignalInterface<int>> slave;

SC_CTOR(CONSUMER)
{
SC_METHOD(process);
sensitive << slave;
dont_initialize();


}

void process()
{
    //hena ana ba3mel read mn al port  al 3amlltello creation and print it
    cout <<"@"<<sc_time_stamp()<<"C: " << slave->read()<<endl;

}



};


int sc_main (int __attribute__((unused)) sc_argc,
             char __attribute__((unused)) *sc_argv[])
{
    PRODUCER P1("Producer1");
    CONSUMER C1("Consumer1");


    signal<int> channel;
    
    P1.master.bind(channel);
    C1.slave.bind(channel);


sc_start();  // Added to start the simulation

    return 0;
}
