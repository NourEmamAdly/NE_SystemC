#ifndef EXOR_H
#define EXOR_H

#include <systemc.h>

#include "nand.h"

// TODO: Insert your code here

SC_MODULE(exor)
{
// Inistantiatiog NAND's from the Nand Class 
public:
nand nand1 , nand2, nand3, nand4;

//Defining Inputs and outputs
sc_in<bool> A;
sc_in<bool> B;
sc_out<bool> Z;

//Defining Signals to carry the intermediate values.
sc_signal<bool> h1;
sc_signal<bool> h2;
sc_signal<bool> h3;

// NOTE: "All inputs,outputs as well as handling signals should be initialized with a name in the CTOR"  //defining values for debugging
SC_CTOR(exor) : nand1("NAND_1"),nand2("NAND_2"),nand3("NAND_3"),nand4("NAND_4"),A("Input_A"),B("Input_B"),Z("Output_1"),h1("signal1"),h2("signal2"),h3("signal3")

{

//First NAND
nand1.A.bind(A);
nand1.B.bind(B);
nand1.Z.bind(h1);


//Second NAND
nand2.A.bind(A);
nand2.B.bind(h1);
nand2.Z.bind(h2);

//Third NAND
nand3.A.bind(h1);
nand3.B.bind(B);
nand3.Z.bind(h3);


//Third NAND
nand4.A.bind(h2);
nand4.B.bind(h3);
nand4.Z.bind(Z);




}



};

#endif // EXOR_H
