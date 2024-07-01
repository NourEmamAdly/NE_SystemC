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

#include <systemc.h>
#include <iostream>
#include "producer.h"
#include "consumer.h"


// LECTURE 3 PAGE 27:
/*
sc_fifo<T> has following predefined methods:
- write(): This method writes the values passed as an argument into the FIFO. If the
FIFO is full then write() function waits until a FIFO slot is available
- nb_write(): This method is the same as write(), the only difference is, when the
fifo is full nb_write() does not wait until a free FIFO slot is available. Rather it
returns false.
- read(): This method returns the least recent written data in the FIFO. If the FIFO is
empty, then the read() function waits until data is available in the FIFO.
- nb_read(): This method is same as read(), the only difference is, when the FIFO is
empty, nb_read() does not wait until the FIFO has some data. Rather it returns false.
- num_available(): This method returns the numbers of data values available in the
FIFO in the current delta time.
- num_free(): This method returns the number of free slots available in the FIFO in the
current delta time.
*/


int sc_main(int __attribute__((unused)) argc,
            char __attribute__((unused)) *argv[])
{
    // MK_GPT: Defines a SystemC clock named 'clk1' with a period of 1 nanosecond (1, SC_NS) and a duty cycle of 50% (0.5).
    // MK_GPT: The clock is initially set to 0 nanoseconds (0), and it has a phase shift of 0 nanoseconds (0, SC_NS).
    // MK_GPT: The last argument 'true' indicates that the clock starts with a rising edge.
    /*
    sc_clock(const char* name_, const sc_time& period_, double duty_cycle_ = 0.5, const sc_time& start_time_ = SC_ZERO_TIME, bool posedge_first_ = true );
    */
    // Setup Clocks
    sc_clock clk1("clk1", 1, SC_NS, 0.5, 0, SC_NS, true);
    sc_clock clk2("clk2", 2, SC_NS, 0.5, 0, SC_NS, true);

    // Setup Producer
    producer p("producer");

    // Setup Consumer
    consumer c("consumer");

    // Setup communication channel:
    sc_fifo<unsigned int> channel(4);

    // Bind Signals:
    p.clk(clk1);
    p.fifo_port.bind(channel);

    c.clk(clk2);
    c.fifo_port.bind(channel);

    // Setup Waveform Tracing:
    sc_trace_file *wf = sc_create_vcd_trace_file("tff");
    wf->set_time_unit(1, SC_PS);
    sc_trace(wf, clk1, "clk1");
    sc_trace(wf, clk2, "clk2");

    // https://workspace.accellera.org/Discussion_Forums/community_forum/archive/msg?list_name=community_forum&monthdir=200411&msg=msg00013.html
    channel.trace(wf);

    sc_start();

    //channel.dump();

    sc_close_vcd_trace_file(wf);
    return 0;
}
