#ifndef SUBNET_H
#define SUBNET_H

#include <systemc.h>

#include "place.h"
#include "transition.h"

// Subnet:
// TODO


SC_MODULE(subnet)
{
    public:
        transition<1,1,1> ACT;
        transition<1,1,0> RD;
        transition<1,1,0> PRE;
        transition<1,1,0> WR;
        place<3,3> ACTIVE;
        SC_CTOR(subnet): ACT("ACT"), RD("RD"), PRE("PRE"), WR("WR"), ACTIVE(0)
        {
            //ACT.in.bind(IDLE);
            ACT.inhibitors.bind(ACTIVE);                                                             // WHATTTTTT???????
            ACT.out.bind(ACTIVE);
            
            RD.in.bind(ACTIVE);
            RD.out.bind(ACTIVE);
            
            PRE.in.bind(ACTIVE);
            //PRE.out.bind(IDLE);

            WR.in.bind(ACTIVE);
            WR.out.bind(ACTIVE);
            //SC_THREAD(process);

        }
};

#endif // SUBNET_H
