//
// Created by sauci on 30/11/2018.
//

#ifndef XCPUI_CAN_INTERFACE_PEAK_H
#define XCPUI_CAN_INTERFACE_PEAK_H

#include "can_interface.h"
#include "can_interface_peak_types.h"
#include "usb_interface_peak.h"
#include "usb_interface_peak_types.h"

class CANInterfacePEAK : public USBInterfacePEAK, public CANInterface
{
    TPCANHandle channel_;

    TPCANBaudrate get_baud_rate_internal(can_types::baud_rate_type baud_rate);

    TPCANHandle get_channel_internal(can_types::channel_type channel);

public:

    CANInterfacePEAK(can_types::channel_type channel,
                     can_types::baud_rate_type baud_rate);

    ~CANInterfacePEAK();

    can_types::status_type read(can_types::buffer_type &buffer) override;

    can_types::status_type write(can_types::buffer_type &buffer) override;

    const can_types::channel_type get_channel() override;

    const can_types::baud_rate_type get_baud_rate() override;

    const can_types::channel_type get_channel_count() override;
};

#endif //XCPUI_CAN_INTERFACE_PEAK_H
