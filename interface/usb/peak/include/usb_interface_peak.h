//
// Created by sauci on 03/12/2018.
//

#ifndef XCPUI_USB_INTERFACE_PEAK_H
#define XCPUI_USB_INTERFACE_PEAK_H

#include "usb_interface.h"
#include "usb_interface_peak_types.h"

class USBInterfacePEAK : public USBInterface
{
protected:

    TPCANStatus get_value(TPCANHandle channel,
                          TPCANParameter parameter,
                          void *data,
                          std::uint32_t size);

public:

    USBInterfacePEAK();

    ~USBInterfacePEAK();

    const HINSTANCE &get_dll() const override;

    const usb_types::interface_name_type get_interface_name() override;

    const bool is_plugged_in() override;
};

#endif //XCPUI_USB_INTERFACE_PEAK_H
