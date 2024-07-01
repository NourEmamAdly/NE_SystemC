

/**************************************************************************************************************
 *                                                                                                            *
 *                      How to create a custome interface and custom channel Code                             *      
 *                                                                                                            *
 *                                                                                                            *
 * ************************************************************************************************************/
 
#include <systemc.h>
//TO PRINT OUT
#include <iostream>
//to be able to use queues as we will not implement everything in fifo we just implement the concept
#include <queue>

using namespace std;

/**************************************************************************************************************
 *                                        NOW WE DEFINE FIFO Interface                                        *
 * ************************************************************************************************************/
template <class T>
//we create simple fifo interface class where we drive it from sc_interface
class  SimpleFIFOInterface : public sc_interface
{
public:
//WE DEFINE HERE 2 VIRTUAL FUNCTIONS FOR READING AND WRITING
// INTERFACE DESCRIBES NOW HOW WE CAN COMMUNICATE WITH FIFO CHANNEL
virtual T read()= 0;
virtual void write(T)= 0;


};


/**************************************************************************************************************
 *                                        NOW WE WILL BUILD THE FIFO channel                                  *     
 *                   WE DRIVE A CHANNEL FROM THE FIFO INTERFACE WE CREATED AND IMPLEMENT THE VIRTUAL FUNTION  * 
 * ************************************************************************************************************/

template <class T>
class SimpleFIFO : public SimpleFIFOInterface <T>
{
//we define queue that get elements of type T WHIC WE CALL FIFO
private:
queue<T> fifo;

//defining SC EVENT THAT WILL BE NOTIFING WHEN SOMETHING IS WRITTEN OR READ
sc_event writtenEvent;
sc_event readEvent;

//defining a variable to determine the max size of the queue
uint64_t maxsize;

public:
//defining constructor and defining the default size of the the queue is 16 and store that in max size .thats the initialization
SimpleFIFO(uint64_t size=16) : maxsize(size){}

//implementing the read virtual function of the interface inside the channel
T read()
{
    //before reading we check if fifo is empty then we wait until something is written to the FIFO until written event
    if(fifo.empty()==true)
    {
        wait(writtenEvent);

    }
    // we taje the first element of the fifo and then popit
    T val = fifo.front();
    fifo.pop();

    /* WE HAVE A READ EVENT AND WE WILL NOTIIFY IMMEDIATELY BECAUSE THIS READ EVENT IS THERE TO BLOCK WITH A 
       WAIT STATEMENT THE WRITE PROCESS WHEN THING IS FULL AND SOMEBODY READ FROM THE QUEUE THEN WE WRITE TO THE FIFO  */
    readEvent.notify(SC_ZERO_TIME);
    return val;


}

//implementing the read virtual function of the interface inside the channel
void write(T val)
{
    //WE CHECK IF THE FIFO IS FULL THEN WE WAIT FOR THE READ EVENT ONCE SOMEONE READ FROM THE FIFO THEEN WE CAN WRITE TO THE FREE PLACE
    if(fifo.size() == maxsize)
    {

        wait(readEvent);

    }
    //WE PUSH VALUE INTO THE FIFO THEN NOTIFY WITH A WRITE EVENT THAT MEANS THERE IS INFO IN THE FIFO YOU CAN READ IT 
    fifo.push(val);
    writtenEvent.notify(SC_ZERO_TIME);

}


};


/**************************************************************************************************************
 *                        NOW WE Create Two Modules and create ports using our interface                      *
 *                         WE USE THREADS AS WE USE WAIT ON EVENT AND METHODS WILL GET AN ERROR               *
 * ************************************************************************************************************/

SC_MODULE(PRODUCER)
{
//producer will have a port and the port is described by our interface
//here we inestantiated a port from the infterface class that we created
sc_port<SimpleFIFOInterface<int>> master;

SC_CTOR(PRODUCER)
{
SC_THREAD(process);

}

void process()
{
    int counter=0;
while(true)
{
    wait(1,SC_NS);
    //hena ana ba3mel write 3alla al port al 3amlltello creation here bal value bta3 al counter
        cout <<"@"<<sc_time_stamp()<<"P: " << counter<<endl;

    master->write(counter++);
}


}


};



SC_MODULE(CONSUMER)
{
sc_port<SimpleFIFOInterface<int>> slave;

SC_CTOR(CONSUMER)
{
SC_THREAD(process);

}

void process()
{
while(true)
{
    wait(1,SC_NS);
    //hena ana ba3mel read mn al port  al 3amlltello creation and print it
    cout <<"@"<<sc_time_stamp()<<"C: " << slave->read()<<endl;
}


}



};


int sc_main (int __attribute__((unused)) sc_argc,
             char __attribute__((unused)) *sc_argv[])
{
    PRODUCER P1("Producer1");
    CONSUMER C1("Consumer1");


    SimpleFIFO<int> channel(4);
    
    P1.master.bind(channel);
    C1.slave.bind(channel);


sc_start(10, SC_NS);  // Added to start the simulation

    return 0;
}
