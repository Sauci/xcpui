//
// Created by sauci on 15/11/2018.
//

#ifndef XCPUI_XCP_INTERFACE_H
#define XCPUI_XCP_INTERFACE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <algorithm>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

#include "xcp_interface_types.h"
#include "xcp_interface_exception.h"

using namespace xcp_interface_types;

/**
 * @brief interface API defining a common behavior for all XCP devices.
 *
 * this API should be used as follow:
 * - use the set_* methods to configure the communication and protocol parameters, such as the
 *   communication channel, the baud rate the different CAN identifiers and the timeout parameters.
 * - use the initialize_hardware method to initialize the interfaced hardware with the parameters
 *   defined previously.
 * - use the XCP related methods such as connect as required by the application.
 */
struct XCPInterface
{
    /**
     * @brief returns the master CAN identifier.
     * @returns master CAN identifier
     */
    virtual const identifier_type get_master_identifier() = 0;

    /**
     * @brief returns the slave CAN identifier.
     * @returns slave CAN identifier
     */
    virtual const identifier_type get_slave_identifier() = 0;

    /**
     * @brief returns the broadcast CAN identifier.
     * @returns broadcast CAN identifier
     */
    virtual const identifier_type get_broadcast_identifier() = 0;

    /**
     * @brief returns the timing parameter value.
     * @param timing_parameter_id the identifier of the timing parameter (T1 .. T7)
     * @returns timing parameter value [ms]
     */
    virtual const timing_type get_timing_parameter(timing_id_type timing_parameter_id) = 0;

    /**
     * @brief dequeues a single DTO (data transfer object) from the internal DTO queue.
     * @warning the user is responsible of de-allocating the returned object.
     * @returns an object responding to XCPDTOInterface interface
     *     @retval nullptr no DTO available in tue queue
     *     @retval XCPDTOInterface * pointer to the older allocated object
     */
    virtual XCPDTOInterface *dequeue_dto() = 0;

    /**
     * @brief sets the master CAN identifier.
     * @param identifier master CAN identifier
     * @throw invalid_identifier_error if the requested CAN identifier is invalid
     */
    virtual void set_master_identifier(identifier_type identifier) = 0;

    /**
     * @brief sets the slave CAN identifier.
     * @param identifier slave CAN identifier
     * @throw invalid_identifier_error if the requested CAN identifier is invalid
     */
    virtual void set_slave_identifier(identifier_type identifier) = 0;

    /**
     * @brief sets the broadcast CAN identifier.
     * @param identifier broadcast CAN identifier
     * @throw invalid_identifier_error if the requested CAN identifier is invalid
     */
    virtual void set_broadcast_identifier(identifier_type identifier) = 0;

    /**
     * @brief sets the communication timeout parameter.
     * @param timing_id_type identifier if the timing parameter [T1 .. T7]
     * @param timing_type communication timeout parameter
     */
    virtual void set_timing_parameter(timing_id_type timing_id, timing_type timing) = 0;

    /**
     * @brief initializes the hardware.
     * @param placeholder placeholder parameter, not used
     * @throw invalid_command_error if any initialization command does not executes as expected
     *
     * @note this function shall be called once all communication and protocol parameters are set.
     */
    virtual void initialize_hardware() = 0;

    /**
     * @brief de-initializes the hardware.
     * @param wait_for_completion if true, this method will only return once all queued commands
     * will be executed. if false, this method will discard the queued commands and return
     * immediately.
     * @throw invalid_command_error if any de-initialization command does not executes as expected
     *
     * @note this function shall be called once initialize_hardware has been called.
     */
    virtual void de_initialize_hardware(bool wait_for_completion) = 0;

    virtual bool cto_completion_handler(XCPCTORequestInterface *message) = 0;

#ifndef XCP_DISABLE_PROGRAM_VERIFY_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'PROGRAM_VERIFY', identified with command code 0xC8.'
     */
    virtual void program_verify(XCPCTOProgramVerify &cto) = 0;

#endif /* #ifndef XCP_DISABLE_PROGRAM_VERIFY_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_MAX_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'PROGRAM_MAX', identified with command code 0xC9.'
     */
    virtual void program_max(XCPCTOProgramMax &cto) = 0;

#endif /* #ifndef XCP_DISABLE_PROGRAM_MAX_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_NEXT_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'PROGRAM_NEXT', identified with command code 0xCA.'
     */
    virtual void program_next(XCPCTOProgramNext &cto) = 0;

#endif /* #ifndef XCP_DISABLE_PROGRAM_NEXT_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_FORMAT_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'PROGRAM_FORMAT', identified with command code 0xCB.'
     */
    virtual void program_format(XCPCTOProgramFormat &cto) = 0;

#endif /* #ifndef XCP_DISABLE_PROGRAM_FORMAT_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_PREPARE_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'PROGRAM_PREPARE', identified with command code 0xCC.'
     */
    virtual void program_prepare(XCPCTOProgramPrepare &cto) = 0;

#endif /* #ifndef XCP_DISABLE_PROGRAM_PREPARE_COMMAND */

#ifndef XCP_DISABLE_GET_SECTOR_INFO_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_SECTOR_INFO', identified with command code 0xCD.'
     */
    virtual void get_sector_info(XCPCTOGetSectorInfo &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_SECTOR_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_PGM_PROCESSOR_INFO_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_PGM_PROCESSOR_INFO', identified with command code 0xCE.'
     */
    virtual void get_pgm_processor_info(XCPCTOGetPgmProcessorInfo &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_PGM_PROCESSOR_INFO_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_RESET_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'PROGRAM_RESET', identified with command code 0xCF.'
     */
    virtual void program_reset(XCPCTOProgramReset &cto) = 0;

#endif /* #ifndef XCP_DISABLE_PROGRAM_RESET_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'PROGRAM', identified with command code 0xD0.'
     */
    virtual void program(XCPCTOProgram &cto) = 0;

#endif /* #ifndef XCP_DISABLE_PROGRAM_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_CLEAR_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'PROGRAM_CLEAR', identified with command code 0xD1.'
     */
    virtual void program_clear(XCPCTOProgramClear &cto) = 0;

#endif /* #ifndef XCP_DISABLE_PROGRAM_CLEAR_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_START_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'PROGRAM_START', identified with command code 0xD2.'
     */
    virtual void program_start(XCPCTOProgramStart &cto) = 0;

#endif /* #ifndef XCP_DISABLE_PROGRAM_START_COMMAND */

#ifndef XCP_DISABLE_ALLOC_ODT_ENTRY_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'ALLOC_ODT_ENTRY', identified with command code 0xD3.'
     */
    virtual void alloc_odt_entry(XCPCTOAllocEntry &cto) = 0;

#endif /* #ifndef XCP_DISABLE_ALLOC_ODT_ENTRY_COMMAND */

#ifndef XCP_DISABLE_ALLOC_ODT_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'ALLOC_ODT', identified with command code 0xD4.'
     */
    virtual void alloc_odt(XCPCTOAlloc &cto) = 0;

#endif /* #ifndef XCP_DISABLE_ALLOC_ODT_COMMAND */

#ifndef XCP_DISABLE_ALLOC_DAQ_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'ALLOC_DAQ', identified with command code 0xD5.'
     */
    virtual void alloc_daq(XCPCTOAlloc &cto) = 0;

#endif /* #ifndef XCP_DISABLE_ALLOC_DAQ_COMMAND */

#ifndef XCP_DISABLE_FREE_DAQ_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'FREE_DAQ', identified with command code 0xD6.'
     */
    virtual void free_daq(XCPCTOFree &cto) = 0;

#endif /* #ifndef XCP_DISABLE_FREE_DAQ_COMMAND */

#ifndef XCP_DISABLE_GET_DAQ_EVENT_INFO_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_DAQ_EVENT_INFO', identified with command code 0xD7.'
     */
    virtual void get_daq_event_info(XCPCTOGetEventInfo &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_DAQ_EVENT_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_DAQ_LIST_INFO_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_DAQ_LIST_INFO', identified with command code 0xD8.'
     */
    virtual void get_daq_list_info(XCPCTOGetListInfo &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_DAQ_LIST_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_DAQ_RESOLUTION_INFO_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_DAQ_RESOLUTION_INFO', identified with command code 0xD9.'
     */
    virtual void get_daq_resolution_info(XCPCTOGetResolutionInfo &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_DAQ_RESOLUTION_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_DAQ_PROCESSOR_INFO_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_DAQ_PROCESSOR_INFO', identified with command code 0xDA.'
     */
    virtual void get_daq_processor_info(XCPCTOGetProcessorInfo &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_DAQ_PROCESSOR_INFO_COMMAND */

#ifndef XCP_DISABLE_READ_DAQ_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'READ_DAQ', identified with command code 0xDB.'
     */
    virtual void read_daq(XCPCTORead &cto) = 0;

#endif /* #ifndef XCP_DISABLE_READ_DAQ_COMMAND */

#ifndef XCP_DISABLE_GET_DAQ_CLOCK_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_DAQ_CLOCK', identified with command code 0xDC.'
     */
    virtual void get_daq_clock(XCPCTOGetClock &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_DAQ_CLOCK_COMMAND */

#ifndef XCP_DISABLE_START_STOP_SYNCH_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'START_STOP_SYNCH', identified with command code 0xDD.'
     */
    virtual void start_stop_synch(XCPCTOStartStopSynch &cto) = 0;

#endif /* #ifndef XCP_DISABLE_START_STOP_SYNCH_COMMAND */

#ifndef XCP_DISABLE_START_STOP_DAQ_LIST_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'START_STOP_DAQ_LIST', identified with command code 0xDE.'
     */
    virtual void start_stop_daq_list(XCPCTOStartStopList &cto) = 0;

#endif /* #ifndef XCP_DISABLE_START_STOP_DAQ_LIST_COMMAND */

#ifndef XCP_DISABLE_GET_DAQ_LIST_MODE_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_DAQ_LIST_MODE', identified with command code 0xDF.'
     */
    virtual void get_daq_list_mode(XCPCTOGetListMode &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_DAQ_LIST_MODE_COMMAND */

#ifndef XCP_DISABLE_SET_DAQ_LIST_MODE_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'SET_DAQ_LIST_MODE', identified with command code 0xE0.'
     */
    virtual void set_daq_list_mode(XCPCTOSetListMode &cto) = 0;

#endif /* #ifndef XCP_DISABLE_SET_DAQ_LIST_MODE_COMMAND */

#ifndef XCP_DISABLE_WRITE_DAQ_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'WRITE_DAQ', identified with command code 0xE1.'
     */
    virtual void write_daq(XCPCTOWrite &cto) = 0;

#endif /* #ifndef XCP_DISABLE_WRITE_DAQ_COMMAND */

#ifndef XCP_DISABLE_SET_DAQ_PTR_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'SET_DAQ_PTR', identified with command code 0xE2.'
     */
    virtual void set_daq_ptr(XCPCTOSetPtr &cto) = 0;

#endif /* #ifndef XCP_DISABLE_SET_DAQ_PTR_COMMAND */

#ifndef XCP_DISABLE_CLEAR_DAQ_LIST_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'CLEAR_DAQ_LIST', identified with command code 0xE3.'
     */
    virtual void clear_daq_list(XCPCTOClearList &cto) = 0;

#endif /* #ifndef XCP_DISABLE_CLEAR_DAQ_LIST_COMMAND */

#ifndef XCP_DISABLE_COPY_CAL_PAGE_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'COPY_CAL_PAGE', identified with command code 0xE4.'
     */
    virtual void copy_cal_page(XCPCTOCopyCalPage &cto) = 0;

#endif /* #ifndef XCP_DISABLE_COPY_CAL_PAGE_COMMAND */

#ifndef XCP_DISABLE_GET_SEGMENT_MODE_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_SEGMENT_MODE', identified with command code 0xE5.'
     */
    virtual void get_segment_mode(XCPCTOGetSegmentMode &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_SEGMENT_MODE_COMMAND */

#ifndef XCP_DISABLE_SET_SEGMENT_MODE_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'SET_SEGMENT_MODE', identified with command code 0xE6.'
     */
    virtual void set_segment_mode(XCPCTOSetSegmentMode &cto) = 0;

#endif /* #ifndef XCP_DISABLE_SET_SEGMENT_MODE_COMMAND */

#ifndef XCP_DISABLE_GET_PAGE_INFO_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_PAGE_INFO', identified with command code 0xE7.'
     */
    virtual void get_page_info(XCPCTOGetPageInfo &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_PAGE_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_SEGMENT_INFO_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_SEGMENT_INFO', identified with command code 0xE8.'
     */
    virtual void get_segment_info(XCPCTOGetSegmentInfo &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_SEGMENT_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_PAG_PROCESSOR_INFO_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_PAG_PROCESSOR_INFO', identified with command code 0xE9.'
     */
    virtual void get_pag_processor_info(XCPCTOGetPagProcessorInfo &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_PAG_PROCESSOR_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_CAL_PAGE_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_CAL_PAGE', identified with command code 0xEA.'
     */
    virtual void get_cal_page(XCPCTOGetCalPage &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_CAL_PAGE_COMMAND */

#ifndef XCP_DISABLE_SET_CAL_PAGE_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'SET_CAL_PAGE', identified with command code 0xEB.'
     */
    virtual void set_cal_page(XCPCTOSetCalPage &cto) = 0;

#endif /* #ifndef XCP_DISABLE_SET_CAL_PAGE_COMMAND */

#ifndef XCP_DISABLE_MODIFY_BITS_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'MODIFY_BITS', identified with command code 0xEC.'
     */
    virtual void modify_bits(XCPCTOModifyBits &cto) = 0;

#endif /* #ifndef XCP_DISABLE_MODIFY_BITS_COMMAND */

#ifndef XCP_DISABLE_SHORT_DOWNLOAD_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'SHORT_DOWNLOAD', identified with command code 0xED.'
     */
    virtual void short_download(XCPCTOShortDownload &cto) = 0;

#endif /* #ifndef XCP_DISABLE_SHORT_DOWNLOAD_COMMAND */

#ifndef XCP_DISABLE_DOWNLOAD_MAX_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'DOWNLOAD_MAX', identified with command code 0xEE.'
     */
    virtual void download_max(XCPCTODownloadMax &cto) = 0;

#endif /* #ifndef XCP_DISABLE_DOWNLOAD_MAX_COMMAND */

#ifndef XCP_DISABLE_DOWNLOAD_NEXT_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'DOWNLOAD_NEXT', identified with command code 0xEF.'
     */
    virtual void download_next(XCPCTODownloadNext &cto) = 0;

#endif /* #ifndef XCP_DISABLE_DOWNLOAD_NEXT_COMMAND */

#ifndef XCP_DISABLE_DOWNLOAD_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'DOWNLOAD', identified with command code 0xF0.'
     */
    virtual void download(XCPCTODownload &cto) = 0;

#endif /* #ifndef XCP_DISABLE_DOWNLOAD_COMMAND */

#ifndef XCP_DISABLE_USER_CMD_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'USER_CMD', identified with command code 0xF1.'
     */
    virtual void user_cmd(XCPCTOUserCmd &cto) = 0;

#endif /* #ifndef XCP_DISABLE_USER_CMD_COMMAND */

#ifndef XCP_DISABLE_TRANSPORT_LAYER_CMD_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'TRANSPORT_LAYER_CMD', identified with command code 0xF2.'
     */
    virtual void transport_layer_cmd(XCPCTOTransportLayerCmd &cto) = 0;

#endif /* #ifndef XCP_DISABLE_TRANSPORT_LAYER_CMD_COMMAND */

#ifndef XCP_DISABLE_BUILD_CHECKSUM_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'BUILD_CHECKSUM', identified with command code 0xF3.'
     */
    virtual void build_checksum(XCPCTOBuildChecksum &cto) = 0;

#endif /* #ifndef XCP_DISABLE_BUILD_CHECKSUM_COMMAND */

#ifndef XCP_DISABLE_SHORT_UPLOAD_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'SHORT_UPLOAD', identified with command code 0xF4.'
     */
    virtual void short_upload(XCPCTOShortUpload &cto) = 0;

#endif /* #ifndef XCP_DISABLE_SHORT_UPLOAD_COMMAND */

#ifndef XCP_DISABLE_UPLOAD_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'UPLOAD', identified with command code 0xF5.'
     */
    virtual void upload(XCPCTOUpload &cto) = 0;

#endif /* #ifndef XCP_DISABLE_UPLOAD_COMMAND */

#ifndef XCP_DISABLE_SET_MTA_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'SET_MTA', identified with command code 0xF6.'
     */
    virtual void set_mta(XCPCTOSetMta &cto) = 0;

#endif /* #ifndef XCP_DISABLE_SET_MTA_COMMAND */

#ifndef XCP_DISABLE_UNLOCK_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'UNLOCK', identified with command code 0xF7.'
     */
    virtual void unlock(XCPCTOUnlock &cto) = 0;

#endif /* #ifndef XCP_DISABLE_UNLOCK_COMMAND */

#ifndef XCP_DISABLE_GET_SEED_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_SEED', identified with command code 0xF8.'
     */
    virtual void get_seed(XCPCTOGetSeed &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_SEED_COMMAND */

#ifndef XCP_DISABLE_SET_REQUEST_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'SET_REQUEST', identified with command code 0xF9.'
     */
    virtual void set_request(XCPCTOSetRequest &cto) = 0;

#endif /* #ifndef XCP_DISABLE_SET_REQUEST_COMMAND */

#ifndef XCP_DISABLE_GET_ID_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_ID', identified with command code 0xFA.'
     */
    virtual void get_id(XCPCTOGetId &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_ID_COMMAND */

#ifndef XCP_DISABLE_GET_COMM_MODE_INFO_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_COMM_MODE_INFO', identified with command code 0xFB.'
     */
    virtual void get_comm_mode_info(XCPCTOGetCommModeInfo &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_COMM_MODE_INFO_COMMAND */

#ifndef XCP_DISABLE_SYNCH_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'SYNCH', identified with command code 0xFC.'
     */
    virtual void synch(XCPCTOSynch &cto) = 0;

#endif /* #ifndef XCP_DISABLE_SYNCH_COMMAND */

#ifndef XCP_DISABLE_GET_STATUS_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'GET_STATUS', identified with command code 0xFD.'
     */
    virtual void get_status(XCPCTOGetStatus &cto) = 0;

#endif /* #ifndef XCP_DISABLE_GET_STATUS_COMMAND */

#ifndef XCP_DISABLE_DISCONNECT_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'DISCONNECT', identified with command code 0xFE.'
     */
    virtual void disconnect(XCPCTODisconnect &cto) = 0;

#endif /* #ifndef XCP_DISABLE_DISCONNECT_COMMAND */

#ifndef XCP_DISABLE_CONNECT_COMMAND

    /**
     * @brief .
     * @param cto the CTO (command transfer object)
     * @note corresponds to XCP protocol command 'CONNECT', identified with command code 0xFF.'
     */
    virtual void connect(XCP::CONNECT::MODE mode) = 0;

#endif /* #ifndef XCP_DISABLE_CONNECT_COMMAND */

};

#endif //XCPUI_XCP_INTERFACE_H
