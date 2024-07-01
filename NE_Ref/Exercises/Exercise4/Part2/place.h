#ifndef PLACE_H
#define PLACE_H

#include <systemc.h>

// Place Interface:
/****************************************************************************************************************************************
 *                        Implementing custom Place Interface and Place Channel                                                         *
 ****************************************************************************************************************************************/
// Deriving the place interface from the SC Interface standrd
class placeInterface : public sc_interface  // Creating Place Custom Interface
{

//defining the pure virtual functions that will be implemented in the drived channel
public:
virtual void addTokens(unsigned int n)=0;
virtual void removeTokens(unsigned int n)=0;
virtual unsigned int testTokens()=0;

};


// Deriving a channel called place from the place Interface that we created up
class place :public placeInterface
{

private:
unsigned int tokens;

public:
//defining the place channel constructor
place(unsigned int initial_tokens = 1): tokens(initial_tokens)
{
    //one way of implementation
}

                                    // Implementing the pure virtual functions mentioned in the Interface
 
//Here the function is adding n tokens to the current value of the tokens
void addTokens(unsigned int n)
{
    tokens+=n;
}

//Here the function is subtracting n tokens to the current value of the tokens
void removeTokens (unsigned int n)
{
    tokens-=n;
}

//Here the function is returning the current value of the tokens we have
unsigned int testTokens()
{

    return tokens;
}
};

#endif // PLACE_H
