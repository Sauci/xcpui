//
// Created by sauci on 03/12/2018.
//

#ifndef XCPUI_USB_INTERFACE_H
#define XCPUI_USB_INTERFACE_H

#include <Windows.h>

#include "usb_interface_types.h"

class USBInterface
{
public:

    /**
     * @brief returns the DLL handle.
     */
    virtual const HINSTANCE &get_dll() const = 0;

    /**
     * @brief returns the name of the interface.
     * @returns the name of the interface
     */
    virtual const usb_types::interface_name_type get_interface_name() = 0;

    /**
     * @brief returns the interface status.
     * @returns the interface status
     *     @retval true: the interface is plugged in
     *     @retval false: the interface is not plugged in
     */
    virtual const bool is_plugged_in() = 0;
};

#endif //XCPUI_INTERFACE_INTERFACE_H
