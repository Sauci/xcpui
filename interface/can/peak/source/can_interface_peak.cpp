//
// Created by sauci on 30/11/2018.
//

#include "can_interface_peak.h"
#include "can_interface_exception.h"

TPCANStatus (__stdcall *c_can_initialize)(TPCANHandle,
                                          TPCANBaudrate,
                                          TPCANType,
                                          std::uint32_t,
                                          std::uint32_t);

TPCANStatus (__stdcall *c_can_uninitialize)(TPCANHandle);

TPCANStatus (__stdcall *c_can_read)(TPCANHandle, TPCANMsg *, TPCANTimestamp *);

TPCANStatus (__stdcall *c_can_write)(TPCANHandle, TPCANMsg *);

CANInterfacePEAK::CANInterfacePEAK(can_types::channel_type channel,
                                   can_types::baud_rate_type baud_rate) :
    USBInterfacePEAK()
{
    c_can_initialize = (decltype(c_can_initialize))GetProcAddress(USBInterfacePEAK::get_dll(),
                                                                  "CAN_Initialize");
    c_can_uninitialize = (decltype(c_can_uninitialize))GetProcAddress(USBInterfacePEAK::get_dll(),
                                                                      "CAN_Uninitialize");
    c_can_read = (decltype(c_can_read))GetProcAddress(USBInterfacePEAK::get_dll(), "CAN_Read");
    c_can_write = (decltype(c_can_write))GetProcAddress(USBInterfacePEAK::get_dll(), "CAN_Write");

    channel_ = get_channel_internal(channel);
    c_can_initialize(channel_, get_baud_rate_internal(baud_rate), 0, 0, 0);
}

CANInterfacePEAK::~CANInterfacePEAK()
{
    (void)c_can_uninitialize(channel_);
    c_can_initialize = nullptr;
    c_can_uninitialize = nullptr;
    c_can_read = nullptr;
    c_can_write = nullptr;
}

can_types::status_type CANInterfacePEAK::read(can_types::buffer_type &buffer)
{
    TPCANMsg message;
    TPCANTimestamp timestamp;
    return c_can_read(channel_, &message, &timestamp);
}

can_types::status_type CANInterfacePEAK::write(can_types::buffer_type &buffer)
{
    TPCANMsg message;
    return c_can_write(channel_, &message);
}

const can_types::channel_type CANInterfacePEAK::get_channel()
{
    switch (channel_)
    {
        case TPCANHandle::CHANNEL_BUS_USB_1:
            return 1;
        case TPCANHandle::CHANNEL_BUS_USB_2:
            return 2;
        case TPCANHandle::CHANNEL_BUS_USB_3:
            return 3;
        case TPCANHandle::CHANNEL_BUS_USB_4:
            return 4;
        case TPCANHandle::CHANNEL_BUS_USB_5:
            return 5;
        case TPCANHandle::CHANNEL_BUS_USB_6:
            return 6;
        case TPCANHandle::CHANNEL_BUS_USB_7:
            return 7;
        case TPCANHandle::CHANNEL_BUS_USB_8:
            return 8;
        case TPCANHandle::CHANNEL_BUS_USB_9:
            return 9;
        case TPCANHandle::CHANNEL_BUS_USB_10:
            return 10;
        case TPCANHandle::CHANNEL_BUS_USB_11:
            return 11;
        case TPCANHandle::CHANNEL_BUS_USB_12:
            return 12;
        case TPCANHandle::CHANNEL_BUS_USB_13:
            return 13;
        case TPCANHandle::CHANNEL_BUS_USB_14:
            return 14;
        case TPCANHandle::CHANNEL_BUS_USB_15:
            return 15;
        case TPCANHandle::CHANNEL_BUS_USB_16:
            return 16;
        default:
            throw invalid_hardware_channel_error(channel_);
    }
}

const can_types::baud_rate_type CANInterfacePEAK::get_baud_rate()
{
    std::uint16_t btr01 = 0;

    auto status = USBInterfacePEAK::get_value(channel_,
                                              TPCANParameter::PCAN_BITRATE_INFO,
                                              &btr01,
                                              sizeof(btr01));
    if (status == TPCANStatus::PCAN_ERROR_OK)
    {
        switch (btr01)
        {
            case 20:
                return 1000000;
            case 22:
                return 800000;
            case 28:
                return 500000;
            case 284:
                return 250000;
            case 796:
                return 125000;
            case 17199:
                return 100000;
            case 49998:
                return 95000;
            case 34091:
                return 83000;
            case 18223:
                return 50000;
            case 5140:
                return 47000;
            case 35631:
                return 33000;
            case 21295:
                return 20000;
            case 26415:
                return 10000;
            case 32639:
                return 5000;
            default:
                throw invalid_baud_rate_error(btr01);
        }
    }
    else
    {
        throw invalid_baud_rate_error(btr01);
    }
};

const can_types::channel_type CANInterfacePEAK::get_channel_count()
{
    return 2;
}

TPCANHandle CANInterfacePEAK::get_channel_internal(can_types::channel_type channel)
{
    switch (channel)
    {
        case 1:
            return TPCANHandle::CHANNEL_BUS_USB_1;
        case 2:
            return TPCANHandle::CHANNEL_BUS_USB_2;
        case 3:
            return TPCANHandle::CHANNEL_BUS_USB_3;
        case 4:
            return TPCANHandle::CHANNEL_BUS_USB_4;
        case 5:
            return TPCANHandle::CHANNEL_BUS_USB_5;
        case 6:
            return TPCANHandle::CHANNEL_BUS_USB_6;
        case 7:
            return TPCANHandle::CHANNEL_BUS_USB_7;
        case 8:
            return TPCANHandle::CHANNEL_BUS_USB_8;
        case 9:
            return TPCANHandle::CHANNEL_BUS_USB_9;
        case 10:
            return TPCANHandle::CHANNEL_BUS_USB_10;
        case 11:
            return TPCANHandle::CHANNEL_BUS_USB_11;
        case 12:
            return TPCANHandle::CHANNEL_BUS_USB_12;
        case 13:
            return TPCANHandle::CHANNEL_BUS_USB_13;
        case 14:
            return TPCANHandle::CHANNEL_BUS_USB_14;
        case 15:
            return TPCANHandle::CHANNEL_BUS_USB_15;
        case 16:
            return TPCANHandle::CHANNEL_BUS_USB_16;
        default:
            throw invalid_hardware_channel_error(channel);
    }
}

TPCANBaudrate CANInterfacePEAK::get_baud_rate_internal(can_types::baud_rate_type baud_rate)
{
    switch (baud_rate)
    {
        case 1000000:
            return TPCANBaudrate::BAUD_RATE_1000000;
        case 800000:
            return TPCANBaudrate::BAUD_RATE_800000;
        case 500000:
            return TPCANBaudrate::BAUD_RATE_500000;
        case 250000:
            return TPCANBaudrate::BAUD_RATE_250000;
        case 125000:
            return TPCANBaudrate::BAUD_RATE_125000;
        case 100000:
            return TPCANBaudrate::BAUD_RATE_100000;
        case 95000:
            return TPCANBaudrate::BAUD_RATE_95000;
        case 83000:
            return TPCANBaudrate::BAUD_RATE_83000;
        case 50000:
            return TPCANBaudrate::BAUD_RATE_50000;
        case 47000:
            return TPCANBaudrate::BAUD_RATE_47000;
        case 33000:
            return TPCANBaudrate::BAUD_RATE_33000;
        case 20000:
            return TPCANBaudrate::BAUD_RATE_20000;
        case 10000:
            return TPCANBaudrate::BAUD_RATE_10000;
        case 5000:
            return TPCANBaudrate::BAUD_RATE_5000;
        default:
            throw invalid_baud_rate_error(baud_rate);
    }
}
