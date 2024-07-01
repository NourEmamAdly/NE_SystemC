#ifndef TRANSITION_H
#define TRANSITION_H

#include <systemc.h>

#include "place.h"

// Transition:
// TODO
template<unsigned int N = 1, unsigned int M = 1>
SC_MODULE(transition)
{
    //sc_port<placeInterface> in;
    //sc_port<placeInterface> out;
    sc_port<placeInterface, N, SC_ALL_BOUND> in;
    sc_port<placeInterface, M, SC_ALL_BOUND> out;

    public:
        SC_CTOR(transition){};
        
        void fire(){
            unsigned int temp;
            for(int i=0; i<N; i++){
                temp = in[i]->testTokens();
            }
            if(temp > 0){
                std::cout << this->name() << ": Fired" << std::endl;
                for (int k = 0; k < N; k++){
                    in[k]->removeTokens(1);
                }
                for (int j = 0; j < M; j++){
                    out[j]->addTokens(1);
                }
            }else{
                std::cout << this->name() << ": NOT Fired" << std::endl;
            }
        }
        
};

#endif // TRANSITION_H
