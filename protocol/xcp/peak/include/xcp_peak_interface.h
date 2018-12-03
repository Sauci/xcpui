//
// Created by sauci on 15/11/2018.
//

#ifndef XCPUI_XCP_PEAK_INTERFACE_H
#define XCPUI_XCP_PEAK_INTERFACE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <iostream>
#include <thread>
#include <tuple>

#include "peak_api.h"
#include "queue_safe.h"
#include "xcp_interface.h"
#include "xcp_peak_interface_types.h"

#define MASTER_ID 0
#define SLAVE_ID 1
#define BROADCAST_ID 2

class XCPPEAKInterfaceTest;

class XCPPEAKInterface : public XCPInterface
{
    friend XCPPEAKInterfaceTest;

    Queue<XCPCTORequestInterface *> pending_queue;
    Queue<XCPCTORequestInterface *> ready_queue;
    Queue<XCPDTOInterface *> dto_queue;
    std::thread *dequeue_thread_ = nullptr;

    PEAKAPIInterface &api_;

    peak::ChannelType hardware_channel_ = peak::ChannelType::CHANNEL_BUS_NONE;
    peak::BaudRateType baud_rate_ = peak::BaudRateType::BAUD_RATE_5000;
    DWORD identifier_[3] = {0, 0, 0};
    WORD timing_parameter_[7] = {0, 0, 0, 0, 0, 0, 0};

    void worker(PEAKAPIInterface &api)
    {
        bool signal;

        peak::StatusType status;
        peak::ChannelXCPType xcp_channel_;
        peak::HandleType handle;

        {
            status =
                api.initialize_can_channel(&xcp_channel_, hardware_channel_, baud_rate_, 0, 0, 0);

            if (status != peak::StatusType::DLL_XCP_ERR_OK)
            {
                throw invalid_command_error(status);
            }

            status = api.add_slave_on_can(xcp_channel_,
                                          {identifier_[BROADCAST_ID],
                                           identifier_[MASTER_ID],
                                           identifier_[SLAVE_ID], false},
                                          {timing_parameter_[0], timing_parameter_[1],
                                           timing_parameter_[2], timing_parameter_[3],
                                           timing_parameter_[4], timing_parameter_[5],
                                           timing_parameter_[6], false, 0},
                                          &handle);

            if (status != peak::StatusType::DLL_XCP_ERR_OK)
            {
                throw invalid_command_error(status);
            }
        }

        while (true)
        {
            if (!pending_queue.empty())
            {
                auto request_cto = pending_queue.pop(signal);

                if (!signal)
                {
                    /* the dll function is called here. once called, the cto object will
                     * have updated values for both buffer_ and status_ variables.
                     */
                    request_cto->operator()();

                    ready_queue.push(request_cto);

                    if (this->cto_completion_handler(request_cto))
                    {
                        delete request_cto;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                auto dto = new XCPDTOMessage();

                dto->set_status(api.dequeue_packet(handle,
                                                   peak::QueueType::DTO,
                                                   const_cast<BYTE *>(dto->get_buffer().data()),
                                                   (BYTE)dto->get_buffer().size()));
                if (dto->get_status() == peak::StatusType::DLL_XCP_ERR_QUEUE_EMPTY)
                {
                    delete dto;
                    continue;
                }
                else
                {
                    dto_queue.push(dto);
                }
            }
        }

        {
            status = api.reset_queue(handle, peak::QueueType::CTO);

            if (status != peak::StatusType::DLL_XCP_ERR_OK)
            {
                throw invalid_command_error(status);
            }

            status = api.reset_queue(handle, peak::QueueType::DTO);

            if (status != peak::StatusType::DLL_XCP_ERR_OK)
            {
                throw invalid_command_error(status);
            }

            status = api.remove_slave(handle);

            if (status != peak::StatusType::DLL_XCP_ERR_OK)
            {
                throw invalid_command_error(status);
            }

            status = api.uninitialize_channel(xcp_channel_);

            if (status != peak::StatusType::DLL_XCP_ERR_OK)
            {
                throw invalid_command_error(status);
            }
        }
    }

    void start_thread()
    {
        if (dequeue_thread_ == nullptr)
        {
            dequeue_thread_ =
                new std::thread{std::bind(&XCPPEAKInterface::worker, this, std::ref(api_))};
        }
    }

    void join_thread(bool wait_for_completion)
    {
        if (dequeue_thread_ != nullptr)
        {
            if (wait_for_completion)
            {
                while (!pending_queue.empty())
                {
                }
            }
            pending_queue.signal();
            dequeue_thread_->join();
            delete dequeue_thread_;
            dequeue_thread_ = nullptr;
        }
    }

public:

    XCPPEAKInterface(PEAKAPIInterface &peak_api_interface) : api_(peak_api_interface)
    {
    }

    ~XCPPEAKInterface()
    {
        join_thread(false);
    }

    const identifier_type get_master_identifier() override
    {
        return identifier_[MASTER_ID];
    };

    const identifier_type get_slave_identifier() override
    {
        return identifier_[SLAVE_ID];
    };

    const identifier_type get_broadcast_identifier() override
    {
        return identifier_[BROADCAST_ID];
    };

    XCPDTOInterface *dequeue_dto() override
    {
        if (!dto_queue.empty())
        {
            bool signal = false;
            auto dto = dto_queue.pop(signal);

            if (!signal)
            {
                return dto;
            }
        }
        return nullptr;
    }

    const timing_type get_timing_parameter(timing_id_type timing_id) override
    {
        switch (timing_id)
        {
            case timing_id_type::T1:
                return timing_parameter_[0];
            case timing_id_type::T2:
                return timing_parameter_[1];
            case timing_id_type::T3:
                return timing_parameter_[2];
            case timing_id_type::T4:
                return timing_parameter_[3];
            case timing_id_type::T5:
                return timing_parameter_[4];
            case timing_id_type::T6:
                return timing_parameter_[5];
            case timing_id_type::T7:
                return timing_parameter_[6];
            default:
                throw invalid_timing_parameter_error(timing_id);
        }
    }

    void set_master_identifier(identifier_type identifier) override
    {
        std::cout << "calling API set_master_identifier with value " << identifier << std::endl;
        if (identifier <= 0b00011111111111111111111111111111u)
        {
            identifier_[MASTER_ID] = identifier;
        }
        else
        {
            throw invalid_identifier_error(identifier);
        }
    }

    void set_slave_identifier(identifier_type identifier) override
    {
        if (identifier <= 0b00011111111111111111111111111111u)
        {
            identifier_[SLAVE_ID] = identifier;
        }
        else
        {
            throw invalid_identifier_error(identifier);
        }
    }

    void set_broadcast_identifier(identifier_type identifier) override
    {
        if (identifier <= 0b00011111111111111111111111111111u)
        {
            identifier_[BROADCAST_ID] = identifier;
        }
        else
        {
            throw invalid_identifier_error(identifier);
        }
    }

    void set_timing_parameter(timing_id_type timing_id, timing_type timing) override
    {
        switch (timing_id)
        {
            case timing_id_type::T1:
                timing_parameter_[0] = (WORD)timing;
                break;
            case timing_id_type::T2:
                timing_parameter_[1] = (WORD)timing;
                break;
            case timing_id_type::T3:
                timing_parameter_[2] = (WORD)timing;
                break;
            case timing_id_type::T4:
                timing_parameter_[3] = (WORD)timing;
                break;
            case timing_id_type::T5:
                timing_parameter_[4] = (WORD)timing;
                break;
            case timing_id_type::T6:
                timing_parameter_[5] = (WORD)timing;
                break;
            case timing_id_type::T7:
                timing_parameter_[6] = (WORD)timing;
                break;
            default:
                throw invalid_timing_parameter_error(timing_id);
        }
    }

    void initialize_hardware() override
    {
        start_thread();
    }

    void de_initialize_hardware(bool wait_for_completion) override
    {
        join_thread(wait_for_completion);
    }

#ifndef XCP_DISABLE_PROGRAM_VERIFY_COMMAND

    void program_verify(XCPCTOProgramVerify &cto) override
    {
        pending_queue.push(new PEAKXCPCTOProgramVerify(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_PROGRAM_VERIFY_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_MAX_COMMAND

    void program_max(XCPCTOProgramMax &cto) override
    {
        pending_queue.push(new PEAKXCPCTOProgramMax(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_PROGRAM_MAX_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_NEXT_COMMAND

    void program_next(XCPCTOProgramNext &cto) override
    {
        pending_queue.push(new PEAKXCPCTOProgramNext(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_PROGRAM_NEXT_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_FORMAT_COMMAND

    void program_format(XCPCTOProgramFormat &cto) override
    {
        pending_queue.push(new PEAKXCPCTOProgramFormat(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_PROGRAM_FORMAT_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_PREPARE_COMMAND

    void program_prepare(XCPCTOProgramPrepare &cto) override
    {
        pending_queue.push(new PEAKXCPCTOProgramPrepare(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_PROGRAM_PREPARE_COMMAND */

#ifndef XCP_DISABLE_GET_SECTOR_INFO_COMMAND

    void get_sector_info(XCPCTOGetSectorInfo &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetSectorInfo(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_SECTOR_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_PGM_PROCESSOR_INFO_COMMAND

    void get_pgm_processor_info(XCPCTOGetPgmProcessorInfo &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetPgmProcessorInfo(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_PGM_PROCESSOR_INFO_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_RESET_COMMAND

    void program_reset(XCPCTOProgramReset &cto) override
    {
        pending_queue.push(new PEAKXCPCTOProgramReset(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_PROGRAM_RESET_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_COMMAND

    void program(XCPCTOProgram &cto) override
    {
        pending_queue.push(new PEAKXCPCTOProgram(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_PROGRAM_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_CLEAR_COMMAND

    void program_clear(XCPCTOProgramClear &cto) override
    {
        pending_queue.push(new PEAKXCPCTOProgramClear(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_PROGRAM_CLEAR_COMMAND */

#ifndef XCP_DISABLE_PROGRAM_START_COMMAND

    void program_start(XCPCTOProgramStart &cto) override
    {
        pending_queue.push(new PEAKXCPCTOProgramStart(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_PROGRAM_START_COMMAND */

#ifndef XCP_DISABLE_ALLOC_ODT_ENTRY_COMMAND

    void alloc_odt_entry(XCPCTOAllocEntry &cto) override
    {
        pending_queue.push(new PEAKXCPCTOAllocEntry(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_ALLOC_ODT_ENTRY_COMMAND */

#ifndef XCP_DISABLE_ALLOC_ODT_COMMAND

    void alloc_odt(XCPCTOAlloc &cto) override
    {
        pending_queue.push(new PEAKXCPCTOAlloc(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_ALLOC_ODT_COMMAND */

#ifndef XCP_DISABLE_ALLOC_DAQ_COMMAND

    void alloc_daq(XCPCTOAlloc &cto) override
    {
        pending_queue.push(new PEAKXCPCTOAlloc(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_ALLOC_DAQ_COMMAND */

#ifndef XCP_DISABLE_FREE_DAQ_COMMAND

    void free_daq(XCPCTOFree &cto) override
    {
        pending_queue.push(new PEAKXCPCTOFree(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_FREE_DAQ_COMMAND */

#ifndef XCP_DISABLE_GET_DAQ_EVENT_INFO_COMMAND

    void get_daq_event_info(XCPCTOGetEventInfo &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetEventInfo(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_DAQ_EVENT_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_DAQ_LIST_INFO_COMMAND

    void get_daq_list_info(XCPCTOGetListInfo &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetListInfo(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_DAQ_LIST_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_DAQ_RESOLUTION_INFO_COMMAND

    void get_daq_resolution_info(XCPCTOGetResolutionInfo &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetResolutionInfo(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_DAQ_RESOLUTION_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_DAQ_PROCESSOR_INFO_COMMAND

    void get_daq_processor_info(XCPCTOGetProcessorInfo &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetProcessorInfo(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_DAQ_PROCESSOR_INFO_COMMAND */

#ifndef XCP_DISABLE_READ_DAQ_COMMAND

    void read_daq(XCPCTORead &cto) override
    {
        pending_queue.push(new PEAKXCPCTORead(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_READ_DAQ_COMMAND */

#ifndef XCP_DISABLE_GET_DAQ_CLOCK_COMMAND

    void get_daq_clock(XCPCTOGetClock &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetClock(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_DAQ_CLOCK_COMMAND */

#ifndef XCP_DISABLE_START_STOP_SYNCH_COMMAND

    void start_stop_synch(XCPCTOStartStopSynch &cto) override
    {
        pending_queue.push(new PEAKXCPCTOStartStopSynch(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_START_STOP_SYNCH_COMMAND */

#ifndef XCP_DISABLE_START_STOP_DAQ_LIST_COMMAND

    void start_stop_daq_list(XCPCTOStartStopList &cto) override
    {
        pending_queue.push(new PEAKXCPCTOStartStopList(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_START_STOP_DAQ_LIST_COMMAND */

#ifndef XCP_DISABLE_GET_DAQ_LIST_MODE_COMMAND

    void get_daq_list_mode(XCPCTOGetListMode &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetListMode(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_DAQ_LIST_MODE_COMMAND */

#ifndef XCP_DISABLE_SET_DAQ_LIST_MODE_COMMAND

    void set_daq_list_mode(XCPCTOSetListMode &cto) override
    {
        pending_queue.push(new PEAKXCPCTOSetListMode(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_SET_DAQ_LIST_MODE_COMMAND */

#ifndef XCP_DISABLE_WRITE_DAQ_COMMAND

    void write_daq(XCPCTOWrite &cto) override
    {
        pending_queue.push(new PEAKXCPCTOWrite(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_WRITE_DAQ_COMMAND */

#ifndef XCP_DISABLE_SET_DAQ_PTR_COMMAND

    void set_daq_ptr(XCPCTOSetPtr &cto) override
    {
        pending_queue.push(new PEAKXCPCTOSetPtr(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_SET_DAQ_PTR_COMMAND */

#ifndef XCP_DISABLE_CLEAR_DAQ_LIST_COMMAND

    void clear_daq_list(XCPCTOClearList &cto) override
    {
        pending_queue.push(new PEAKXCPCTOClearList(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_CLEAR_DAQ_LIST_COMMAND */

#ifndef XCP_DISABLE_COPY_CAL_PAGE_COMMAND

    void copy_cal_page(XCPCTOCopyCalPage &cto) override
    {
        pending_queue.push(new PEAKXCPCTOCopyCalPage(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_COPY_CAL_PAGE_COMMAND */

#ifndef XCP_DISABLE_GET_SEGMENT_MODE_COMMAND

    void get_segment_mode(XCPCTOGetSegmentMode &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetSegmentMode(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_SEGMENT_MODE_COMMAND */

#ifndef XCP_DISABLE_SET_SEGMENT_MODE_COMMAND

    void set_segment_mode(XCPCTOSetSegmentMode &cto) override
    {
        pending_queue.push(new PEAKXCPCTOSetSegmentMode(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_SET_SEGMENT_MODE_COMMAND */

#ifndef XCP_DISABLE_GET_PAGE_INFO_COMMAND

    void get_page_info(XCPCTOGetPageInfo &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetPageInfo(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_PAGE_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_SEGMENT_INFO_COMMAND

    void get_segment_info(XCPCTOGetSegmentInfo &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetSegmentInfo(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_SEGMENT_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_PAG_PROCESSOR_INFO_COMMAND

    void get_pag_processor_info(XCPCTOGetPagProcessorInfo &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetPagProcessorInfo(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_PAG_PROCESSOR_INFO_COMMAND */

#ifndef XCP_DISABLE_GET_CAL_PAGE_COMMAND

    void get_cal_page(XCPCTOGetCalPage &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetCalPage(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_CAL_PAGE_COMMAND */

#ifndef XCP_DISABLE_SET_CAL_PAGE_COMMAND

    void set_cal_page(XCPCTOSetCalPage &cto) override
    {
        pending_queue.push(new PEAKXCPCTOSetCalPage(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_SET_CAL_PAGE_COMMAND */

#ifndef XCP_DISABLE_MODIFY_BITS_COMMAND

    void modify_bits(XCPCTOModifyBits &cto) override
    {
        pending_queue.push(new PEAKXCPCTOModifyBits(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_MODIFY_BITS_COMMAND */

#ifndef XCP_DISABLE_SHORT_DOWNLOAD_COMMAND

    void short_download(XCPCTOShortDownload &cto) override
    {
        pending_queue.push(new PEAKXCPCTOShortDownload(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_SHORT_DOWNLOAD_COMMAND */

#ifndef XCP_DISABLE_DOWNLOAD_MAX_COMMAND

    void download_max(XCPCTODownloadMax &cto) override
    {
        pending_queue.push(new PEAKXCPCTODownloadMax(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_DOWNLOAD_MAX_COMMAND */

#ifndef XCP_DISABLE_DOWNLOAD_NEXT_COMMAND

    void download_next(XCPCTODownloadNext &cto) override
    {
        pending_queue.push(new PEAKXCPCTODownloadNext(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_DOWNLOAD_NEXT_COMMAND */

#ifndef XCP_DISABLE_DOWNLOAD_COMMAND

    void download(XCPCTODownload &cto) override
    {
        pending_queue.push(new PEAKXCPCTODownload(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_DOWNLOAD_COMMAND */

#ifndef XCP_DISABLE_USER_CMD_COMMAND

    void user_cmd(XCPCTOUserCmd &cto) override
    {
        pending_queue.push(new PEAKXCPCTOUserCmd(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_USER_CMD_COMMAND */

#ifndef XCP_DISABLE_TRANSPORT_LAYER_CMD_COMMAND

    void transport_layer_cmd(XCPCTOTransportLayerCmd &cto) override
    {
        pending_queue.push(new PEAKXCPCTOTransportLayerCmd(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_TRANSPORT_LAYER_CMD_COMMAND */

#ifndef XCP_DISABLE_BUILD_CHECKSUM_COMMAND

    void build_checksum(XCPCTOBuildChecksum &cto) override
    {
        pending_queue.push(new PEAKXCPCTOBuildChecksum(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_BUILD_CHECKSUM_COMMAND */

#ifndef XCP_DISABLE_SHORT_UPLOAD_COMMAND

    void short_upload(XCPCTOShortUpload &cto) override
    {
        pending_queue.push(new PEAKXCPCTOShortUpload(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_SHORT_UPLOAD_COMMAND */

#ifndef XCP_DISABLE_UPLOAD_COMMAND

    void upload(XCPCTOUpload &cto) override
    {
        pending_queue.push(new PEAKXCPCTOUpload(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_UPLOAD_COMMAND */

#ifndef XCP_DISABLE_SET_MTA_COMMAND

    void set_mta(XCPCTOSetMta &cto) override
    {
        pending_queue.push(new PEAKXCPCTOSetMta(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_SET_MTA_COMMAND */

#ifndef XCP_DISABLE_UNLOCK_COMMAND

    void unlock(XCPCTOUnlock &cto) override
    {
        pending_queue.push(new PEAKXCPCTOUnlock(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_UNLOCK_COMMAND */

#ifndef XCP_DISABLE_GET_SEED_COMMAND

    void get_seed(XCPCTOGetSeed &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetSeed(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_SEED_COMMAND */

#ifndef XCP_DISABLE_SET_REQUEST_COMMAND

    void set_request(XCPCTOSetRequest &cto) override
    {
        pending_queue.push(new PEAKXCPCTOSetRequest(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_SET_REQUEST_COMMAND */

#ifndef XCP_DISABLE_GET_ID_COMMAND

    void get_id(XCPCTOGetId &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetId(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_ID_COMMAND */

#ifndef XCP_DISABLE_GET_COMM_MODE_INFO_COMMAND

    void get_comm_mode_info(XCPCTOGetCommModeInfo &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetCommModeInfo(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_COMM_MODE_INFO_COMMAND */

#ifndef XCP_DISABLE_SYNCH_COMMAND

    void synch(XCPCTOSynch &cto) override
    {
        pending_queue.push(new PEAKXCPCTOSynch(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_SYNCH_COMMAND */

#ifndef XCP_DISABLE_GET_STATUS_COMMAND

    void get_status(XCPCTOGetStatus &cto) override
    {
        pending_queue.push(new PEAKXCPCTOGetStatus(api_, ));
    }

#endif /* #ifndef XCP_DISABLE_GET_STATUS_COMMAND */

#ifndef XCP_DISABLE_DISCONNECT_COMMAND

    void disconnect(XCPCTODisconnect &cto) override
    {
        pending_queue.push(new PEAKXCPCTODisconnect(api_));
    }

#endif /* #ifndef XCP_DISABLE_DISCONNECT_COMMAND */

#ifndef XCP_DISABLE_CONNECT_COMMAND

    void connect(XCP::CONNECT::MODE mode) override
    {
        auto cto = new XCPCTOMessage<decltype(&PEAKAPIInterface::connect), BYTE>
            (api_, &PEAKAPIInterface::connect, (BYTE)mode);
        pending_queue.push(cto);
    }

#endif /* #ifndef XCP_DISABLE_CONNECT_COMMAND */

#if 0
    void get_id(xcp_types::get_id::TYPE request_identification) override
    {
        auto cto = new XCPCTOMessage<decltype(&PEAKAPIInterface::get_identification), BYTE>
            (api_, &PEAKAPIInterface::get_identification, request_identification);
        request_cto_queue.push(cto);
    }
    void get_comm_mode_info() override
    {
        auto cto = new XCPCTOMessage<decltype(&PEAKAPIInterface::get_communication_mode)>(
            api_, &PEAKAPIInterface::get_communication_mode);
        request_cto_queue.push(cto);
    }
#endif

    bool cto_completion_handler(XCPCTORequestInterface *cto_message) override
    {
        return false;
    }
};

#endif // XCPUI_XCP_PEAK_INTERFACE_H
