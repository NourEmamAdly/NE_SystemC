#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <systemc.h>

// TODO
SC_MODULE(stateMachine)
{

 public:
//defining inputs
sc_in<char> input;
sc_in<bool> clk;

//defining enumeration for internal states
enum base {Start, G, GA,GAA, GAAG};

sc_signal<base> currentState;

unsigned int app=0;
unsigned int cnt=0;
SC_CTOR(stateMachine)
{

SC_METHOD(process);
sensitive <<clk.pos();
dont_initialize(); //Process shouldnt be called during the initialization phase
currentState.write(Start); // Initialize the state
}

 private:
void process()
{
   
    char in_value = input.read();
    cnt++;


switch(currentState.read())
{

case Start:
if(in_value == 'G')
{
    currentState.write(G);

}
else
{
    currentState.write(Start);
}
break;

case G:
if(in_value == 'G')
{
    currentState.write(G);

}
else if(in_value == 'A')
{
    currentState.write(GA);
}
else{
    currentState.write(Start);
}
break;

case GA:
if(in_value == 'G')
{
   
    currentState.write(G);

}
else if(in_value == 'A')
{
    currentState.write(GAA);
}
else{
    currentState.write(Start);
}
break;


case GAA:
if(in_value == 'G')
{
    currentState.write(GAAG);
    app++;
    std::cout << "GAAG appearance number"<< app<<"after"<<cnt<<"letters."<<std::endl;

}
else
{
    currentState.write(Start);
}
break;

case GAAG:
if(in_value == 'G')
{
    currentState.write(G);


}
else
{
    currentState.write(Start);
}
break;

default:
break;
}

}


};

#endif // STATE_MACHINE_H


