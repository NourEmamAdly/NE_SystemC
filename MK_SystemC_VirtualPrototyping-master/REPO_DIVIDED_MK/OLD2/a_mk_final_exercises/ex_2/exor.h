#ifndef EXOR_H
#define EXOR_H

#include <systemc.h>

#include "nand.h"

// TODO: Insert your code here
SC_MODULE(exor){
    public:
    nand nand1, nand2, nand3, nand4;        // DF
    sc_in<bool> A;                          // DF                         
    sc_in<bool> B;                          // DF
    sc_out<bool> Z;                         // DF
    sc_signal<bool> h1, h2, h3;                     // signals to carry the intermediate values.

    // All inputs, outputs as well as handling signals should be initialized with a name in the CTOR.
    SC_CTOR(exor) : nand1("NAND_A"), nand2("NAND_B"), nand3("NAND_C"), nand4("NAND_D"), A("Input_1"), B("Input_2"), Z("Output"), h1("Helper_Signal_1"), h2("Helper_Signal_2"), h3("Helper_Signal_3")
    {
        // First NAND
        nand1.A.bind(A);
        nand1.B.bind(B);
        nand1.Z.bind(h1);

        // Second NAND
        nand2.A.bind(A);
        nand2.B.bind(h1);
        nand2.Z.bind(h2);

        // Third NAND
        nand3.A.bind(h1);
        nand3.B.bind(B);
        nand3.Z.bind(h3);

        // Fourth NAND
        nand4.A.bind(h2);
        nand4.B.bind(h3);
        nand4.Z.bind(Z);

    }

};

#endif // EXOR_H
