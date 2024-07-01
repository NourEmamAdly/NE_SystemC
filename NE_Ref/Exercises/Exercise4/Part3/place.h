#ifndef PLACE_H
#define PLACE_H

#include <systemc.h>

// Place Interface:

class placeInterface : public sc_interface
{
public:
virtual void addTokens()=0;
virtual void removeTokens()=0;
virtual bool testTokens()=0;



};


// Place Channel:
template< unsigned int Win =1, unsigned int Wout = 1 >
class place :public placeInterface
{

private:

unsigned int tokens;

public:
place(unsigned int initial_tokens = 1): tokens(initial_tokens)
{
    //one way of implementation
}

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
