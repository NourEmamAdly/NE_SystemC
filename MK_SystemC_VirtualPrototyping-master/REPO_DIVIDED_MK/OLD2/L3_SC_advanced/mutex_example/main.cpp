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
// MK: from lecture slides:
/*
◼ With the help of a so-called Mutex (mutual exclusive), the simultaneous access of
several processes to shared data structures can be regulated in software engineering.
◼ The primitive channel sc_mutex implements a corresponding lock mechanism, i.e. a
mutex will be in one of two exclusive states: unlocked or locked.
◼ This channel is primarily intended for use with multiple processes within a module,
but there is also an interface sc_mutex_if, so ports of this type can also be created.
◼ Only one process can lock a given mutex at one time. A mutex can only be unlocked
by the particular process that locked the mutex, but may be locked subsequently by
a different process.
◼ The sc_mutex class comes with pre-defined methods:
◼ int lock(): Lock the mutex if it is free, else wait till mutex gets free
◼ int unlock(): Unlock the mutex, returns -1 if mutex was not locked
◼ int trylock(): Check if mutex is free, if free then lock it else return -1.
*/
SC_MODULE (sc_mutex_example) {
    sc_in<bool> clock;

    sc_mutex bus;

    void busAccess(int who) {
        cout << "@" << sc_time_stamp() <<" Bus access by " << who << endl;
    }

    void process0() {
        while (true) {
            wait();
            cout << "@" << sc_time_stamp() << " Checking mutex 0" << endl;
            // Check if mutex is available
            if (bus.trylock() != -1) {                                              // Lock is free when returns -1, if free locks it
                cout << "@" << sc_time_stamp() << " Got mutex for 0" << endl;
                busAccess(0);
                wait(2); // Wait for two cycles
                // Unlock the mutex
                cout << "@" << sc_time_stamp() << " 0 mutex unlocked" << endl;
                bus.unlock();
            }
        }
    }

    void process1() {
        while (true) {
            wait();
            cout << "@" << sc_time_stamp() <<" Checking mutex 1"<<endl;
            // Wait till mutex is available
            bus.lock();
            cout << "@" << sc_time_stamp() <<" Got mutex for 1"<<endl;
            busAccess(1);
            wait(3); // Wait for three cycles
            // Unlock the mutex
            cout << "@" << sc_time_stamp() << " 1 mutex unlocked" << endl;
            bus.unlock();
        }
    }

    SC_CTOR(sc_mutex_example) {
        SC_THREAD(process0);
        sensitive << clock;
        SC_THREAD(process1);
        sensitive << clock;
    }
};


int sc_main(int __attribute__((unused)) argc,
            char __attribute__((unused)) *argv[])
{
    sc_clock clock ("my_clock",sc_time(1,SC_NS));

    sc_mutex_example object("wait");
    object.clock (clock);

    sc_start(10,SC_NS);
    return 0;
}
