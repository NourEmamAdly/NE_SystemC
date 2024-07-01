#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <systemc.h>

// TODO

SC_MODULE(stateMachine){
    public:
    sc_in<char> input;
    sc_in<bool> clk;
    enum base {Start, G, GA, GAA, GAAG};
    sc_signal<base> currentState;
    unsigned int app = 0;
    unsigned int cnt = 0;

    SC_CTOR(stateMachine){
        SC_METHOD(process);
        sensitive << clk.pos();
        dont_initialize();      // process should not be called during the initialization phase
    }

    private:
    void process(){
        char in_value = input.read();
        cnt++;
        switch (currentState)
        {
        case Start: if(in_value == 'G')
                    { 
                        currentState = G;
                    }else{
                        currentState = Start;
                    }
            break;
        case G:     if(in_value == 'G')
                    { 
                        currentState = G;
                    }else if (in_value == 'A'){
                        currentState = GA;
                    }else{
                        currentState = Start;
                    }
            break;
        case GA:    if(in_value == 'G')
                    { 
                        currentState = G;
                    }else if (in_value == 'A'){
                        currentState = GAA;
                    }else{
                        currentState = Start;
                    }
            break;
        case GAA:   if(in_value == 'G')
                    { 
                        currentState = GAAG;
                        app++;
                        std::cout << "GAAG appearance number " << app << " after " << cnt << " letters." << std::endl;
                    }else{
                        currentState = Start;
                    }
            break;
        case GAAG:  if(in_value == 'G')
                    { 
                        currentState = G;
                    }else{
                        currentState = Start;
                    }
            break;
        
        default:
            break;
        }
    }
    
};

#endif // STATE_MACHINE_H
