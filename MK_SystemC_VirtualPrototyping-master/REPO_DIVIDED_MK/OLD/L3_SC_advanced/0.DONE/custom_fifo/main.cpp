/*
 * Copyright 2017 Matthias Jung
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors:
 *     - Matthias Jung
 */

#include <iostream>
#include <systemc.h>
#include <queue>

using namespace std;
// SystemC has a fifo, but here we do that to understand how to implement a custom channel
// SystemC allows the creation of custom channels according to your needs
// Interface methods are allowed to block by calling wait statements (Note that only in SC_THREADs these methods can be called)
// FIFO should implement blocking read (IF fifo is empty) and blocking write (if fifo is full)
// FIFOInterface should have pure virtual functions for read and write

// Interface describes how we can communicate with the fifo (read, write methods)
template <class T>                                                          // Templated for generalization for any datatype
class SimpleFIFOInterface : public sc_interface                             // is derived from the sc_interface
{
    public:
    virtual T read() = 0;                                                   // pure virtual read function
    virtual void write(T) = 0;
    // Just for Debug
    virtual void printFIFO()
    {
        cout << "Warning: Debug Function Not Implemented" << endl;
    }
};

// Creating a custom channel
template <class T>                                                          // has to be templated as interface is templated
class SimpleFIFO : public SimpleFIFOInterface<T>
{
    private:                                                                // TC
    std::queue<T> fifo;                                                     
    sc_event writtenEvent;                                                  // Events are needed to unlock Consumer/Product
    sc_event readEvent;
    unsigned int maxSize;

    public:
    SimpleFIFO(unsigned int size=16) : maxSize(size)                        // 16 is a default size
    {
    }

    T read()
    {
        if(fifo.empty() == true)                                            // blocking behavior (if it is empty)
        {
            std::cout << "Wait for Write" << std::endl;
            wait(writtenEvent);                                            // wait until an event happens
        }
        T val = fifo.front();                                               // get the first element from the fifo, save in val
        fifo.pop();                                                         // pop it from the fifo, remove it.
        readEvent.notify(SC_ZERO_TIME);                                     // notify the write that a read has happen to know that there is an empty space now
        return val;
    }

    void write(T d)
    {
        if(fifo.size() == maxSize)                                          // fifo is full
        {
            std::cout << "Wait for Read" << std::endl;
            wait(readEvent);
        }
        fifo.push(d);
        writtenEvent.notify(SC_ZERO_TIME);
    }

    void printFIFO()
    {
        unsigned int n = fifo.size();

        std::cout << "SimpleFIFO (" << maxSize << ") " << "[";
        for(unsigned int i = 0; i < n; i++) {
            std::cout << "█";
        }
        for(unsigned int i = 0; i < maxSize-n; i++) {
            std::cout << " ";
        }
        std::cout << "]" << std::endl;
        std::cout.flush();
    }
};

// Create the producer module and use the fifo
SC_MODULE(PRODUCER)
{
    sc_port< SimpleFIFOInterface<int> > master;                         // Create a port, described by the fifo interface with integers

    SC_CTOR(PRODUCER)
    {
        SC_THREAD(process);
    }

    void process()                                                      // Produces data every 1 ns
    {
        while(true)
        {
            wait(1,SC_NS);
            master->write(10);                                          // TC it is an arrow not .
            std::cout << "@" << sc_time_stamp() << " Write: 10 ";
            master->printFIFO();
        }
    }
};

// Create a consumer port
SC_MODULE(CONSUMER)
{
    sc_port< SimpleFIFOInterface<int> > slave;                          // Create a port

    SC_CTOR(CONSUMER)
    {
        SC_THREAD(process);
        sensitive << slave;
    }

    void process()                                                     // Consumes data every 4 ns (prints it)
    {
        while(true)
        {
            wait(4,SC_NS);
            std::cout << "@" << sc_time_stamp() << " Read : "
                      << slave->read() << " ";
            slave->printFIFO();
        }
    }
};

int sc_main(int __attribute__((unused)) argc,
            char __attribute__((unused)) *argv[])
{
    PRODUCER pro1("pro1");                                          // Instatiate a producer
    CONSUMER con1("con1");                                          // Instatiate a consumer
    SimpleFIFO<int> channel(4);                                     // Create a simple fifo with length 4

    sc_signal<int> foo;

    pro1.master.bind(channel);                                      // Bind producer master port to the channel
    con1.slave.bind(channel);                                       // Bind consumer slave port to the channel      Producer -> Channel -> Consumer

    sc_start(10,SC_NS);                                             // Dont forget

    return 0;
}
