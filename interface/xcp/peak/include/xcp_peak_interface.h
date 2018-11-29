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

/*
template<typename F, typename... ARGS>
class PEAKCTOCallable : public XCPCTORequestInterface
{
    peak::StatusType status_;
    F function_;
    PEAKAPIInterface *api_;
    std::tuple<ARGS...> parameters_;
    std::vector<std::uint8_t> buffer_ = std::vector<std::uint8_t>(8);

public:

    explicit PEAKCTOCallable(PEAKAPIInterface *C, F f, ARGS... args) : XCPCTORequestInterface()
    {
        api_ = C;
        function_ = f;
        parameters_ = std::make_tuple(args...);
    }

    void operator()() override
    {
        // if (async)
        //{
        //    std::tuple_cat(std::make_tuple(api_, api_->get_handle()), parameters_,
        //                   std::make_tuple((char *)nullptr, (BYTE)buffer_.size()));
        //}
        // else
        //{
        throw "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        auto p = std::tuple_cat(std::make_tuple(api_, api_->get_handle()), parameters_,
                                std::make_tuple(buffer_.data(), (BYTE)buffer_.size()));

        status_ = std::apply(function_, p);
        //}
    }
};
*/

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

    const baud_rate_type get_baud_rate() override
    {
        switch (baud_rate_)
        {
            case peak::BaudRateType::BAUD_RATE_1000000:
                return 1000000;
            case peak::BaudRateType::BAUD_RATE_800000:
                return 800000;
            case peak::BaudRateType::BAUD_RATE_500000:
                return 500000;
            case peak::BaudRateType::BAUD_RATE_250000:
                return 250000;
            case peak::BaudRateType::BAUD_RATE_125000:
                return 125000;
            case peak::BaudRateType::BAUD_RATE_100000:
                return 100000;
            case peak::BaudRateType::BAUD_RATE_95000:
                return 95000;
            case peak::BaudRateType::BAUD_RATE_83000:
                return 83000;
            case peak::BaudRateType::BAUD_RATE_50000:
                return 50000;
            case peak::BaudRateType::BAUD_RATE_47000:
                return 47000;
            case peak::BaudRateType::BAUD_RATE_33000:
                return 33000;
            case peak::BaudRateType::BAUD_RATE_20000:
                return 20000;
            case peak::BaudRateType::BAUD_RATE_10000:
                return 10000;
            case peak::BaudRateType::BAUD_RATE_5000:
                return 5000;
            default:
                throw invalid_baud_rate_error(baud_rate_);
        }
    };

    const hardware_channel_type get_hardware_channel() override
    {
        switch (hardware_channel_)
        {
            case peak::ChannelType::CHANNEL_BUS_USB_1:
                return 1;
            case peak::ChannelType::CHANNEL_BUS_USB_2:
                return 2;
            case peak::ChannelType::CHANNEL_BUS_USB_3:
                return 3;
            case peak::ChannelType::CHANNEL_BUS_USB_4:
                return 4;
            case peak::ChannelType::CHANNEL_BUS_USB_5:
                return 5;
            case peak::ChannelType::CHANNEL_BUS_USB_6:
                return 6;
            case peak::ChannelType::CHANNEL_BUS_USB_7:
                return 7;
            case peak::ChannelType::CHANNEL_BUS_USB_8:
                return 8;
            case peak::ChannelType::CHANNEL_BUS_USB_9:
                return 9;
            case peak::ChannelType::CHANNEL_BUS_USB_10:
                return 10;
            case peak::ChannelType::CHANNEL_BUS_USB_11:
                return 11;
            case peak::ChannelType::CHANNEL_BUS_USB_12:
                return 12;
            case peak::ChannelType::CHANNEL_BUS_USB_13:
                return 13;
            case peak::ChannelType::CHANNEL_BUS_USB_14:
                return 14;
            case peak::ChannelType::CHANNEL_BUS_USB_15:
                return 15;
            case peak::ChannelType::CHANNEL_BUS_USB_16:
                return 16;
            default:
                throw invalid_hardware_channel_error(hardware_channel_);
        }
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

    void set_baud_rate(baud_rate_type baud_rate) override
    {
        switch (baud_rate)
        {
            case 1000000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_1000000;
                break;
            case 800000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_800000;
                break;
            case 500000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_500000;
                break;
            case 250000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_250000;
                break;
            case 125000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_125000;
                break;
            case 100000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_100000;
                break;
            case 95000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_95000;
                break;
            case 83000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_83000;
                break;
            case 50000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_50000;
                break;
            case 47000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_47000;
                break;
            case 33000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_33000;
                break;
            case 20000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_20000;
                break;
            case 10000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_10000;
                break;
            case 5000:
                baud_rate_ = peak::BaudRateType::BAUD_RATE_5000;
                break;
            default:
                throw invalid_baud_rate_error(baud_rate);
        }
    }

    void set_hardware_channel(interface_types::hardware_channel_type hardware_channel) override
    {
        switch (hardware_channel)
        {
            case 1:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_1;
                break;
            case 2:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_2;
                break;
            case 3:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_3;
                break;
            case 4:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_4;
                break;
            case 5:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_5;
                break;
            case 6:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_6;
                break;
            case 7:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_7;
                break;
            case 8:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_8;
                break;
            case 9:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_9;
                break;
            case 10:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_10;
                break;
            case 11:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_11;
                break;
            case 12:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_12;
                break;
            case 13:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_13;
                break;
            case 14:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_14;
                break;
            case 15:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_15;
                break;
            case 16:
                hardware_channel_ = peak::ChannelType::CHANNEL_BUS_USB_16;
                break;
            default:
                throw invalid_hardware_channel_error(hardware_channel);
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
