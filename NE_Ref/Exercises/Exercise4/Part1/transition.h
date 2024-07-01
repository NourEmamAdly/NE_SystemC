#ifndef TRANSITION_H
#define TRANSITION_H

#include <systemc.h>

#include "place.h"
/****************************************************************************************************************************************
 *                        Implementing Transition module which consists  of two custom ports                                            *
 ****************************************************************************************************************************************/
// Transition:
SC_MODULE(transition)
{
//defining two custom ports Input and Output Ports for the transition oduled
sc_port<placeInterface> in;
sc_port<placeInterface> out;

public:
void fire()
{
//as the input port is derived from the same place interface so we can access the functions of the channel that is derived from the derived Interface
unsigned int temp =in->testTokens();

//if the value of tokens at the transistion module input port at least one token then print fired and then remove a token from the input and add token to the output
if (temp>0) 
{
    std::cout << this->name() << ":Fired" << std::endl;
    in->removeTokens(1);
    out->addTokens(1);
}
//if the value of tokens at the transistion module input port is zero then print this function
else
{
std::cout<<this->name()<< ": NOT Fired"<<std::endl;
}

}

SC_CTOR(transition){};


};

#endif // TRANSITION_H
