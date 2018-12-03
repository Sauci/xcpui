//
// Created by sauci on 26/11/2018.
//

#ifndef XCPUI_CAN_INTERFACE_TYPES_H
#define XCPUI_CAN_INTERFACE_TYPES_H

#include <cstdint>
#include <string>
#include <vector>

namespace can_types
{
    typedef enum
    {
        CAN = 1000
    } interface_type;

    typedef std::string interface_name_type;
    typedef std::uint32_t baud_rate_type;
    typedef std::uint32_t channel_type;
    typedef std::uint32_t status_type;
    typedef std::vector<std::uint8_t> buffer_type;
}

#endif //XCPUI_CAN_INTERFACE_TYPES_H
