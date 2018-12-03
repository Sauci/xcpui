//
// Created by sauci on 30/11/2018.
//

#ifndef XCPUI_CAN_INTERFACE_PEAK_TYPES_H
#define XCPUI_CAN_INTERFACE_PEAK_TYPES_H

#include <cstdint>

typedef enum : std::uint16_t
{
    BAUD_RATE_1000000 = 0x0014u,
    BAUD_RATE_800000 = 0x0016u,
    BAUD_RATE_500000 = 0x001Cu,
    BAUD_RATE_250000 = 0x011Cu,
    BAUD_RATE_125000 = 0x031Cu,
    BAUD_RATE_100000 = 0x432Fu,
    BAUD_RATE_95000 = 0xC34Eu,
    BAUD_RATE_83000 = 0x852Bu,
    BAUD_RATE_50000 = 0x472Fu,
    BAUD_RATE_47000 = 0x1414u,
    BAUD_RATE_33000 = 0x8B2Fu,
    BAUD_RATE_20000 = 0x532Fu,
    BAUD_RATE_10000 = 0x672Fu,
    BAUD_RATE_5000 = 0x7F7Fu
} TPCANBaudrate;

typedef std::uint8_t TPCANMessageType;
typedef std::uint8_t TPCANType;

typedef struct tagTPCANMsg
{
    std::uint32_t ID;      // 11/29-bit message identifier
    TPCANMessageType MSGTYPE; // Type of the message
    std::uint8_t LEN;     // Data Length Code of the message (0..8)
    std::uint8_t DATA[8]; // Data of the message (DATA[0]..DATA[7])
} TPCANMsg;

typedef struct tagTPCANTimestamp
{
    std::uint32_t millis;             // Base-value: milliseconds: 0.. 2^32-1
    std::uint16_t millis_overflow;    // Roll-arounds of millis
    std::uint16_t micros;             // Microseconds: 0..999
} TPCANTimestamp;

#endif //XCPUI_CAN_INTERFACE_PEAK_TYPES_H
