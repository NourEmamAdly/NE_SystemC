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
sc_port<placeInterface> in[N];
public:
SC_CTOR(transition): in("in") ,out("out"){}

public:
void fire()
{

int counterToken=0;
//check if every input port have at least one token and count how many input ports that have at least one token
for (int i=0;i<N;i++)
{
    if (in[i]->testTokens() >= 1)
    {

        counterToken++; //Count how many input ports that have at least one token

    }


}


if (counterToken == N) //^If all previous ports have a token, then...
        {
          // if all previous places have tokens
            cout << this->name() << ": Fired" << endl;
         // Then remove from each token from each input port and 
            for (int i = 0; i < N; i++){
                in[i]->removeTokens(1);     //----Task 1 and 2
                                            //^Remove tokens from previous place
            }
         // Then add token to each output port
            for (int i = 0; i < M; i++){
                out[i]->addTokens(1);  //-----Task 1 and 2
                                        //^Add token to next place
            }

        }
// otherwise print that this port is not fired
else{
            cout << this->name() << ": NOT Fired" << endl;
        }
    }

};





};


#endif // TRANSITION_H
