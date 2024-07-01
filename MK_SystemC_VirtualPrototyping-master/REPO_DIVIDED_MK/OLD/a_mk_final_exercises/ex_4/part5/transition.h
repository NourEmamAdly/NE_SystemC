#ifndef TRANSITION_H
#define TRANSITION_H

#include <systemc.h>

#include "place.h"

// Transition:
// TODO
template<unsigned int N = 1, unsigned int M = 1, unsigned int L=0>
SC_MODULE(transition){
    sc_port<placeInterface, N, SC_ALL_BOUND> in;                         // Create a port.
    sc_port<placeInterface, M, SC_ALL_BOUND> out;                       // Create a port.
    sc_port<placeInterface, L, SC_ZERO_OR_MORE_BOUND> inhibitors;


    public:   // DF or it will cause a problem
        SC_CTOR(transition){};          // DF semi-colon if want to not define anything

        void fire(){
            unsigned int conditions_satisfied = 1;
            for(int i = 0; i<N; i++){
                bool toks = in[i]->testTokens();
                if(toks == 0){
                    conditions_satisfied = 0;
                }
            }
            for(int i = 0; i<L; i++){
                bool inhibs = inhibitors[i]->testTokens();
                if(inhibs == 1){
                    conditions_satisfied = 0;
                }
            }
            if(conditions_satisfied==1){
                std::cout << this->name() << ": Fired" << std::endl;
                for(int i = 0; i<N; i++){
                    in[i]->removeTokens();
                }
                for(int i = 0; i<M; i++){
                    out[i]->addTokens();
                }
            }else{
                std::cout << this->name() << ": NOT Fired" << std::endl;
            }
        }

    
        
};
#endif // TRANSITION_H
