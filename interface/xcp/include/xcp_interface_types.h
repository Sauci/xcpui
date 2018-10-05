//
// Created by sauci on 26/11/2018.
//

#ifndef XCPUI_XCP_INTERFACE_TYPES_H
#define XCPUI_XCP_INTERFACE_TYPES_H

#include <vector>

#include "xcp_api_types.h"

namespace xcp_interface_type
{
    struct XCPMessageInterface
    {
        std::vector<uint8_t> buffer_ = std::vector<uint8_t>(8);

        xcp_api_types::status_type status_ = 0;

        virtual void operator()() = 0;

        const std::vector<uint8_t> &get_buffer() const
        {
            return buffer_;
        }

        const xcp_api_types::status_type &get_status() const
        {
            return status_;
        }
    };

    struct XCPCTOMessageInterface : public XCPMessageInterface
    {
        XCPCTOMessageInterface() : XCPMessageInterface()
        {
        }
    };

    typedef std::vector<std::uint8_t> buffer_type;
    typedef std::uint32_t hardware_channel_type;
    typedef xcp_api_types::baud_rate_type baud_rate_type;
    typedef xcp_api_types::identifier_type identifier_type;
    typedef xcp_api_types::message_type message_type;
    typedef xcp_api_types::status_type status_type;
    typedef xcp_api_types::timing_parameter_id_type timing_parameter_id_type;
    typedef xcp_api_types::timing_parameter_type timing_parameter_type;
}

#endif //XCPUI_XCP_INTERFACE_TYPES_H
