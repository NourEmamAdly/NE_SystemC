#include "nand.h"       // Headers include is with quotations.


int sc_main(int argc, char  *argv[])
{
    // 1. Create the Signals
    sc_signal <bool> A, B, Z;

    // 2. Instantiate the NAND gate module
    nand my_nand("my_nand");

    // 3. Bind the signals
    my_nand.A(A);
    my_nand.B(B);
    my_nand.Z(Z);

    // 4. Initialize the signals & Display the result
    A= true;
    B = true;

    // 5. Run the simulation
    //sc_start(5, SC_NS);
    sc_start();
    // 6. Display the result
    std::cout << "NAND Gate Result (A=" << A << ", B=" << B << "): Z=" << Z << std::endl;

    return 0;
}