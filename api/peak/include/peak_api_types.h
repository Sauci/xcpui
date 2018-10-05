//
// Created by sauci on 15/11/2018.
//

#ifndef XCPUI_PEAK_API_TYPES_H
#define XCPUI_PEAK_API_TYPES_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cstdint>

namespace peak
{
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
    } BaudRateType;

    typedef std::uint8_t *BaudRateFDType;

    typedef enum : std::uint16_t
    {
        CHANNEL_BUS_NONE = 0x00u,
        CHANNEL_BUS_USB_1 = 0x51u,
        CHANNEL_BUS_USB_2 = 0x52u,
        CHANNEL_BUS_USB_3 = 0x53u,
        CHANNEL_BUS_USB_4 = 0x54u,
        CHANNEL_BUS_USB_5 = 0x55u,
        CHANNEL_BUS_USB_6 = 0x56u,
        CHANNEL_BUS_USB_7 = 0x57u,
        CHANNEL_BUS_USB_8 = 0x58u,
        CHANNEL_BUS_USB_9 = 0x509u,
        CHANNEL_BUS_USB_10 = 0x50Au,
        CHANNEL_BUS_USB_11 = 0x50Bu,
        CHANNEL_BUS_USB_12 = 0x50Cu,
        CHANNEL_BUS_USB_13 = 0x50Du,
        CHANNEL_BUS_USB_14 = 0x50Eu,
        CHANNEL_BUS_USB_15 = 0x50Fu,
        CHANNEL_BUS_USB_16 = 0x510u
    } ChannelType;

    typedef std::uint8_t HardwareType;

    typedef enum : std::uint16_t
    {
        LANGUAGE_NEUTRAL = 0x00u,
        LANGUAGE_GERMAN = 0x07u,
        LANGUAGE_ENGLISH = 0x09u,
        LANGUAGE_SPANISH = 0x0Au,
        LANGUAGE_ITALIAN = 0x10u,
        LANGUAGE_FRENCH = 0x0Cu
    } LanguageType;

    typedef std::uint64_t TimestampFDType;

    typedef struct
    {
        /**
         * @brief base value: milliseconds: 0.. 2^32-1
         */
        std::uint32_t millisecond;

        /**
         * @brief roll-around of milliseconds
         */
        std::uint16_t millisecond_overflow;

        /**
         * @brief microseconds: 0..999
         */
        std::uint16_t microsecond;
    } TimestampType;

    typedef std::uint16_t ChannelXCPType;
    typedef std::uint32_t HandleType;

    typedef enum : std::uint32_t
    {
        CTO = 1,
        DTO = 2
    } QueueType;

    struct TransportCANType
    {
        std::uint32_t broadcast_id;
        std::uint32_t master_id;
        std::uint32_t slave_id;
        bool incremental_id_used;

        bool operator==(const TransportCANType &other) const
        {
            if ((other.broadcast_id == this->broadcast_id) &&
                (other.master_id == this->master_id) &&
                (other.slave_id == this->slave_id) &&
                (other.incremental_id_used == this->incremental_id_used))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    };

    typedef struct
    {
        std::uint16_t daq_list_number;
        std::uint16_t event_channel_number;
        std::uint8_t transmission_rate;
        std::uint8_t daq_priority;
    } DAQListConfigType;

    typedef struct
    {
        std::uint8_t compression_method;
        std::uint8_t encryption_method;
        std::uint8_t programming_method;
        std::uint8_t access_method;
    } ProgramFormatType;

    typedef struct
    {
        std::uint8_t bit_offset;
        std::uint8_t daq_size;
        std::uint8_t daq_address_extension;
        std::uint32_t daq_address;
    } ODTEntryType;

    struct ProtocolType
    {
        std::uint16_t T1;
        std::uint16_t T2;
        std::uint16_t T3;
        std::uint16_t T4;
        std::uint16_t T5;
        std::uint16_t T6;
        std::uint16_t T7;
        /* DEPRECATED: Value will automatically adjusted on connect (XCP_Connect) */
        bool motorola_format;
        /* DEPRECATED: Value will automatically adjusted on connect (XCP_Connect) */
        std::uint8_t address_granularity;

        bool operator==(const ProtocolType &other) const
        {
            if ((other.T1 == this->T1) &&
                (other.T2 == this->T2) &&
                (other.T3 == this->T3) &&
                (other.T4 == this->T4) &&
                (other.T5 == this->T5) &&
                (other.T6 == this->T6) &&
                (other.T7 == this->T7) &&
                (other.motorola_format == this->motorola_format) &&
                (other.address_granularity == this->address_granularity)
                )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    };

    typedef struct
    {
        std::uint8_t segment_number;
        std::uint8_t page_number;
    } CalibrationPageType;

    typedef enum : std::uint32_t
    {
        /**
         * @brief command processor synchronization
         */
            DLL_XCP_ERR_CMD_SYNCH = 0,

        /**
         * @brief command was not executed
         */
            DLL_XCP_ERR_CMD_BUSY = 0x10,

        /**
         * @brief command rejected because DAQ is running
         */
            DLL_XCP_ERR_DAQ_ACTIVE = 0x11,

        /**
         * @brief command rejected because program is running
         */
            DLL_XCP_ERR_PGM_ACTIVE = 0x12,

        /**
         * @brief unknown command or not implemented optional command
         */
            DLL_XCP_ERR_CMD_UNKNOWN = 0x20,

        /**
         * @brief command syntax invalid
         */
            DLL_XCP_ERR_CMD_SYNTAX = 0x21,

        /**
         * @brief command syntax valid but command parameter(s) out of range
         */
            DLL_XCP_ERR_OUT_OF_RANGE = 0x22,

        /**
         * @brief the memory location is write protected
         */
            DLL_XCP_ERR_WRITE_PROTECTED = 0x23,

        /**
         * @brief the memory location is not accessible
         */
            DLL_XCP_ERR_ACCESS_DENIED = 0x24,

        /**
         * @brief access denied, seed & key is required
         */
            DLL_XCP_ERR_ACCESS_LOCKED = 0x25,

        /**
         * @brief selected page not available
         */
            DLL_XCP_ERR_PAGE_NOT_VALID = 0x26,

        /**
         * @brief selected page mode not available
         */
            DLL_XCP_ERR_MODE_NOT_VALID = 0x27,

        /**
         * @brief selected segment not valid
         */
            DLL_XCP_ERR_SEGMENT_NOT_VALID = 0x28,

        /**
         * @brief sequence error
         */
            DLL_XCP_ERR_SEQUENCE = 0x29,

        /**
         * @brief DAQ configuration not valid
         */
            DLL_XCP_ERR_DAQ_CONFIG = 0x2A,

        /**
         * @brief memory overflow error
         */
            DLL_XCP_ERR_MEMORY_OVERFLOW = 0x30,

        /**
         * @brief generic error
         */
            DLL_XCP_ERR_GENERIC = 0x31,

        /**
         * @brief the slave internal program verify routine detects an error
         */
            DLL_XCP_ERR_VERIFY = 0x32,

        /**
         * @brief access to the requested resource is temporary not possible
         */
            DLL_XCP_ERR_RESOURCE_TEMPORARY_NOT_ACCESSIBLE = 0x33,

        /**
         * @brief acknowledge/no error
         */
            DLL_XCP_ERR_OK = (1 << 8),

        /**
         * @brief function not available/operation not implemented
         */
            DLL_XCP_ERR_NOT_IMPLEMENTED = (2 << 8),

        /**
         * @brief invalid parameter value
         */
            DLL_XCP_ERR_INVALID_PARAMETER = (3 << 8),

        /**
         * @brief the maximum amount of registered Slave channels was reached
         */
            DLL_XCP_ERR_MAX_CHANNELS = (4 << 8),

        /**
         * @brief the given handle is invalid
         */
            DLL_XCP_ERR_INVALID_HANDLE = (5 << 8),

        /**
         * @brief a timeout was reached by calling a function synchronously
         */
            DLL_XCP_ERR_INTERNAL_TIMEOUT = (6 << 8),

        /**
         * @brief the queue being referred is empty
         */
            DLL_XCP_ERR_QUEUE_EMPTY = (7 << 8),

        /**
         * @brief the size of the given buffer, is not big enough
         */
            DLL_XCP_ERR_INSUFFICIENT_BUFFER = (8 << 8),

        /**
         * @brief flag for a specific error within the underlying transport channel
         */
            DLL_XCP_ERR_TRANSPORT_CHANNEL = 0x80000000
    } StatusType;
}

#endif //XCPUI_PEAK_API_TYPES_H
