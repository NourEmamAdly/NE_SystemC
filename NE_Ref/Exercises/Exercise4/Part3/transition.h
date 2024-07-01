#ifndef TRANSITION_H
#define TRANSITION_H

#include <systemc.h>

#include "place.h"
using namespace std;

// Transition:

//In order to have more than one port for in and out we template the transistion module as follows to support multiple input ports
// N is the number of input ports , M is the number of output ports
template<unsigned int N=1, unsigned int M=1>
SC_MODULE(transition)
{

public:
sc_port<placeInterface, N,SC_ALL_BOUND> in;
sc_port<placeInterface, M,SC_ALL_BOUND> out;

public:
SC_CTOR(transition): in("in") ,out("out"){}

public:
void fire()
{
int counterToken=0;
unsigned int temp =in->testTokens();

for (int i=0;i<N;i++)
{
    if (temp >=1)
    {

        counterToken++; //Count how many input ports that have at least one token

    }


}


if (counterToken >= N)
{

cout << this->name() << ": Fired" <<endl;


for (int j=0;j<N;j++)
{
in[j]->removeTokens();
}

for (int k=0;k<N;k++)
{
out[k]->addTokens();
}

}

else
{
cout << this->name() <<":NOT Fired" <<endl;

}

}



};


#endif // TRANSITION_H
