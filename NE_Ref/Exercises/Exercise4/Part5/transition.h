#ifndef TRANSITION_H
#define TRANSITION_H

#include <systemc.h>

#include "place.h"
using namespace std;

// Transition:

//In order to have more than one port for in and out we template the transistion module as follows to support multiple input ports
// N is the number of input ports , M is the number of output ports and an inhebitor to check if there is a certain value at it or not to perform the functions
template<unsigned int N=1, unsigned int M=1, unsigned int L=0>
SC_MODULE(transition)
{

//Inestantiating ports from the interface we derived from the sc_interface
public:
sc_port<placeInterface, N,SC_ALL_BOUND> in;
sc_port<placeInterface, M,SC_ALL_BOUND> out;
sc_port<placeInterface, L, SC_ZERO_OR_MORE_BOUND> inhibitors;

//defining constructor of the transistion module we are creating here
public:
 SC_CTOR(transition): in("in"), out("out"), inhibitors("inhibitors"){}

//implementing a function here called fire that checks how many inputs ports that have at least one token
// and do other checks and print output
public:
void fire()
{
 //check Places
        int counterToken = 0;
        int counterInh = 0;
        for (int i = 0; i < N; i++)
        {
            if (in[i]->testTokens())
            {
                counterToken++;
            }
        }

        
        for (int i = 0; i < L; i++)
        {
            if (inhibitors[i]->getTokens() > 0)
            {
                counterInh++;
            }
        }

        //if all previous places have tokens
        if ((counterToken == N) && (counterInh == 0))  //----- Task 5
        {
            cout << this->name() << ": Fired" << endl;

            for (int i = 0; i < N; i++){
                in[i]->removeTokens();
            }
            for (int i = 0; i < M; i++){
                out[i]->addTokens();
            }

        }else{
            cout << this->name() << ": NOT Fired" << endl;
        }
    }

};


#endif // TRANSITION_H
