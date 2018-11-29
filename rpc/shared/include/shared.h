//
// Created by sauci on 23/11/2018.
//

#ifndef XCPUI_SHARED_H
#define XCPUI_SHARED_H

#include "interface_interface_types.h"
#include "xcp_interface_types.h"

#define STR(val) #val

#define GET_INTERFACE_TYPE 0
#define GET_INTERFACE_NAME 1
#define GET_HARDWARE_CHANNEL_COUNT 2
#define IS_PLUGGED_IN 3
#define GET_MASTER_IDENTIFIER 4
#define GET_SLAVE_IDENTIFIER 5
#define GET_BROADCAST_IDENTIFIER 6
#define GET_BAUD_RATE 7
#define GET_HARDWARE_CHANNEL 8
#define GET_TIMING_PARAMETER 9
#define GET_CTO 10
#define SET_MASTER_IDENTIFIER 11
#define SET_SLAVE_IDENTIFIER 12
#define SET_BROADCAST_IDENTIFIER 13
#define SET_BAUD_RATE 14
#define SET_HARDWARE_CHANNEL 15
#define SET_TIMING_PARAMETER 16
#define INITIALIZE_HARDWARE 17
#define DE_INITIALIZE_HARDWARE 18

MSGPACK_ADD_ENUM(interface_types::interface_type);
MSGPACK_ADD_ENUM(xcp_interface_types::timing_id_type);

MSGPACK_ADD_ENUM(XCP::CONNECT::MODE);

struct XCPDTOPacket
{
    bool valid;
    xcp_interface_types::status_type status;
    xcp_interface_types::buffer_type buffer;

    MSGPACK_DEFINE_ARRAY(valid, status, buffer);
};

#endif //XCPUI_SHARED_H
