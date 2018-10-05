//
// Created by sauci on 20/11/2018.
//

#ifndef XCPUI_XCP_INTERFACE_MOCK_H
#define XCPUI_XCP_INTERFACE_MOCK_H

#include <gmock/gmock.h>

#include "xcp_interface.h"

class XCPInterfaceMock : public XCPInterface
{
    MOCK_METHOD1(set_master_identifier, void(xcp_api_types::identifier_type));

    MOCK_METHOD1(set_slave_identifier, void(xcp_api_types::identifier_type));

    MOCK_METHOD1(set_broadcast_identifier, void(xcp_api_types::identifier_type));

    MOCK_METHOD1(set_baud_rate, void(xcp_api_types::baud_rate_type));

    MOCK_METHOD1(set_hardware_channel, void(xcp_api_types::hardware_channel_type));

    MOCK_METHOD1(set_timing_parameter_t1, void(xcp_api_types::timing_parameter_type));

    MOCK_METHOD1(set_timing_parameter_t2, void(xcp_api_types::timing_parameter_type));

    MOCK_METHOD1(set_timing_parameter_t3, void(xcp_api_types::timing_parameter_type));

    MOCK_METHOD1(set_timing_parameter_t4, void(xcp_api_types::timing_parameter_type));

    MOCK_METHOD1(set_timing_parameter_t5, void(xcp_api_types::timing_parameter_type));

    MOCK_METHOD1(set_timing_parameter_t6, void(xcp_api_types::timing_parameter_type));

    MOCK_METHOD1(set_timing_parameter_t7, void(xcp_api_types::timing_parameter_type));

    MOCK_METHOD1(initialize_hardware, void(xcp_api_types::placeholder_type));

    MOCK_METHOD1(de_initialize_hardware, void(xcp_api_types::placeholder_type));

    MOCK_METHOD1(read, void(xcp_api_types::placeholder_type));

    MOCK_METHOD1(connect, void(xcp_protocol_types::connect::mode_type));
};

#endif //XCPUI_XCP_INTERFACE_MOCK_H
