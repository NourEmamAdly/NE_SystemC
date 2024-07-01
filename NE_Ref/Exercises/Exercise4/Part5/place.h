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



// Place Interface:
/***********************************************************************************************
*                        DEriving Place Interface from the SC_Interface                        *
************************************************************************************************/
class placeInterface : public sc_interface
{
public:
//Implementing Virtual functions that will be implemented in the derived place channel
virtual void addTokens()=0;
virtual void removeTokens()=0;
virtual bool testTokens()=0;



};


// Place Channel:
/***********************************************************************************************
*                       DEriving Place Channel from the Place Interface                        *
************************************************************************************************/
//defining templated place channel to Input and output
template< unsigned int Win =1, unsigned int Wout = 1 >
class place :public placeInterface
{

private:

unsigned int tokens;

//defining constructor for the derived place channel class
public:
place(unsigned int initial_tokens = 16): tokens(initial_tokens)
{
    //one way of implementation
}
//implementing the virtual functions 
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
    return (true);
}
else 
{
    return false;

}

}



};

#endif // PLACE_H
