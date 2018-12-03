//
// Created by sauci on 03/12/2018.
//

#ifndef XCPUI_CAN_INTERFACE_MOCK_H
#define XCPUI_CAN_INTERFACE_MOCK_H

#include <gmock/gmock.h>

#include "can_interface.h"

class CANInterfaceMock : public CANInterface
{
    MOCK_METHOD2(CANInterface, void(can_types::channel_type, can_types::baud_rate_type));

    MOCK_METHOD1(read, can_types::status_type(can_types::buffer_type & ));

    MOCK_METHOD1(write, can_types::status_type(can_types::buffer_type & ));

    MOCK_METHOD0(get_channel, can_types::channel_type());

    MOCK_METHOD0(get_baud_rate, can_types::baud_rate_type());
};

#endif //XCPUI_CAN_INTERFACE_MOCK_H
