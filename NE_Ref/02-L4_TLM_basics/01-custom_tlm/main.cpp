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

enum cmd {READ, WRITE};

// MK:  This is the data packet and we dont transfer it but a pointer to it
struct transaction
{
    unsigned int data;
    unsigned int addr;
    cmd command;                // MK: enum to know if read/write
};

class transactionInterface : public sc_interface
{
    public:
    virtual void transport(transaction &trans) = 0;             // MK: pass by reference (pointer) not value. This is the function that will be called.
};

// The Initiator. This could be a CPU for example.
SC_MODULE(PRODUCER)
{
    sc_port< transactionInterface > master;

    SC_CTOR(PRODUCER)
    {
        SC_THREAD(process);
    }

    void process()
    {
        // Write:
        for(unsigned int i=0; i < 4; i++)
        {
            wait(1,SC_NS);
            transaction trans;
            trans.addr = i;
            trans.data = rand();
            trans.command = cmd::WRITE;
            master->transport(trans);                           // This is the call of the transport function
        }

        // Read:
        for(unsigned int i=0; i < 4; i++)
        {
            wait(1,SC_NS);
            transaction trans;
            trans.addr = i;
            trans.data = 0;
            trans.command = cmd::READ;
            master->transport(trans);
            cout << trans.data << endl;
        }
    }
};

// This is the target, defined this way to derive it from the transaction interface as well and need to implement tht virtual pure function
// This could be for example a memory.
// This "class CONSUMER : public sc_module" is exactly the same as SC_MODULE(CONSUMER)
class CONSUMER : public sc_module, public transactionInterface
{
    private:
    unsigned int memory[1024];

    public:
    SC_CTOR(CONSUMER)
    {
        for(unsigned int i=0; i < 1024; i++)
        {
            memory[i] = 0; // Initialize memory
        }
    }
    // This function is blocking 
    void transport(transaction &trans)
    {
        if(trans.command == cmd::WRITE)
        {
            memory[trans.addr] = trans.data;
        }
        else // cmd::READ
        {
            trans.data = memory[trans.addr];
        }
    }
};

int sc_main(int __attribute__((unused)) argc,
            char __attribute__((unused)) *argv[])
{
    PRODUCER cpu("cpu");
    CONSUMER mem("memory");

    cpu.master.bind(mem);           // this is the binding. from lecture "The produce is active, the consumer is passive"

    sc_start();

    return 0;
}
