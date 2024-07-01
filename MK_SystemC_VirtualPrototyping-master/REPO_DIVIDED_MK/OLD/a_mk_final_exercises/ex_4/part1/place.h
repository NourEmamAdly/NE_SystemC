#ifndef PLACE_H
#define PLACE_H

#include <systemc.h>

// Place Interface:
// TODO
//template <class T>                                                          // Templated for generalization for any datatype
class placeInterface : public sc_interface                             // is derived from the sc_interface
{
    public:
    virtual void addTokens(unsigned int n) = 0;
    virtual void removeTokens(unsigned int n) = 0;
    virtual unsigned int testTokens() = 0;
};
// Place Channel:
// TODO
//template <class T>                                                          // has to be templated as interface is templated
class placeChannel : public placeInterface          //<T>
{
    private:                                                                // TC
        unsigned int tokens;                                                   

    public:
        placeChannel(unsigned int initialNumberOfTokens=16) : tokens(initialNumberOfTokens)                        // 16 is a default size. The initial number of tokens should be passed as a parameter to the constructor and then set in the constructor of the channel place.
        {
        }

        void addTokens(unsigned int n){
            tokens = tokens + n;
        }
        void removeTokens(unsigned int n){
            tokens = tokens - n;
        }
        unsigned int testTokens(){
            return tokens;
        }


};
#endif // PLACE_H
