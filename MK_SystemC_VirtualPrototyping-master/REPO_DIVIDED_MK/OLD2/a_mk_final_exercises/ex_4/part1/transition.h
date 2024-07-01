#ifndef TRANSITION_H
#define TRANSITION_H

#include <systemc.h>

#include "place.h"

// Transition:
// TODO
SC_MODULE(transition){
    sc_port< placeInterface > in;                         // Create a port.
    sc_port< placeInterface > out;                         // Create a port.

    public:   // DF or it will cause a problem
        SC_CTOR(transition){};          // DF semi-colon if want to not define anything

        void fire(){
            unsigned int toks = in->testTokens();
            if(toks > 0){
                in->removeTokens(1);
                out->addTokens(1);
                std::cout << this->name() << ": Fired" << std::endl;
            }else{
                std::cout << this->name() << ": NOT Fired" << std::endl;
            }
        }
};
#endif // TRANSITION_H
