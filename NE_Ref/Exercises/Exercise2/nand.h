#ifndef NAND_H
#define NAND_H

#include <systemc.h>


SC_MODULE(nand)
{
    sc_in<bool> A;
    sc_in<bool> B;
    sc_out<bool> Z;


    SC_CTOR(nand) : A("Input_A"),B("Input_B"),Z("Output_Z") //Strings for debugging
    {

    SC_METHOD(do_nand);
    sensitive << A << B ;


    }

void do_nand()
{
   Z.write(!(A.read() && B.read()));


}






};
#endif
