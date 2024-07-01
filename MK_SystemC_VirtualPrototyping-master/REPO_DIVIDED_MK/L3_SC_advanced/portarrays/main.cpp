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

using namespace std;
// MK: From Lecture:
/*
-Static port creation at compile time
-Connected channels are addressed with [] operator
*/
template <int N=1>                      // Is templated
SC_MODULE(module)
{
    // Instead of
    //sc_port<sc_fifo_out_if<int> > port1;
    //sc_port<sc_fifo_out_if<int> > port2;
    //sc_port<sc_fifo_out_if<int> > port3;

    sc_port<sc_fifo_out_if<int> > port[N];

    SC_CTOR(module)
    {
        SC_THREAD(process);
    }

    void process()
    {
        wait(SC_ZERO_TIME);

        for(int i=0; i < N; i++)
        {
           port[i]->write(2);
           std::cout << this->name()
                     << ": Write to port " << i << std::endl;
           wait(1, SC_NS);
        }

        std::cout << flush;
    }

};

int sc_main(int __attribute__((unused)) argc,
            char __attribute__((unused)) *argv[])
{
    /*module<3> m("m");
    sc_fifo<int> f1, f2, f3, f4;

    m.port[0].bind(f1);
    m.port[1].bind(f2);
    m.port[2].bind(f3);

    module<1> n("n");
    module<1> o("o");

    n.port[0].bind(f4);
    o.port[0].bind(f4);*/

    // MK_GPT: Declare a module with 3 ports named "m"
    module<3> m("m");
    // MK_GPT: Declare four FIFO channels f1, f2, f3, f4
    sc_fifo<int> f1, f2, f3, f4;
    // MK_GPT: Bind the first port of module m to FIFO f1
    m.port[0].bind(f1);
    // MK_GPT: Bind the second port of module m to FIFO f2
    m.port[1].bind(f2);
    // MK_GPT: Bind the third port of module m to FIFO f3
    m.port[2].bind(f3);
    // MK_GPT: Declare a module with 1 port named "n"
    module<1> n("n");
    // MK_GPT: Declare a module with 1 port named "o"
    module<1> o("o");
    // MK_GPT: Bind the port of module n to FIFO f4
    n.port[0].bind(f4);
    // MK_GPT: Bind the port of module o to the same FIFO f4
    o.port[0].bind(f4);


    sc_start();
    return 0;
}
