#ifndef TRANSITION_H
#define TRANSITION_H

#include <systemc.h>

#include "place.h"

// Transition:
SC_MODULE(transition)
{

sc_port<placeInterface> in;
sc_port<placeInterface> out;

public:
void fire()
{

unsigned int temp =in->testTokens();

if (temp>0)
{
    std::cout << this->name() << ":Fired" << std::endl;
    in->removeTokens(1);
    out->addTokens(1);
}
else
{
std::cout<<this->name()<< ": NOT Fired"<<std::endl;
}

}

SC_CTOR(transition){};


};

#endif // TRANSITION_H