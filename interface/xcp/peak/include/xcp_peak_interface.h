//
// Created by sauci on 15/11/2018.
//

#ifndef XCPUI_XCP_PEAK_INTERFACE_H
#define XCPUI_XCP_PEAK_INTERFACE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <iostream>
#include <thread>
#include <tuple>

#include "queue_safe.h"
#include "peak_api.h"
#include "peak_api_types.h"
#include "xcp_interface.h"

template<typename F, typename ...ARGS>
class XCPCTOMessage : public XCPCTOMessageInterface
{
    F function_;
    PEAKAPIInterface *api_;
    std::tuple<ARGS...> parameters_;
    std::vector<std::uint8_t> buffer_ = std::vector<std::uint8_t>(8);

public:

    explicit XCPCTOMessage(PEAKAPIInterface *C, F f, ARGS... args) : XCPCTOMessageInterface()
    {
        api_ = C;
        function_ = f;
        parameters_ = std::make_tuple(args...);
    }

    void operator()() override
    {
        //if (async)
        //{
        //    std::tuple_cat(std::make_tuple(api_, api_->get_handle()), parameters_,
        //                   std::make_tuple((char *)nullptr, (BYTE)buffer_.size()));
        //}
        //else
        //{
        auto p = std::tuple_cat(std::make_tuple(api_, api_->get_handle()), parameters_,
                                std::make_tuple(buffer_.data(), (BYTE)buffer_.size()));

        status_ = std::apply(function_, p);
        //}
    }
};

class XCPPEAKInterface : public XCPInterface
{
    Queue<XCPCTOMessageInterface *> request_cto_queue;
    Queue<XCPCTOMessageInterface *> response_cto_queue;
    std::thread *dequeue_thread_ = nullptr;

    void worker(PEAKAPIInterface *api)
    {
        bool signal;

        peak::StatusType status;
        peak::ChannelXCPType xcp_channel_;
        peak::HandleType handle;

        {
            status = api->initialize_can_channel(
                &xcp_channel_, hardware_channel_, baud_rate_, 0, 0, 0);

            if (status != peak::StatusType::DLL_XCP_ERR_OK)
            {
                throw invalid_command_error(status);
            }

            status = api->add_slave_on_can(
                xcp_channel_,
                {
                    broadcast_identifier_, master_identifier_, slave_identifier_, false
                },
                {
                    timing_parameter_[0], timing_parameter_[1], timing_parameter_[2],
                    timing_parameter_[3], timing_parameter_[4], timing_parameter_[5],
                    timing_parameter_[6], false, 0
                },
                &handle);

            if (status != peak::StatusType::DLL_XCP_ERR_OK)
            {
                throw invalid_command_error(status);
            }
        }

        while (true)
        {
            if (!request_cto_queue.empty())
            {
                auto cto = request_cto_queue.pop(signal);

                if (!signal)
                {
                    /* the dll function is called here. once called, the cto object will have
                     * updated values for both buffer_ and status_ variables.
                     */
                    cto->operator()();

                    response_cto_queue.push(cto);

                    if (this->cto_completion_handler(xcp_api_types::message_type::CTO, cto))
                    {
                        delete cto;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                std::vector<std::uint8_t> buffer = std::vector<std::uint8_t>(8);

                status = api->dequeue_packet(handle,
                                             peak::QueueType::DTO,
                                             buffer.data(),
                                             (BYTE)buffer.size());
                if (status == peak::StatusType::DLL_XCP_ERR_QUEUE_EMPTY)
                {
                    continue;
                }
                else if (status == peak::StatusType::DLL_XCP_ERR_OK)
                {
                    //completion_handler_(xcp_api_types::message_type::DTO, status, buffer);
                }
                else
                {
                    throw invalid_command_error(status);
                }
            }
        }

        {
            status = api->reset_queue(handle, peak::QueueType::CTO);

            if (status != peak::StatusType::DLL_XCP_ERR_OK)
            {
                throw invalid_command_error(status);
            }

            status = api->reset_queue(handle, peak::QueueType::DTO);

            if (status != peak::StatusType::DLL_XCP_ERR_OK)
            {
                throw invalid_command_error(status);
            }

            status = api->remove_slave(handle);

            if (status != peak::StatusType::DLL_XCP_ERR_OK)
            {
                throw invalid_command_error(status);
            }

            status = api->uninitialize_channel(xcp_channel_);

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
            dequeue_thread_ = new std::thread{std::bind(&XCPPEAKInterface::worker, this, api_)};
        }
    }

    void join_thread(bool wait_for_completion)
    {
        if (dequeue_thread_ != nullptr)
        {
            if (wait_for_completion)
            {
                while (!request_cto_queue.empty())
                {
                }
            }
            request_cto_queue.signal();
            dequeue_thread_->join();
            delete dequeue_thread_;
            dequeue_thread_ = nullptr;
        }
    }

protected:

    PEAKAPIInterface *api_;

    peak::ChannelType hardware_channel_ = peak::ChannelType::CHANNEL_BUS_NONE;
    peak::BaudRateType baud_rate_ = peak::BaudRateType::BAUD_RATE_5000;

    DWORD master_identifier_ = 0;
    DWORD slave_identifier_ = 0;
    DWORD broadcast_identifier_ = 0;

    WORD timing_parameter_[7] = {0, 0, 0, 0, 0, 0, 0};

public:

    XCPPEAKInterface(PEAKAPIInterface &peak_api_interface)
    {
        api_ = &peak_api_interface;
    }

    ~XCPPEAKInterface()
    {
        join_thread(false);
    }

    const identifier_type get_master_identifier() override
    {
        return master_identifier_;
    };

    const identifier_type get_slave_identifier() override
    {
        return slave_identifier_;
    };

    const identifier_type get_broadcast_identifier() override
    {
        return broadcast_identifier_;
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
            case peak::BaudRateType::BAUD_RATE_5000 :
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

    void dequeue_cto(bool &is_valid, status_type &status, buffer_type &buffer) override
    {
        if (!response_cto_queue.empty())
        {
            bool signal = false;
            auto message = response_cto_queue.pop(signal);

            if (!signal)
            {
                status = message->get_status();
                buffer = message->get_buffer();
                is_valid = true;
            }
            else
            {
                is_valid = false;
            }
        }
        else
        {
            is_valid = false;
        }
    }

    const timing_parameter_type
    get_timing_parameter(timing_parameter_id_type timing_parameter_id) override
    {
        switch (timing_parameter_id)
        {
            case timing_parameter_id_type::T1:
                return timing_parameter_[0];
            case timing_parameter_id_type::T2:
                return timing_parameter_[1];
            case timing_parameter_id_type::T3:
                return timing_parameter_[2];
            case timing_parameter_id_type::T4:
                return timing_parameter_[3];
            case timing_parameter_id_type::T5:
                return timing_parameter_[4];
            case timing_parameter_id_type::T6:
                return timing_parameter_[5];
            case timing_parameter_id_type::T7:
                return timing_parameter_[6];
            default:
                throw invalid_timing_parameter_error(timing_parameter_id);
        }
    }

    void set_master_identifier(identifier_type identifier) override
    {
        std::cout << "calling API set_master_identifier with value " << identifier << std::endl;
        if (identifier <= 0b00011111111111111111111111111111u)
        {
            master_identifier_ = identifier;
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
            slave_identifier_ = identifier;
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
            broadcast_identifier_ = identifier;
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
            case 800000 :
                baud_rate_ = peak::BaudRateType::BAUD_RATE_800000;
                break;
            case 500000 :
                baud_rate_ = peak::BaudRateType::BAUD_RATE_500000;
                break;
            case 250000 :
                baud_rate_ = peak::BaudRateType::BAUD_RATE_250000;
                break;
            case 125000 :
                baud_rate_ = peak::BaudRateType::BAUD_RATE_125000;
                break;
            case 100000 :
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
            case 5000 :
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

    void set_timing_parameter(timing_parameter_id_type timing_parameter_id,
                              timing_parameter_type timing_parameter) override
    {
        switch (timing_parameter_id)
        {
            case timing_parameter_id_type::T1:
                timing_parameter_[0] = (WORD)timing_parameter;
                break;
            case timing_parameter_id_type::T2:
                timing_parameter_[1] = (WORD)timing_parameter;
                break;
            case timing_parameter_id_type::T3:
                timing_parameter_[2] = (WORD)timing_parameter;
                break;
            case timing_parameter_id_type::T4:
                timing_parameter_[3] = (WORD)timing_parameter;
                break;
            case timing_parameter_id_type::T5:
                timing_parameter_[4] = (WORD)timing_parameter;
                break;
            case timing_parameter_id_type::T6:
                timing_parameter_[5] = (WORD)timing_parameter;
                break;
            case timing_parameter_id_type::T7:
                timing_parameter_[6] = (WORD)timing_parameter;
                break;
            default:
                throw invalid_timing_parameter_error(timing_parameter_id);
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

    void connect(xcp_types::connect::MODE mode) override
    {
        auto cto = new XCPCTOMessage<decltype(&PEAKAPIInterface::connect), BYTE>
            (api_, &PEAKAPIInterface::connect, mode);
        request_cto_queue.push(cto);
    }

    void disconnect() override
    {
        auto cto = new XCPCTOMessage<decltype(&PEAKAPIInterface::disconnect)>
            (api_, &PEAKAPIInterface::disconnect);
        request_cto_queue.push(cto);
    }

    void get_status() override
    {
        auto cto = new XCPCTOMessage<decltype(&PEAKAPIInterface::get_status)>
            (api_, &PEAKAPIInterface::get_status);
        request_cto_queue.push(cto);
    }

    void synch() override
    {
        auto cto = new XCPCTOMessage<decltype(&PEAKAPIInterface::synchronize)>
            (api_, &PEAKAPIInterface::synchronize);
        request_cto_queue.push(cto);
    }

    void get_comm_mode_info() override
    {
        auto cto = new XCPCTOMessage<decltype(&PEAKAPIInterface::get_communication_mode)>(
            api_, &PEAKAPIInterface::get_communication_mode);
        request_cto_queue.push(cto);
    }

    void get_id(xcp_types::get_id::TYPE request_identification) override
    {
        auto cto = new XCPCTOMessage<decltype(&PEAKAPIInterface::get_identification), BYTE>
            (api_, &PEAKAPIInterface::get_identification, request_identification);
        request_cto_queue.push(cto);
    }

    void set_request(xcp_types::set_request::MODE mode,
                     xcp_types::set_request::SESSION_CONFIGURATION_ID session_configuration_id) override
    {
        auto cto = new XCPCTOMessage<decltype(&PEAKAPIInterface::set_request), BYTE, WORD>
            (api_, &PEAKAPIInterface::set_request, mode, session_configuration_id);
        request_cto_queue.push(cto);
    }

    void get_seed(peak::HandleType xcp_handle, BYTE mode, BYTE resource) override
    {
    }

    void unlock(peak::HandleType xcp_handle, BYTE key_length, BYTE *key) override
    {
    }

    void transport_layer_cmd(BYTE sub_command,
                             BYTE *parameter_buffer,
                             WORD parameter_buffer_length) override
    {
    }

    void user_cmd(BYTE sub_command,
                  BYTE *parameter_buffer,
                  WORD parameter_buffer_length) override
    {
    }

    void set_mta(BYTE address_extension, DWORD address) override
    {
    }

    void upload(BYTE number_of_elements) override
    {
    }

    void short_upload(BYTE number_of_elements, BYTE address_extension, DWORD address) override
    {
    }

    void download(peak::HandleType xcp_handle,
                  BYTE number_of_elements,
                  BYTE *data_buffer,
                  BYTE data_buffer_length) override
    {
    }

    void download_next(BYTE number_of_elements,
                       BYTE *data_buffer,
                       BYTE data_buffer_length) override
    {
    }

    void download_max(BYTE *data_buffer) override
    {
    }

    void modify_bits(BYTE shift_value, WORD and_mask, WORD xor_mask) override
    {
    }

    void set_calibration_page(BYTE mode, peak::CalibrationPageType page) override
    {
    }

    void get_calibration_page(BYTE mode, BYTE data_segment_number) override
    {
    }

    void get_paging_processor_information(peak::HandleType xcp_handle) override
    {
    }

    void get_segment_information(BYTE mode,
                                 BYTE segment_number,
                                 BYTE segment_info,
                                 BYTE mapping_index) override
    {
    }

    void get_page_information(peak::CalibrationPageType page) override
    {
    }

    void set_segment_mode(BYTE mode, BYTE segment_number) override
    {
    }

    void get_segment_mode(BYTE segment_number) override
    {
    }

    void copy_calibration_page(peak::CalibrationPageType source,
                               peak::CalibrationPageType destination) override
    {
    }

    void set_daq_list_pointer(WORD daq_list_number, BYTE odt_number, BYTE odt_entry) override
    {
    }

    void write_daq_list_entry(peak::ODTEntryType entry) override
    {
    }

    void write_daq_list_entries(BYTE number_of_elements, peak::ODTEntryType *elements) override
    {
    }

    void set_daq_list_mode(BYTE mode, peak::DAQListConfigType configuration) override
    {
    }

    void start_stop_daq_list(BYTE mode, WORD daq_list_number) override
    {
    }

    void start_stop_synchronized_daq_list(BYTE mode) override
    {
    }

    void read_daq_list_entry(peak::HandleType xcp_handle) override
    {
    }

    void get_daq_clock(peak::HandleType xcp_handle) override
    {
    }

    void get_daq_processor_information(peak::HandleType xcp_handle) override
    {
    }

    void get_daq_resolution_information(peak::HandleType xcp_handle) override
    {
    }

    void get_daq_list_mode(WORD daq_list_number) override
    {
    }

    void get_event_channel_information(WORD event_channel_number) override
    {
    }

    void clear_daq_list(WORD daq_list_number) override
    {
    }

    void get_daq_list_information(WORD daq_list_number) override
    {
    }

    void free_daq_lists(peak::HandleType xcp_handle) override
    {
    }

    void allocate_daq_lists(WORD daq_count) override
    {
    }

    void allocate_odt(WORD daq_list_number, BYTE odt_count) override
    {
    }

    void allocate_odt_entry(WORD daq_list_number, BYTE odt_number, BYTE entries_count) override
    {
    }

    void program_start(peak::HandleType xcp_handle) override
    {
    }

    void program_clear(BYTE mode, DWORD clear_range) override
    {
    }

    void program(BYTE number_of_elements, BYTE *data_buffer, BYTE data_buffer_length) override
    {
    }

    void program_reset(peak::HandleType xcp_handle) override
    {
    }

    void get_program_processor_information(peak::HandleType xcp_handle) override
    {
    }

    void get_sector_information(BYTE mode, BYTE sector_number) override
    {
    }

    void program_prepare(WORD code_size) override
    {
    }

    void program_format(peak::ProgramFormatType format) override
    {
    }

    void program_next(BYTE number_of_elements, BYTE *data_buffer, BYTE data_length) override
    {
    }

    void program_max(BYTE *element_buffer) override
    {
    }

    void program_verify(BYTE mode, WORD type, DWORD value) override
    {
    }

    void build_checksum(DWORD block_size) override
    {
    }

    void get_error_text(peak::StatusType error_code, LPSTR text_buffer)
    {
    }

    bool
    cto_completion_handler(message_type message_type, XCPMessageInterface *cto_message) override
    {
        return false;
    }
};

#endif //XCPUI_XCP_PEAK_INTERFACE_H
