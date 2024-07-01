#ifndef PLACE_H
#define PLACE_H

#include <systemc.h>

// Place Interface:
// TODO
//template <class T>                                                          // Templated for generalization for any datatype
class placeInterface : public sc_interface                             // is derived from the sc_interface
{
    public:
    virtual void addTokens() = 0;
    virtual void removeTokens() = 0;
    virtual bool testTokens() = 0;
};
// Place Channel:
// TODO
template<unsigned int Win = 1, unsigned int Wout = 1>                                                        // has to be templated as interface is templated
class placeChannel : public placeInterface          //<T>
{
    private:                                                                // TC
        unsigned int tokens;                                                   

    public:
        placeChannel(unsigned int initialNumberOfTokens=16) : tokens(initialNumberOfTokens)                        // 16 is a default size. The initial number of tokens should be passed as a parameter to the constructor and then set in the constructor of the channel place.
        {
        }

        void addTokens(){
            tokens += Win;
        }
        void removeTokens(){
            tokens -= Wout;
        }
        bool testTokens(){
            return tokens>=Wout;
        }


};
#endif // PLACE_H
