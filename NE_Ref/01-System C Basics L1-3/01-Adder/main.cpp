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


/*class adder : public sc_module {                                   SC_MODULE(adder){



};                                                  =              }; */  

SC_MODULE(adder)
{

//defining input and output signals (Input and output ports)
sc_in<int> a;
sc_in<int> b;
sc_out<int> c;

/* 
its a variable not signal
int nour;

*/
//defining a process
void compute()
{
//c=a+b; --> when you deal with variables not signals
c.write(a.read()+b.read()); //thats we write to o/p c the adding of a and b

}

// Defining Class constructor it has same name as class
// it will be called when an object of this class created

SC_CTOR(adder)
{

// you register now what is called system c method with the name of the process
// system kernel knows now when the adder should do the computation kernel will call this process
SC_METHOD(compute);

//Sinsitivity list this method is sensitive to signal a & b ,sesitivity tell the kernel that when any change happen with a or b this method should be called
sensitive << a << b;

}


};

int sc_main (int __attribute__((unused)) sc_argc,
             char __attribute__((unused)) *sc_argv[])
{
    std::cout << "\nT\t\tS\tR\tQ\tN" << std::endl;
    toplevel t("t");

    sc_set_stop_mode(SC_STOP_FINISH_DELTA);
    sc_start();

    return 0;
}
