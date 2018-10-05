//
// Created by sauci on 26/11/2018.
//

#ifndef XCPUI_INTERFACE_INTERFACE_TYPES_H
#define XCPUI_INTERFACE_INTERFACE_TYPES_H

#include <cstdint>
#include <string>

namespace interface_types
{
    typedef enum
    {
        XCP = 1000
    } interface_type;

    typedef std::string interface_name_type;

    typedef std::uint32_t hardware_channel_type;
}

#endif //XCPUI_INTERFACE_INTERFACE_TYPES_H
