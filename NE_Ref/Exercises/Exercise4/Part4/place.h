#ifndef PLACE_H
#define PLACE_H

#include <systemc.h>
//Note : this part is correct only the printing should be 
/*
t.ACT: Fired                                                         
t.ACT:NOT Fired
t.RD: Fired 
t.WR: Fired                           
t.PRE: Fired 
t.ACT:NOT Fired       */


/*It should be as shown below only the last ACT
t.ACT: Fired                                                         
t.ACT:NOT Fired
t.RD: Fired 
t.WR: Fired                         
t.PRE: Fired 
t.ACT:NOT Fired       */

//this file contains derived interface and derived channel from this interface implementing the virtual functions of the interface in the derived channel



/****************************************************************************************************************************************
 *                Implementing Templated channel with a certain Win and Wout as input and output weight of the place channel            *
 ****************************************************************************************************************************************/
// Place Interface:

class placeInterface : public sc_interface
{
public:
//Updated the virtual functions to be templated
virtual void addTokens()=0;
virtual void removeTokens()=0;
virtual bool testTokens()=0;



};


// Deriving Place Channel from place interface but this time it would be templated
//Win denotes the input weight of the place chnnel and Wout is the output weight of the place channel
template< unsigned int Win =1, unsigned int Wout = 1 >
class place :public placeInterface
{

public:
unsigned int tokens;
//defining the place channel constructor
place(unsigned int initial_tokens = 1): tokens(initial_tokens)
{
    //one way of implementation
}

//implementing the channel functions using the input and output weight of the custom channels
void addTokens()
{
    tokens+=Win;
}

void removeTokens ()
{
    tokens-=Wout;
}

bool testTokens()
{
if (tokens >= Wout)
{
    return true;
}
else 
{
    return false;

}
}
};

#endif // PLACE_H
