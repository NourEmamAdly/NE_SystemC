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
// Discussing systemc bit vector 
int sc_main (int __attribute__((unused)) sc_argc,
             char __attribute__((unused)) *sc_argv[])
{
    sc_bv<2> a = 2;                                                 // MK: bit vector, it is templated so takes the number of inputs 2 and set them to the decimal number two 
    sc_bv<2> b = "10";                                              // MK: same as above
    std::cout << a << std::endl; // 10                              // MK: printed as binary             
    a = 5;                                                          // MK: overflow will happen
    std::cout << a << std::endl; // 01 OVERFLOW
    a = a | b;                                                      // MK: All operations are overloaded, so can use all operations on the binary numbers
    std::cout << a << std::endl; // 11
    bool c = a.and_reduce();                                        // MK: Bitwise and operation of all the bits in a 
    std::cout << c << std::endl; // 1

    sc_bv<6> d = "000000";
    d.range(0,3) = "1111";                                          // MK: Work on a specific range.            
    std::cout << d << std::endl; // 001111
    std::cout << d(0,3) << std::endl; // 1111
    std::cout << d.range(0,3) << std::endl; // 1111
    std::cout << d[0] << std::endl; // 1                            // MK: Access each bit alone

    d = (a, d.range(0,3));                                          // MK: Concatenate two bit vectors
    std::cout << d << std::endl; // 111111

    return 0;
}




