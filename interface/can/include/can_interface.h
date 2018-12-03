//
// Created by sauci on 26/11/2018.
//

#ifndef XCPUI_CAN_INTERFACE_H
#define XCPUI_CAN_INTERFACE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "can_interface_types.h"

class CANInterface
{
public:

    /**
     * @brief reads data from CAN.
     * @param buffer the buffer containing data to read
     */
    virtual can_types::status_type read(can_types::buffer_type &buffer) = 0;

    /**
     * @brief writes data to CAN.
     * @param buffer the buffer containing data to write
     */
    virtual can_types::status_type write(can_types::buffer_type &buffer) = 0;

    /**
     * @brief returns the hardware channel.
     * @returns hardware channel value [-]
     */
    virtual const can_types::channel_type get_channel() = 0;

    /**
     * @brief returns the baud rate value.
     * @returns baud rate value [baud/s]
     */
    virtual const can_types::baud_rate_type get_baud_rate() = 0;

    /**
     * @brief returns the number of hardware channel(s) available on the interface.
     * @returns the number of hardware channel(s)
     */
    virtual const can_types::channel_type get_channel_count() = 0; /* TODO: return an other type. */
};

#endif //XCPUI_CAN_INTERFACE_H
