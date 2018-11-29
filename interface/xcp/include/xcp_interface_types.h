//
// Created by sauci on 26/11/2018.
//

#ifndef XCPUI_XCP_INTERFACE_TYPES_H
#define XCPUI_XCP_INTERFACE_TYPES_H

#include <vector>

#define PROGRAM_VERIFY 0xC8
#define PROGRAM_MAX 0xC9
#define PROGRAM_NEXT 0xCA
#define PROGRAM_FORMAT 0xCB
#define PROGRAM_PREPARE 0xCC
#define GET_SECTOR_INFO 0xCD
#define GET_PGM_PROCESSOR_INFO 0xCE
#define PROGRAM_RESET 0xCF
#define PROGRAM 0xD0
#define PROGRAM_CLEAR 0xD1
#define PROGRAM_START 0xD2
#define ALLOC_ODT_ENTRY 0xD3
#define ALLOC_ODT 0xD4
#define ALLOC_DAQ 0xD5
#define FREE_DAQ 0xD6
#define GET_DAQ_EVENT_INFO 0xD7
#define GET_DAQ_LIST_INFO 0xD8
#define GET_DAQ_RESOLUTION_INFO 0xD9
#define GET_DAQ_PROCESSOR_INFO 0xDA
#define READ_DAQ 0xDB
#define GET_DAQ_CLOCK 0xDC
#define START_STOP_SYNCH 0xDD
#define START_STOP_DAQ_LIST 0xDE
#define GET_DAQ_LIST_MODE 0xDF
#define SET_DAQ_LIST_MODE 0xE0
#define WRITE_DAQ 0xE1
#define SET_DAQ_PTR 0xE2
#define CLEAR_DAQ_LIST 0xE3
#define COPY_CAL_PAGE 0xE4
#define GET_SEGMENT_MODE 0xE5
#define SET_SEGMENT_MODE 0xE6
#define GET_PAGE_INFO 0xE7
#define GET_SEGMENT_INFO 0xE8
#define GET_PAG_PROCESSOR_INFO 0xE9
#define GET_CAL_PAGE 0xEA
#define SET_CAL_PAGE 0xEB
#define MODIFY_BITS 0xEC
#define SHORT_DOWNLOAD 0xED
#define DOWNLOAD_MAX 0xEE
#define DOWNLOAD_NEXT 0xEF
#define DOWNLOAD 0xF0
#define USER_CMD 0xF1
#define TRANSPORT_LAYER_CMD 0xF2
#define BUILD_CHECKSUM 0xF3
#define SHORT_UPLOAD 0xF4
#define UPLOAD 0xF5
#define SET_MTA 0xF6
#define UNLOCK 0xF7
#define GET_SEED 0xF8
#define SET_REQUEST 0xF9
#define GET_ID 0xFA
#define GET_COMM_MODE_INFO 0xFB
#define SYNCH 0xFC
#define GET_STATUS 0xFD
#define DISCONNECT_COMMAND_CODE 0xFE
#define CONNECT_COMMAND_CODE 0xFF

namespace xcp_interface_types
{
    typedef std::uint32_t baud_rate_type;
    typedef std::vector<std::uint8_t> buffer_type;
    typedef std::uint32_t hardware_channel_type;
    typedef std::uint32_t identifier_type;
    typedef std::uint32_t status_type;
    typedef enum : std::uint16_t
    {
        T1 = 1,
        T2 = 2,
        T3 = 3,
        T4 = 4,
        T5 = 5,
        T6 = 6,
        T7 = 7
    } timing_id_type;
    typedef std::uint16_t timing_type;
}

struct XCPCTORequestInterface
{
    typedef std::uint8_t COMMAND_CODE;

    virtual void operator()() = 0;
};

class XCPDTOInterface
{
protected:

    xcp_interface_types::buffer_type buffer = xcp_interface_types::buffer_type(8);
    xcp_interface_types::status_type status = 0;

public:

    const xcp_interface_types::buffer_type &get_buffer() const
    {
        return buffer;
    }

    const xcp_interface_types::status_type get_status() const
    {
        return status;
    }
};

namespace XCP
{

#ifndef XCP_DISABLE_CONNECT_COMMAND

namespace CONNECT
{
    typedef enum : std::uint8_t
    {
        NORMAL = 0,
        USER_DEFINED = 1
    } MODE;
};

#endif

#ifndef XCP_DISABLE_DISCONNECT_COMMAND

struct XCPCTODisconnect : public XCPCTORequestInterface
{
    COMMAND_CODE code = DISCONNECT;
};

#endif

#ifndef XCP_DISABLE_GET_STATUS_COMMAND

struct XCPCTOGetStatus : public XCPCTOQualifiedInterface<GET_STATUS>
{
    using XCPCTOQualifiedInterface::XCPCTOQualifiedInterface;
};

#endif

#ifndef XCP_DISABLE_SYNCH_COMMAND

struct XCPCTOSynch : public XCPCTOQualifiedInterface<SYNCH>
{
    using XCPCTOQualifiedInterface::XCPCTOQualifiedInterface;
};

#endif

#ifndef XCP_DISABLE_GET_COMM_MODE_INFO_COMMAND

struct XCPCTOGetCommModeInfo : public XCPCTOQualifiedInterface<GET_COMM_MODE_INFO>
{
};

#endif

#ifndef XCP_DISABLE_GET_ID_COMMAND

struct XCPCTOGetID : public XCPCTOQualifiedInterface<GET_ID>
{
};

#endif

#ifndef XCP_DISABLE_SET_REQUEST_COMMAND

struct XCPCTOSetRequest : public XCPCTOQualifiedInterface<SET_REQUEST>
{
};

#endif

#ifndef XCP_DISABLE_GET_SEED_COMMAND

struct XCPCTOGetSeed : public XCPCTOQualifiedInterface<GET_SEED>
{
};

#endif

#ifndef XCP_DISABLE_UNLOCK_COMMAND

struct XCPCTOUnlock : public XCPCTOQualifiedInterface<UNLOCK>
{
};

#endif

#ifndef XCP_DISABLE_SET_MTA_COMMAND

struct XCPCTOSetMTA : public XCPCTOQualifiedInterface<SET_MTA>
{
};

#endif

#ifndef XCP_DISABLE_UPLOAD_COMMAND

struct XCPCTOUpload : public XCPCTOQualifiedInterface<UPLOAD>
{
};

#endif

#ifndef XCP_DISABLE_SHORT_UPLOAD_COMMAND

struct XCPCTOShortUpload : public XCPCTOQualifiedInterface<SHORT_UPLOAD>
{
};

#endif

#ifndef XCP_DISABLE_BUILD_CHECKSUM_COMMAND

struct XCPCTOBuildChecksum : public XCPCTOQualifiedInterface<BUILD_CHECKSUM>
{
};

#endif

#ifndef XCP_DISABLE_TRANSPORT_LAYER_CMD_COMMAND

struct XCPCTOTransportLayerCMD : public XCPCTOQualifiedInterface<TRANSPORT_LAYER_CMD>
{
};

#endif

#ifndef XCP_DISABLE_USER_CMD_COMMAND

struct XCPCTOUserCMD : public XCPCTOQualifiedInterface<USER_CMD>
{
};

#endif

namespace CALIBRATION
{
    typedef std::uint32_t ADDRESS;
    typedef std::uint8_t ADDRESS_EXTENSION;
    typedef std::uint8_t *ALIGNMENT;
    typedef ALIGNMENT DATA_ELEMENTS; /* TODO: add support for multiple types. */
    typedef std::uint8_t NUMBER_OF_DATA_ELEMENTS;

#ifndef XCP_DISABLE_DOWNLOAD_COMMAND

    struct XCPCTODownload : XCPCTOQualifiedInterface<DOWNLOAD>
    {

        XCPCTODownload(NUMBER_OF_DATA_ELEMENTS number_of_data_elements,
                       ALIGNMENT alignment, DATA_ELEMENTS data_elements)
        {
            number_of_data_elements_ = number_of_data_elements;
            alignment_ = alignment;
            data_elements_ = data_elements;
        }

        NUMBER_OF_DATA_ELEMENTS number_of_data_elements_;
        ALIGNMENT alignment_;
        DATA_ELEMENTS data_elements_;
    };

#endif

#ifndef XCP_DISABLE_DOWNLOAD_NEXT_COMMAND

    struct XCPCTODownloadNext : XCPCTOQualifiedInterface<DOWNLOAD_NEXT>
    {
    };

#endif

#ifndef XCP_DISABLE_DOWNLOAD_MAX_COMMAND

    struct XCPCTODownloadMax : XCPCTOQualifiedInterface<DOWNLOAD_MAX>
    {
    };

#endif

#ifndef XCP_DISABLE_SHORT_DOWNLOAD_COMMAND

    struct XCPCTOShortDownload : XCPCTOQualifiedInterface<SHORT_DOWNLOAD>
    {
    };

#endif

#ifndef XCP_DISABLE_MODIFY_BITS_COMMAND

    struct XCPCTOModifyBits : XCPCTOQualifiedInterface<MODIFY_BITS>
    {
    };

#endif
}

namespace PAGE_SWITCHING
{
#ifndef XCP_DISABLE_SET_CAL_PAGE_COMMAND

    struct XCPCTOSetCalPage : XCPCTOQualifiedInterface<SET_CAL_PAGE>
    {
    };

#endif

#ifndef XCP_DISABLE_GET_CAL_PAGE_COMMAND

    struct XCPCTOGetCalPage : XCPCTOQualifiedInterface<GET_CAL_PAGE>
    {
    };

#endif

#ifndef XCP_DISABLE_GET_PAG_PROCESSOR_INFO_COMMAND

    struct XCPCTOGetPagProcessorInfo : XCPCTOQualifiedInterface<GET_PAG_PROCESSOR_INFO>
    {
    };

#endif

#ifndef XCP_DISABLE_GET_SEGMENT_INFO_COMMAND

    struct XCPCTOGetSegmentInfo : XCPCTOQualifiedInterface<GET_SEGMENT_INFO>
    {
    };

#endif

#ifndef XCP_DISABLE_GET_PAGE_INFO_COMMAND

    struct XCPCTOGetPageInfo : XCPCTOQualifiedInterface<GET_PAGE_INFO>
    {
    };

#endif

#ifndef XCP_DISABLE_SET_SEGMENT_MODE_COMMAND

    struct XCPCTOSetSegmentMode : XCPCTOQualifiedInterface<SET_SEGMENT_MODE>
    {
    };

#endif

#ifndef XCP_DISABLE_GET_SEGMENT_MODE_COMMAND

    struct XCPCTOGetSegmentMode : XCPCTOQualifiedInterface<GET_SEGMENT_MODE>
    {
    };

#endif

#ifndef XCP_DISABLE_COPY_CAL_PAGE_COMMAND

    struct XCPCTOCopyCalPage : XCPCTOQualifiedInterface<COPY_CAL_PAGE>
    {
    };

#endif
}

namespace DATA_ACQUISITION
{
#ifndef XCP_DISABLE_CLEAR_DAQ_LIST_COMMAND

    struct XCPCTOClearDAQList : XCPCTOQualifiedInterface<CLEAR_DAQ_LIST>
    {
    };

#endif

#ifndef XCP_DISABLE_SET_DAQ_PTR_COMMAND

    struct XCPCTOSetDAQPtr : XCPCTOQualifiedInterface<SET_DAQ_PTR>
    {
    };

#endif

#ifndef XCP_DISABLE_WRITE_DAQ_COMMAND

    struct XCPCTOWriteDAQ : XCPCTOQualifiedInterface<WRITE_DAQ>
    {
    };

#endif

#ifndef XCP_DISABLE_SET_DAQ_LIST_MODE_COMMAND

    struct XCPCTOSetDAQListMode : XCPCTOQualifiedInterface<SET_DAQ_LIST_MODE>
    {
    };

#endif

#ifndef XCP_DISABLE_GET_DAQ_LIST_MODE_COMMAND

    struct XCPCTOGetDAQListMode : XCPCTOQualifiedInterface<GET_DAQ_LIST_MODE>
    {
    };

#endif

#ifndef XCP_DISABLE_START_STOP_DAQ_LIST_COMMAND

    struct XCPCTOStartStopDAQList : XCPCTOQualifiedInterface<START_STOP_DAQ_LIST>
    {
    };

#endif

#ifndef XCP_DISABLE_START_STOP_SYNCH_COMMAND

    struct XCPCTOStartStopSynch : XCPCTOQualifiedInterface<START_STOP_SYNCH>
    {
    };

#endif

#ifndef XCP_DISABLE_GET_DAQ_CLOCK_COMMAND

    struct XCPCTOGetDAQClock : XCPCTOQualifiedInterface<GET_DAQ_CLOCK>
    {
    };

#endif

#ifndef XCP_DISABLE_READ_DAQ_COMMAND

    struct XCPCTOReadDAQ : XCPCTOQualifiedInterface<READ_DAQ>
    {
    };

#endif

#ifndef XCP_DISABLE_GET_DAQ_PROCESSOR_INFO_COMMAND

    struct XCPCTOGetDAQProcessorInfo : XCPCTOQualifiedInterface<GET_DAQ_PROCESSOR_INFO>
    {
    };

#endif

#ifndef XCP_DISABLE_GET_DAQ_RESOLUTION_INFO_COMMAND

    struct XCPCTOGetDAQResolutionInfo : XCPCTOQualifiedInterface<GET_DAQ_RESOLUTION_INFO>
    {
    };

#endif

#ifndef XCP_DISABLE_GET_DAQ_EVENT_INFO_COMMAND

    struct XCPCTOGetDAQEventInfo : XCPCTOQualifiedInterface<GET_DAQ_EVENT_INFO>
    {
    };

#endif

#ifndef XCP_DISABLE_FREE_DAQ_COMMAND

    struct XCPCTOFreeDAQ : XCPCTOQualifiedInterface<FREE_DAQ>
    {
    };

#endif

#ifndef XCP_DISABLE_ALLOC_DAQ_COMMAND

    struct XCPCTOAllocDAQ : XCPCTOQualifiedInterface<ALLOC_DAQ>
    {
    };

#endif

#ifndef XCP_DISABLE_ALLOC_ODT_COMMAND

    struct XCPCTOAllocODT : XCPCTOQualifiedInterface<ALLOC_ODT>
    {
    };

#endif

#ifndef XCP_DISABLE_ALLOC_ODT_ENTRY_COMMAND

    struct XCPCTOAllocODTEntry : XCPCTOQualifiedInterface<ALLOC_ODT_ENTRY>
    {
    };

#endif
}

namespace PROGRAMMING
{

#ifndef XCP_DISABLE_PROGRAM_COMMAND

    struct XCPCTOProgram : XCPCTOQualifiedInterface<PROGRAM>
    {
    };

#endif

#ifndef XCP_DISABLE_PROGRAM_START_COMMAND

    struct XCPCTOProgramStart : XCPCTOQualifiedInterface<PROGRAM_START>
    {
    };

#endif

#ifndef XCP_DISABLE_PROGRAM_CLEAR_COMMAND

    struct XCPCTOProgramClear : XCPCTOQualifiedInterface<PROGRAM_CLEAR>
    {
    };

#endif

#ifndef XCP_DISABLE_PROGRAM_RESET_COMMAND

    struct XCPCTOProgramReset : XCPCTOQualifiedInterface<PROGRAM_RESET>
    {
    };

#endif

#ifndef XCP_DISABLE_GET_PGM_PROCESSOR_INFO_COMMAND

    struct XCPCTOGetPgmProcessorInfo : XCPCTOQualifiedInterface<GET_PGM_PROCESSOR_INFO>
    {
    };

#endif

#ifndef XCP_DISABLE_GET_SECTOR_INFO_COMMAND

    struct XCPCTOGetSectorInfo : XCPCTOQualifiedInterface<GET_SECTOR_INFO>
    {
    };

#endif

#ifndef XCP_DISABLE_PROGRAM_FORMAT_COMMAND

    struct XCPCTOProgramFormat : XCPCTOQualifiedInterface<PROGRAM_FORMAT>
    {
    };

#endif

#ifndef XCP_DISABLE_PROGRAM_NEXT_COMMAND

    struct XCPCTOProgramNext : XCPCTOQualifiedInterface<PROGRAM_NEXT>
    {
    };

#endif

#ifndef XCP_DISABLE_PROGRAM_MAX_COMMAND

    struct XCPCTOProgramMax : XCPCTOQualifiedInterface<PROGRAM_MAX>
    {
    };

#endif

#ifndef XCP_DISABLE_PROGRAM_VERIFY_COMMAND

    struct XCPCTOProgramVerify : XCPCTOQualifiedInterface<PROGRAM_VERIFY>
    {
    };

#endif
}

#if 0
struct XCPCTOGetID : public XCPCTOQualifiedInterface<GET_ID>
{
    enum : std::uint8_t
    {
        ASCII_TEXT = 0,
        ASAM_MC2_NO_PATH = 1,
        ASAM_MC2_PATH = 2,
        ASAM_MC2_URL = 3,
        ASAM_MC2_UPLOAD = 4
    } TYPE;

    XCPCTOGetID(TYPE type) : TYPE(type)
    {}
};

struct XCPCTOSetRequest : public XCPCTOQualifiedInterface<SET_REQUEST>
{
    std::uint16_t SESSION_CONFIGURATION_ID;
    std::uint8_t MODE;

    XCPCTOSetRequest(SESSION_CONFIGURATION_ID session_configuration_id, MODE mode)
    {
        SESSION_CONFIGURATION_ID = session_configuration_id;
        MODE = mode;
    }
};

struct XCPCTOGetSeed : public XCPCTOQualifiedInterface<GET_SEED>
{
    typedef enum : std::uint8_t
    {
        FIRST_PART = 0,
        REMAINING_PART = 1
    } MODE;

    typedef enum : std::uint8_t
    {
        TRUE_ = 0,
        FALSE_ = 1
    } RESOURCE;
};

struct XCPCTOUnlock : public XCPCTOQualifiedInterface<UNLOCK>
{
    typedef std::uint8_t KEY_LENGTH;
    typedef std::uint8_t *KEY;
};

struct XCPCTOSetMTA : public XCPCTOQualifiedInterface<SET_MTA>
{
    typedef std::uint8_t ADDRESS_EXT;
    typedef std::uint32_t ADDRESS;
};

struct XCPCTOUpload : public XCPCTOQualifiedInterface<UPLOAD>
{
    typedef std::uint8_t NUM_OF_DATA;
};

struct XCPCTOShortUpload : public XCPCTOQualifiedInterface<SHORT_UPLOAD>
{
    typedef std::uint8_t NUM_OF_DATA;
    typedef std::uint8_t ADDRESS_EXT;
    typedef std::uint32_t ADDRESS;
};

struct XCPCTOBuildChecksum : public XCPCTOQualifiedInterface<BUILD_CHECKSUM>
{
    typedef std::uint32_t BLOCK_SIZE;
};

struct XCPCTOTransportLayerCmd : public XCPCTOQualifiedInterface<TRANSPORT_LAYER_CMD>
{
    typedef std::uint8_t SUB_COMMAND_CODE;
    typedef std::uint8_t *PARAMETERS;
};

struct XCPCTOUserCmd : public XCPCTOQualifiedInterface<USER_CMD>
{
    typedef std::uint8_t SUB_COMMAND_CODE;
    typedef std::uint8_t *PARAMETERS;
};

struct XCPCTODownload : public XCPCTOQualifiedInterface<DOWNLOAD>
{
    typedef std::uint8_t NUM_OF_DATA;
    typedef std::uint8_t ALIGNMENT;
    typedef union
    {
        std::uint8_t BYTE_SIZE_1[1];
        std::uint8_t BYTE_SIZE_2[2];
        std::uint8_t BYTE_SIZE_4[4];
    } *DATA;
};

struct XCPCTODownloadNext : public XCPCTOQualifiedInterface<DOWNLOAD_NEXT>
{
    typedef std::uint8_t NUM_OF_DATA;
    typedef std::uint8_t ALIGNMENT;
    typedef union
    {
        std::uint8_t BYTE_SIZE_1[1];
        std::uint8_t BYTE_SIZE_2[2];
        std::uint8_t BYTE_SIZE_4[4];
    } *DATA;
};

struct XCPCTODownloadMax : public XCPCTOQualifiedInterface<DOWNLOAD_MAX>
{
    typedef std::uint8_t ALIGNMENT;
    typedef union
    {
        std::uint8_t BYTE_SIZE_1[1];
        std::uint32_t BYTE_SIZE_3[3];
    } *DATA;
};

struct XCPCTOShortDownload : public XCPCTOQualifiedInterface<SHORT_DOWNLOAD>
{
    typedef std::uint8_t NUM_OF_DATA;
    typedef std::uint8_t ADDRESS_EXT;
    typedef std::uint32_t ADDRESS;
    typedef union
    {
        std::uint8_t BYTE_SIZE_1[1];
        std::uint8_t BYTE_SIZE_2[2];
        std::uint8_t BYTE_SIZE_4[4];
    } *DATA; /* TODO: might use template? */
};

struct XCPCTOModifyBits : public XCPCTOQualifiedInterface<MODIFY_BITS>
{
    typedef std::uint8_t SHIFT_VALUE;
    typedef std::uint16_t AND_MASK;
    typedef std::uint16_t XOR_MASK;
};

struct XCPCTOSetCalPage : public XCPCTOQualifiedInterface<SET_CAL_PAGE>
{
    typedef std::uint8_t MODE;
    typedef std::uint8_t LOGICAL_DATA_SEGMENT_NUMBER;
    typedef std::uint8_t LOGICAL_DATA_PAGE_NUMBER;
};

struct XCPCTOGetCalPage : public XCPCTOQualifiedInterface<GET_CAL_PAGE>
{
    typedef std::uint8_t ACCESS_MODE;
    typedef std::uint8_t LOGICAL_DATA_SEGMENT_NUMBER;
};

struct XCPCTOGetSegmentInfo : public XCPCTOQualifiedInterface<GET_SEGMENT_INFO>
{
    typedef std::uint8_t MODE;
    typedef std::uint8_t SEGMENT_NUMBER;
    typedef std::uint8_t SEGMENT_INFO;
    typedef std::uint8_t MAPPING_INDEX;
};

struct XCPCTOGetPageInfo : public XCPCTOQualifiedInterface<GET_PAGE_INFO>
{
    typedef std::uint8_t SEGMENT_NUMBER;
    typedef std::uint8_t PAGE_NUMBER;
};

struct XCPCTOSetSegmentMode : public XCPCTOQualifiedInterface<SET_SEGMENT_MODE>
{
    typedef std::uint8_t MODE;
    typedef std::uint8_t SEGMENT_NUMBER;
};

struct XCPCTOGetSegmentMode : public XCPCTOQualifiedInterface<GET_SEGMENT_MODE>
{
    typedef std::uint8_t SEGMENT_NUMBER;
};

struct XCPCTOCopyCalPage : public XCPCTOQualifiedInterface<COPY_CAL_PAGE>
{
    typedef std::uint8_t LOGICAL_DATA_SEGMENT_NUMBER_SOURCE;
    typedef std::uint8_t LOGICAL_DATA_PAGE_NUMBER_SOURCE;
    typedef std::uint8_t LOGICAL_DATA_SEGMENT_NUMBER_DESTINATION;
    typedef std::uint8_t LOGICAL_DATA_PAGE_NUMBER_DESTINATION;
};

struct XCPCTOClearDAQList : public XCPCTOQualifiedInterface<CLEAR_DAQ_LIST>
{
    typedef std::uint16_t DAQ_LIST_NUMBER;
};

struct XCPCTOSetDAQPtr : public XCPCTOQualifiedInterface<SET_DAQ_PTR>
{
    typedef std::uint16_t DAQ_LIST_NUMBER;
    typedef std::uint8_t ODT_NUMBER;
    typedef std::uint8_t ODT_ENTRY_NUMBER;
};

struct XCPCTOWriteDAQ : public XCPCTOQualifiedInterface<WRITE_DAQ>
{
};

struct XCPCTOSetDAQListMode : public XCPCTOQualifiedInterface<SET_DAQ_LIST_MODE>
{
};

struct XCPCTOGetDAQListMode : public XCPCTOQualifiedInterface<GET_DAQ_LIST_MODE>
{
};

struct XCPCTOStartStopDAQList : public XCPCTOQualifiedInterface<START_STOP_DAQ_LIST>
{
};

struct XCPCTOStartStopSynch : public XCPCTOQualifiedInterface<START_STOP_SYNCH>
{
};

struct XCPCTOGetDAQClock : public XCPCTOQualifiedInterface<GET_DAQ_CLOCK>
{
};

struct XCPCTOReadDAQ : public XCPCTOQualifiedInterface<READ_DAQ>
{
};

struct XCPCTOGetDAQProcessorInfo : public XCPCTOQualifiedInterface<GET_DAQ_PROCESSOR_INFO>
{
};

struct XCPCTOGetDAQResolutionInfo : public XCPCTOQualifiedInterface<GET_DAQ_RESOLUTION_INFO>
{
};

struct XCPCTOGetDAQListInfo : public XCPCTOQualifiedInterface<GET_DAQ_LIST_INFO>
{
};

struct XCPCTOGetDAQEventInfo : public XCPCTOQualifiedInterface<GET_DAQ_EVENT_INFO>
{
};

struct XCPCTOFreeDAQ : public XCPCTOQualifiedInterface<FREE_DAQ>
{
};

struct XCPCTOAllocDAQ : public XCPCTOQualifiedInterface<ALLOC_DAQ>
{
};

struct XCPCTOAllocODT : public XCPCTOQualifiedInterface<ALLOC_ODT>
{
};

struct XCPCTOAllocODTEntry : public XCPCTOQualifiedInterface<ALLOC_ODT_ENTRY>
{
};

struct XCPCTOProgramStart : public XCPCTOQualifiedInterface<PROGRAM_START>
{
};

struct XCPCTOProgramClear : public XCPCTOQualifiedInterface<PROGRAM_CLEAR>
{
};

struct XCPCTOProgram : public XCPCTOQualifiedInterface<PROGRAM>
{
};

struct XCPCTOProgramReset : public XCPCTOQualifiedInterface<PROGRAM_RESET>
{
};

struct XCPCTOGetPgmProcessorInfo : public XCPCTOQualifiedInterface<GET_PGM_PROCESSOR_INFO>
{
};

struct XCPCTOGetSectorInfo : public XCPCTOQualifiedInterface<GET_SECTOR_INFO>
{
};

struct XCPCTOProgramPrepare : public XCPCTOQualifiedInterface<PROGRAM_PREPARE>
{
};

struct XCPCTOProgramFormat : public XCPCTOQualifiedInterface<PROGRAM_FORMAT>
{
};

struct XCPCTOProgramNext : public XCPCTOQualifiedInterface<PROGRAM_NEXT>
{
};

struct XCPCTOProgramMax : public XCPCTOQualifiedInterface<PROGRAM_MAX>
{
};

struct XCPCTOProgramVerify : public XCPCTOQualifiedInterface<PROGRAM_VERIFY>
{
};
#endif

}

typedef enum : std::uint8_t
{
    CAL_PAG = 0,
    DAQ = (1 << 2),
    STIM = (1 << 3),
    PGM = (1 << 4)
} RESOURCE;

typedef std::uint8_t COMM_MODE_BASIC;
const COMM_MODE_BASIC BYTE_ORDER = 0;
const COMM_MODE_BASIC ADDRESS_GRANULARITY_0 = (1 << 1);
const COMM_MODE_BASIC ADDRESS_GRANULARITY_1 = (1 << 2);
const COMM_MODE_BASIC SLAVE_BLOCK_MODE = (1 << 6);
const COMM_MODE_BASIC OPTIONAL_ = (1 << 7); /* TODO: undefine and redefine. */

#endif //XCPUI_XCP_INTERFACE_TYPES_H
