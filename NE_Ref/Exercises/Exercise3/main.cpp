#include <iostream>
#include <systemc.h>

#include "state_machine.h"
#include "stimuli.h"
/****************************************************************************************************************************************
 *                        Implementing Main function to test the State machine Module with the stimuli                                  *
 ****************************************************************************************************************************************/
int sc_main(int, char**)
{
    //instentiating state machine object called genomeParser
    stateMachine genomeParser("genomeParser");
    //Inistentiating a stimuli object called genomeInput " this will be the input to the State machine object"
    stimuli genomeInput("genomeInput");
    //defining an sc-clock to bind it to the stimuli object
    sc_clock clk("clk", sc_time(1, SC_NS));
    // defining a helping signal between the stimuli and the state machine objects
    sc_signal<char> h1;
 // bind the state machine input port to the helping signal comming from the stimuli
    genomeParser.input.bind(h1);
    //bind the stimuli Output pin to the same helping signal its the input to the state machine object
    genomeInput.output.bind(h1);
    // bind the state machine object clck pin to the created clock here
    genomeParser.clk.bind(clk);
    // connect the stimuli output clck pin to the same clock its the input clk to the state machine object
    genomeInput.clk.bind(clk);

    sc_start();

    return 0;
}
