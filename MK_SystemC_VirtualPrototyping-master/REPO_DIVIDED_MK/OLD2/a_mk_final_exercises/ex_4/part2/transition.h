#ifndef TRANSITION_H
#define TRANSITION_H

#include <systemc.h>

#include "place.h"

// Transition:
// TODO
template<unsigned int N = 1, unsigned int M = 1>
SC_MODULE(transition){
    sc_port<placeInterface, N, SC_ALL_BOUND> in;                         // Create a port.
    sc_port<placeInterface, M, SC_ALL_BOUND> out;                       // Create a port.

    public:   // DF or it will cause a problem
        SC_CTOR(transition){};          // DF semi-colon if want to not define anything

        void fire(){
            unsigned int all_have_at_least_1_token = 1;
            for(int i = 0; i<N; i++){
                unsigned int toks = in[i]->testTokens();
                if(toks == 0){
                    all_have_at_least_1_token = 0;
                }
            }
            if(all_have_at_least_1_token==1){
                std::cout << this->name() << ": Fired" << std::endl;
                for(int i = 0; i<N; i++){
                    in[i]->removeTokens(1);
                }
                for(int i = 0; i<M; i++){
                    out[i]->addTokens(1);
                }

            }else{
                std::cout << this->name() << ": NOT Fired" << std::endl;
            }
        }
};
#endif // TRANSITION_H
