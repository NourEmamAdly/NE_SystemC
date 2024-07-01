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


// Deriving Place Channel from place interface but this time it would be templated
//Win denotes the input weight of the place chnnel and Wout is the output weight of the place channel
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

//implementing the channel functions using the input and output weight of the channels
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
