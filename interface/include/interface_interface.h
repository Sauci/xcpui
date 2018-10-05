//
// Created by sauci on 26/11/2018.
//

#ifndef XCPUI_INTERFACE_INTERFACE_H
#define XCPUI_INTERFACE_INTERFACE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "interface_interface_types.h"

struct InterfaceInterface
{
    /**
     * @brief returns interface type.
     * @returns the interface type
     */
    virtual const interface_types::interface_type get_interface_type() = 0;

    /**
     * @brief returns the name of the interface.
     * @returns the name of the interface
     */
    virtual const interface_types::interface_name_type get_interface_name() = 0;

    /**
     * @brief returns the number of hardware channel(s) available on the interface.
     * @returns the number of hardware channel(s)
     */
    virtual const interface_types::hardware_channel_type get_hardware_channel_count() = 0;

    /**
     * @brief returns the interface status.
     * @returns the interface status
     *     @retval true: the interface is plugged in
     *     @retval false: the interface is not plugged in
     */
    virtual const bool is_plugged_in() = 0;
};

#endif //XCPUI_INTERFACE_INTERFACE_H
