#ifndef EXOR_H
#define EXOR_H

#include <systemc.h>

#include "nand.h"

// TODO: Insert your code here
SC_MODULE(exor){
    sc_in<bool> A;
    sc_in<bool> B;
    sc_out<bool> Z;

    nand nand1, nand2, nand3, nand4;    // We need to initialize for nands.
    sc_signal<bool> h1, h2, h3;         // signals to carry the intermediate values.

    // All inputs, outputs as well as handling signals should be initialized with a name in the CTOR.
    SC_CTOR(exor) : nand1("Nand_1"), nand2("Nand_2"), nand3("Nand_3"), nand4("Nand_4"), h1("H1"), h2("H2"), h3("H3"), A("XOR_IN1"), B("XOR_IN2"), Z("XOR_OUT")
    {
        // First Nand:
        nand1.A.bind(A);
        nand1.B.bind(B);
        nand1.Z.bind(h1);

        // Second Nand:
        nand2.A.bind(h1);
        nand2.B.bind(B);
        nand2.Z.bind(h2);

        // Third Nand:
        nand3.A.bind(A);
        nand3.B.bind(h1);
        nand3.Z.bind(h3);

        // Fourth Nand:
        nand4.A.bind(h2);
        nand4.B.bind(h3);
        nand4.Z.bind(Z);
    }
};

#endif // EXOR_H
