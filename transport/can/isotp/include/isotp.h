//
// Created by sauci on 03/12/2018.
//

#ifndef XCPUI_ISOTP_H
#define XCPUI_ISOTP_H

#include "can_interface.h"

class ISOTP
{
    CANInterface &can_interface_;

public:

    ISOTP(CANInterface &can_interface) : can_interface_(can_interface)
    {
    }

    const CANInterface &get_can_interface() const
    {
        return can_interface_;
    }
};

#endif //XCPUI_ISOTP_H
