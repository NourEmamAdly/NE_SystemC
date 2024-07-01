#ifndef SIGNAL_H
#define SIGNAL_H

#include <systemc.h>

// Creating a custom signal
template <class T>
class SignalInterface : public sc_interface
{
    public:
    virtual T read() = 0;
    virtual void write(T) = 0;
};

template <class T>
class Signal : public SignalInterface<T>, public sc_prim_channel            // Derive from sc_prim_channel to be able to add signals to the sensitivity list. TC NEED TO IMPLEMENT THE PURE VIRTUAL FUNCTIONS of this channel
{
    private:
    T currentValue;
    T newValue;
    sc_event valueChangedEvent;

    public:
    Signal() {
        currentValue = 0;
        newValue = 0;
    }

    T read()
    {
        return currentValue;
    }

    void write(T d)
    {
        newValue = d;   
        if(newValue != currentValue)                                            // ONLY if they are different
        {
            request_update();                                                   // 1. Call to SystemC Scheudler
        }
    }

    void update()                                                             // 2. P.V.F of sc_prim_channel, so need to implement it
    {
        if(newValue != currentValue)
        {
            currentValue = newValue;                                         // Actual update happens here
            valueChangedEvent.notify(SC_ZERO_TIME);                         // Fire the event with SC_ZERO_TIME
        }
    }

    const sc_event& default_event() const {                                    // 3. Not a must, but should be implemented to be able to use the event in the sensitivity list
        return valueChangedEvent;
    }
};
#endif // SIGNAL_H
