#ifndef NAND_H
#define NAND_H

#include <systemc.h>
/****************************************************************************************************************************************
 *                                                Implementing NAND Gate Module                                                         *
 ****************************************************************************************************************************************/

//defining the Nand Module
SC_MODULE(nand)
{
    //defining input and output ports of the module
    sc_in<bool> A;
    sc_in<bool> B;
    sc_out<bool> Z;

    //defining nand constructor and intialize it with names for debugging
    SC_CTOR(nand) : A("Input_A"),B("Input_B"),Z("Output_Z") //Strings for debugging
    {
   //defining do nand method that is sensitive to any change that happens in A & B
    SC_METHOD(do_nand);
    sensitive << A << B ;


    }
//Implementing the nand method
void do_nand()
{
    // Implementing NAND GATE function
   Z.write(!(A.read() && B.read()));

}



};
#endif
