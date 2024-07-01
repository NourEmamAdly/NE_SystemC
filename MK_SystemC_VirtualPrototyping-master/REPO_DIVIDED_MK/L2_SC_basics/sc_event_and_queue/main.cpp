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
// MK: IDEA: Difference btw using SC_EVENT and SC_EVENT_QUEUE
// Using SC_EVENT:
//  - Events are fired using "notify()" function
//  - ONLY the FIRST notification is noted. Triggered only once
//  - Other following notifications are ignored.
// Using SC_EVENT_QUEUE:
//  - Events are fired using "notify()" function
//  - ALL notifications are noted. Triggers ALL.
//  - EVEN ORDERS THE EVENTS IN ASCENDING ORDER.
using namespace std;

// EVENT
SC_MODULE(eventTester)
{
    sc_event triggerEvent;

    SC_CTOR(eventTester)
    {
        SC_THREAD(triggerProcess);
        SC_METHOD(sensitiveProcess);
        sensitive << triggerEvent;
        dont_initialize();
    }

    void triggerProcess()
    {
       wait(SC_ZERO_TIME);
       triggerEvent.notify(10,SC_NS);
       triggerEvent.notify(20,SC_NS); // Will be ignored
       triggerEvent.notify(30,SC_NS); // Will be ignored
    }

    void sensitiveProcess()
    {
        cout << "EVENT: @" << sc_time_stamp() << endl;
    }
};

// EVENT QUEUE
SC_MODULE(eventQueueTester)
{
    sc_event_queue triggerEventQueue;

    SC_CTOR(eventQueueTester)
    {
        SC_THREAD(triggerProcess);
        SC_METHOD(sensitiveProcess);
        sensitive << triggerEventQueue;
        dont_initialize();
    }

    void triggerProcess()
    {
       wait(100,SC_NS);
       triggerEventQueue.notify(10,SC_NS);
       triggerEventQueue.notify(20,SC_NS); // Will NOT be ignored
       triggerEventQueue.notify(40,SC_NS); // Will NOT be ignored       // WILL ORDER THE EVENTS
       triggerEventQueue.notify(30,SC_NS); // Will NOT be ignored
    }

    void sensitiveProcess()
    {
        cout << "QUEUE: @" << sc_time_stamp() << endl;
    }
};

int sc_main(int __attribute__((unused)) argc,
            char __attribute__((unused)) *argv[])
{
    eventTester et("et");
    eventQueueTester eqt("eqt");
    sc_start();
    return 0;
}
