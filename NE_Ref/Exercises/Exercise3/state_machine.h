#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <systemc.h>
/****************************************************************************************************************************************
 *                        Implementing State Machine Module to count the occurance of GAAG Output                                       *
 ****************************************************************************************************************************************/
// TODO
SC_MODULE(stateMachine)
{

 public:
//defining input port of the state machine
sc_in<char> input;
//defining a clck as input to the state machine
sc_in<bool> clk;

//defining enumeration used for defining the states of the state machine
enum base {Start, G, GA,GAA, GAAG};
 
// Define standard SC Signal that shows the current state value inside the state machine
sc_signal<base> currentState;

unsigned int app=0;
unsigned int cnt=0;
 //defining the state machine constructor
SC_CTOR(stateMachine)
{
//defining a process that is sensitive to every positive clck edge
SC_METHOD(process);
sensitive <<clk.pos();
dont_initialize(); //Process shouldnt be called during the initialization phase
currentState.write(Start); // Initialize the state with start ,the beggining of the state machine
}

 //implementing the process
 private:
void process()
{
   //check the input port value to the state machine states 
    char in_value = input.read();
    cnt++;

//check the current state value
switch(currentState.read())
{

 // if the current state = START 
case Start:
 //Check the input port value and if its G so then the current state will be G (check Figure 1)
if(in_value == 'G')
{
    currentState.write(G);

}
 //Other wise then the current state is START  (check Figure 1)
else
{
    currentState.write(Start);
}
break;
 
 // if the current state = G 
case G:
  //Check the input port value and if its G so then the current state will be G (check Figure 1)
if(in_value == 'G')
{
    currentState.write(G);

}
  //Check the input port value and if its A so then the current state will be GA (check Figure 1)
else if(in_value == 'A')
{
    currentState.write(GA);
}
 //Other wise then the current state is START  (check Figure 1)
else{
    currentState.write(Start);
}
break;
// if the current state = GA 
case GA:
 //Check the input port value and if its G so then the current state will be G (check Figure 1)
if(in_value == 'G')
{
   
    currentState.write(G);

}
   //Check the input port value and if its A so then the current state will be GAA (check Figure 1)
else if(in_value == 'A')
{
    currentState.write(GAA);
}
  //Other wise then the current state is START  (check Figure 1)
else{
    currentState.write(Start);
}
break;

 // Incase if the current state = G 
case GAA:
 //Check the input port value and if its A so then the current state will be GAAG (check Figure 1)
if(in_value == 'G')
{
    currentState.write(GAAG);
 //count the occurance of GAAG by one
    app++;
    std::cout << "GAAG appearance number"<< app<<"after"<<cnt<<"letters."<<std::endl;

}
  //Other wise then the current state is START  (check Figure 1)
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


