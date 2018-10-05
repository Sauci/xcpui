//
// Created by sauci on 15/11/2018.
//

#ifndef XCPUI_PEAK_API_H
#define XCPUI_PEAK_API_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <Windows.h>

#include "peak_api_interface.h"

class PEAKAPI : public PEAKAPIInterface
{
    static HINSTANCE basic_library_;
    static HINSTANCE isotp_library_;
    static HINSTANCE xcp_library_;

    typedef peak::StatusType(__stdcall *c_initialize_can_channel)(peak::ChannelXCPType *,
                                                                  peak::ChannelType,
                                                                  peak::BaudRateType,
                                                                  peak::HardwareType, std::uint32_t,
                                                                  std::uint16_t);

    typedef peak::StatusType(__stdcall *c_initialize_can_channel_fd)(peak::ChannelXCPType *,
                                                                     peak::ChannelType,
                                                                     peak::BaudRateFDType);

    typedef peak::StatusType(__stdcall *c_uninitialize_channel)(peak::ChannelXCPType);

    typedef peak::StatusType(__stdcall *c_add_slave_on_can)(peak::ChannelXCPType,
                                                            peak::TransportCANType,
                                                            peak::ProtocolType, peak::HandleType *);

    typedef peak::StatusType(__stdcall *c_remove_slave)(peak::HandleType);

    typedef peak::StatusType(__stdcall *c_dequeue_packet)(peak::HandleType, peak::QueueType,
                                                          std::uint8_t *, std::uint16_t);

    typedef peak::StatusType(__stdcall *c_reset_queue)(peak::HandleType, peak::QueueType);

    typedef peak::StatusType(__stdcall *c_connect)(peak::HandleType, std::uint8_t, std::uint8_t *,
                                                   std::uint8_t);

    typedef peak::StatusType(__stdcall *c_disconnect)(peak::HandleType, std::uint8_t *,
                                                      std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_status)(peak::HandleType, std::uint8_t *,
                                                      std::uint8_t);

    typedef peak::StatusType(__stdcall *c_synchronize)(peak::HandleType, std::uint8_t *,
                                                       std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_communication_mode)(peak::HandleType, std::uint8_t *,
                                                                  std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_identification)(peak::HandleType, std::uint8_t,
                                                              std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_set_request)(peak::HandleType, std::uint8_t,
                                                       std::uint16_t, std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_seed)(peak::HandleType, std::uint8_t, std::uint8_t,
                                                    std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_unlock)(peak::HandleType, std::uint8_t, std::uint8_t *,
                                                  std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_transport_layer_command)(peak::HandleType, std::uint8_t,
                                                                   std::uint8_t *, std::uint16_t,
                                                                   std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_user_command)(peak::HandleType, std::uint8_t,
                                                        std::uint8_t *, std::uint16_t,
                                                        std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_set_memory_transfer_address)(peak::HandleType,
                                                                       std::uint8_t, std::uint32_t,
                                                                       std::uint8_t *,
                                                                       std::uint8_t);

    typedef peak::StatusType(__stdcall *c_upload)(peak::HandleType, std::uint8_t, std::uint8_t *,
                                                  std::uint8_t);

    typedef peak::StatusType(__stdcall *c_short_upload)(peak::HandleType, std::uint8_t,
                                                        std::uint8_t, std::uint32_t, std::uint8_t *,
                                                        std::uint8_t);

    typedef peak::StatusType(__stdcall *c_download)(peak::HandleType, std::uint8_t, std::uint8_t *,
                                                    std::uint8_t, std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_download_next)(peak::HandleType, std::uint8_t,
                                                         std::uint8_t *, std::uint8_t,
                                                         std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_download_max)(peak::HandleType, std::uint8_t *,
                                                        std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_modify_bits)(peak::HandleType, std::uint8_t,
                                                       std::uint16_t, std::uint16_t, std::uint8_t *,
                                                       std::uint8_t);

    typedef peak::StatusType(__stdcall *c_set_calibration_page)(peak::HandleType, std::uint8_t,
                                                                peak::CalibrationPageType,
                                                                std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_calibration_page)(peak::HandleType, std::uint8_t,
                                                                std::uint8_t, std::uint8_t *,
                                                                std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_paging_processor_information)(peak::HandleType,
                                                                            std::uint8_t *,
                                                                            std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_segment_information)(peak::HandleType, std::uint8_t,
                                                                   std::uint8_t, std::uint8_t,
                                                                   std::uint8_t, std::uint8_t *,
                                                                   std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_page_information)(peak::HandleType,
                                                                peak::CalibrationPageType,
                                                                std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_set_segment_mode)(peak::HandleType, std::uint8_t,
                                                            std::uint8_t, std::uint8_t *,
                                                            std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_segment_mode)(peak::HandleType, std::uint8_t,
                                                            std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_copy_calibration_page)(peak::HandleType,
                                                                 peak::CalibrationPageType,
                                                                 peak::CalibrationPageType,
                                                                 std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_set_daq_list_pointer)(peak::HandleType, std::uint16_t,
                                                                std::uint8_t, std::uint8_t,
                                                                std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_write_daq_list_entry)(peak::HandleType,
                                                                peak::ODTEntryType, std::uint8_t *,
                                                                std::uint8_t);

    typedef peak::StatusType(__stdcall *c_write_daq_list_entries)(peak::HandleType, std::uint8_t,
                                                                  peak::ODTEntryType *,
                                                                  std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_set_daq_list_mode)(peak::HandleType, std::uint8_t,
                                                             peak::DAQListConfigType,
                                                             std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_start_stop_daq_list)(peak::HandleType, std::uint8_t,
                                                               std::uint16_t, std::uint8_t *,
                                                               std::uint8_t);

    typedef peak::StatusType(__stdcall *c_start_stop_synchronized_daq_list)(peak::HandleType,
                                                                            std::uint8_t,
                                                                            std::uint8_t *,
                                                                            std::uint8_t);

    typedef peak::StatusType(__stdcall *c_read_daq_list_entry)(peak::HandleType, std::uint8_t *,
                                                               std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_daq_clock)(peak::HandleType, std::uint8_t *,
                                                         std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_daq_processor_information)(peak::HandleType,
                                                                         std::uint8_t *,
                                                                         std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_daq_resolution_information)(peak::HandleType,
                                                                          std::uint8_t *,
                                                                          std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_daq_list_mode)(peak::HandleType, std::uint16_t,
                                                             std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_event_channel_information)(peak::HandleType,
                                                                         std::uint16_t,
                                                                         std::uint8_t *,
                                                                         std::uint8_t);

    typedef peak::StatusType(__stdcall *c_clear_daq_list)(peak::HandleType, std::uint16_t,
                                                          std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_daq_list_information)(peak::HandleType, std::uint16_t,
                                                                    std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_free_daq_lists)(peak::HandleType, std::uint8_t *,
                                                          std::uint8_t);

    typedef peak::StatusType(__stdcall *c_allocate_daq_lists)(peak::HandleType, std::uint16_t,
                                                              std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_allocate_odt)(peak::HandleType, std::uint16_t,
                                                        std::uint8_t, std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_allocate_odt_entry)(peak::HandleType, std::uint16_t,
                                                              std::uint8_t, std::uint8_t,
                                                              std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_program_start)(peak::HandleType, std::uint8_t *,
                                                         std::uint8_t);

    typedef peak::StatusType(__stdcall *c_program_clear)(peak::HandleType, std::uint8_t,
                                                         std::uint32_t, std::uint8_t *,
                                                         std::uint8_t);

    typedef peak::StatusType(__stdcall *c_program)(peak::HandleType, std::uint8_t, std::uint8_t *,
                                                   std::uint8_t, std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_program_reset)(peak::HandleType, std::uint8_t *,
                                                         std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_program_processor_information)(peak::HandleType,
                                                                             std::uint8_t *,
                                                                             std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_sector_information)(peak::HandleType, std::uint8_t,
                                                                  std::uint8_t, std::uint8_t *,
                                                                  std::uint8_t);

    typedef peak::StatusType(__stdcall *c_program_prepare)(peak::HandleType, std::uint16_t,
                                                           std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_program_format)(peak::HandleType, peak::ProgramFormatType,
                                                          std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_program_next)(peak::HandleType, std::uint8_t,
                                                        std::uint8_t *, std::uint8_t,
                                                        std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_program_max)(peak::HandleType, std::uint8_t *,
                                                       std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_program_verify)(peak::HandleType, std::uint8_t,
                                                          std::uint16_t, std::uint32_t,
                                                          std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_build_checksum)(peak::HandleType, std::uint32_t,
                                                          std::uint8_t *, std::uint8_t);

    typedef peak::StatusType(__stdcall *c_get_error_text)(peak::StatusType, std::uint8_t *);

    c_initialize_can_channel _f_xcp_initialize_can_channel;
    c_initialize_can_channel_fd _f_xcp_initialize_can_channel_fd;
    c_uninitialize_channel _f_xcp_uninitialize_channel;
    c_add_slave_on_can _f_xcp_add_slave_on_can;
    c_remove_slave _f_xcp_remove_slave;
    c_dequeue_packet _f_xcp_dequeue_packet;
    c_reset_queue _f_xcp_reset_queue;

    c_connect _f_xcp_connect;
    c_disconnect _f_xcp_disconnect;

    c_get_status _f_xcp_get_status;
    c_synchronize _f_xcp_synchronize;
    c_get_communication_mode _f_xcp_get_communication_mode;
    c_get_identification _f_xcp_get_identification;
    c_set_request _f_xcp_set_request;
    c_get_seed _f_xcp_get_seed;
    c_unlock _f_xcp_unlock;
    c_transport_layer_command _f_xcp_transport_layer_command;
    c_user_command _f_xcp_user_command;

    c_set_memory_transfer_address _f_xcp_set_memory_transfer_address;
    c_upload _f_xcp_upload;
    c_short_upload _f_xcp_short_upload;
    c_download _f_xcp_download;
    c_download_next _f_xcp_download_next;
    c_download_max _f_xcp_download_max;
    c_modify_bits _f_xcp_modify_bits;

    c_set_calibration_page _f_xcp_set_calibration_page;
    c_get_calibration_page _f_xcp_get_calibration_page;
    c_get_paging_processor_information _f_xcp_get_paging_processor_information;
    c_get_segment_information _f_xcp_get_segment_information;
    c_get_page_information _f_xcp_get_page_information;
    c_set_segment_mode _f_xcp_set_segment_mode;
    c_get_segment_mode _f_xcp_get_segment_mode;
    c_copy_calibration_page _f_xcp_copy_calibration_page;

    c_set_daq_list_pointer _f_xcp_set_daq_list_pointer;
    c_write_daq_list_entry _f_xcp_write_daq_list_entry;
    c_write_daq_list_entries _f_xcp_write_daq_list_entries;
    c_set_daq_list_mode _f_xcp_set_daq_list_mode;
    c_start_stop_daq_list _f_xcp_start_stop_daq_list;
    c_start_stop_synchronized_daq_list _f_xcp_start_stop_synchronized_daq_list;
    c_read_daq_list_entry _f_xcp_read_daq_list_entry;
    c_get_daq_clock _f_xcp_get_daq_clock;
    c_get_daq_processor_information _f_xcp_get_daq_processor_information;
    c_get_daq_resolution_information _f_xcp_get_daq_resolution_information;
    c_get_daq_list_mode _f_xcp_get_daq_list_mode;
    c_get_event_channel_information _f_xcp_get_event_channel_information;
    c_clear_daq_list _f_xcp_clear_daq_list;
    c_get_daq_list_information _f_xcp_get_daq_list_information;
    c_free_daq_lists _f_xcp_free_daq_lists;
    c_allocate_daq_lists _f_xcp_allocate_daq_lists;
    c_allocate_odt _f_xcp_allocate_odt;
    c_allocate_odt_entry _f_xcp_allocate_odt_entry;

    c_program_start _f_xcp_program_start;
    c_program_clear _f_xcp_program_clear;
    c_program _f_xcp_program;
    c_program_reset _f_xcp_program_reset;
    c_get_program_processor_information _f_xcp_get_program_processor_information;
    c_get_sector_information _f_xcp_get_sector_information;
    c_program_prepare _f_xcp_program_prepare;
    c_program_format _f_xcp_program_format;
    c_program_next _f_xcp_program_next;
    c_program_max _f_xcp_program_max;
    c_program_verify _f_xcp_program_verify;
    c_build_checksum _f_xcp_build_checksum;
    c_get_error_text _f_xcp_get_error_text;

    peak::HandleType handle_ = 0;

public:

    PEAKAPI();

    const peak::HandleType &get_handle() const override
    {
        return handle_;
    }

    peak::StatusType initialize_can_channel(peak::ChannelXCPType *xcp_channel,
                                            peak::ChannelType channel,
                                            peak::BaudRateType baud_rate,
                                            peak::HardwareType hardware_type,
                                            std::uint32_t io_port,
                                            std::uint16_t interrupt) override;

    peak::StatusType initialize_can_channel(peak::ChannelXCPType *xcp_channel,
                                            peak::ChannelType channel,
                                            peak::BaudRateFDType baud_rate) override;

    peak::StatusType uninitialize_channel(peak::ChannelXCPType xcp_channel) override;

    peak::StatusType add_slave_on_can(peak::ChannelXCPType xcp_channel,
                                      peak::TransportCANType layer_data,
                                      peak::ProtocolType protocol,
                                      peak::HandleType *xcp_handle) override;

    peak::StatusType remove_slave(peak::HandleType xcp_handle) override;

    peak::StatusType dequeue_packet(peak::HandleType xcp_handle,
                                    peak::QueueType queue,
                                    std::uint8_t *packet_buffer,
                                    std::uint16_t packet_buffer_length) override;

    peak::StatusType reset_queue(peak::HandleType xcp_handle, peak::QueueType queue) override;

    peak::StatusType connect(peak::HandleType xcp_handle,
                             std::uint8_t mode,
                             std::uint8_t *cto_buffer,
                             std::uint8_t cto_buffer_length) override;

    peak::StatusType disconnect(peak::HandleType xcp_handle,
                                std::uint8_t *cto_buffer,
                                std::uint8_t cto_buffer_length) override;

    peak::StatusType get_status(peak::HandleType xcp_handle,
                                std::uint8_t *cto_buffer,
                                std::uint8_t cto_buffer_length) override;

    peak::StatusType synchronize(peak::HandleType xcp_handle,
                                 std::uint8_t *cto_buffer,
                                 std::uint8_t cto_buffer_length) override;

    peak::StatusType get_communication_mode(peak::HandleType xcp_handle,
                                            std::uint8_t *cto_buffer,
                                            std::uint8_t cto_buffer_length) override;

    peak::StatusType get_identification(peak::HandleType xcp_handle,
                                        std::uint8_t type,
                                        std::uint8_t *cto_buffer,
                                        std::uint8_t cto_buffer_length) override;

    peak::StatusType set_request(peak::HandleType xcp_handle,
                                 std::uint8_t mode,
                                 std::uint16_t session_id,
                                 std::uint8_t *cto_buffer,
                                 std::uint8_t cto_buffer_length) override;

    peak::StatusType get_seed(peak::HandleType xcp_handle,
                              std::uint8_t mode,
                              std::uint8_t resource,
                              std::uint8_t *cto_buffer,
                              std::uint8_t cto_buffer_length) override;

    peak::StatusType unlock(peak::HandleType xcp_handle,
                            std::uint8_t key_length,
                            std::uint8_t *key,
                            std::uint8_t *cto_buffer,
                            std::uint8_t cto_buffer_length) override;

    peak::StatusType transport_layer_command(peak::HandleType xcp_handle,
                                             std::uint8_t sub_command,
                                             std::uint8_t *parameter_buffer,
                                             std::uint16_t parameter_buffer_length,
                                             std::uint8_t *cto_buffer,
                                             std::uint8_t cto_buffer_length) override;

    peak::StatusType user_command(peak::HandleType xcp_handle,
                                  std::uint8_t sub_command,
                                  std::uint8_t *parameter_buffer,
                                  std::uint16_t parameter_buffer_length,
                                  std::uint8_t *cto_buffer,
                                  std::uint8_t cto_buffer_length) override;

    peak::StatusType set_memory_transfer_address(peak::HandleType xcp_handle,
                                                 std::uint8_t address_extension,
                                                 std::uint32_t address,
                                                 std::uint8_t *cto_buffer,
                                                 std::uint8_t cto_buffer_length) override;

    peak::StatusType upload(peak::HandleType xcp_handle,
                            std::uint8_t number_of_elements,
                            std::uint8_t *cto_buffer,
                            std::uint8_t cto_buffer_length) override;

    peak::StatusType short_upload(peak::HandleType xcp_handle,
                                  std::uint8_t number_of_elements,
                                  std::uint8_t address_extension,
                                  std::uint32_t address,
                                  std::uint8_t *cto_buffer,
                                  std::uint8_t cto_buffer_length) override;

    peak::StatusType download(peak::HandleType xcp_handle,
                              std::uint8_t number_of_elements,
                              std::uint8_t *data_buffer,
                              std::uint8_t data_buffer_length,
                              std::uint8_t *cto_buffer,
                              std::uint8_t cto_buffer_length) override;

    peak::StatusType download_next(peak::HandleType xcp_handle,
                                   std::uint8_t number_of_elements,
                                   std::uint8_t *data_buffer,
                                   std::uint8_t data_buffer_length,
                                   std::uint8_t *cto_buffer,
                                   std::uint8_t cto_buffer_length) override;

    peak::StatusType download_max(peak::HandleType xcp_handle,
                                  std::uint8_t *data_buffer,
                                  std::uint8_t *cto_buffer,
                                  std::uint8_t cto_buffer_length) override;

    peak::StatusType modify_bits(peak::HandleType xcp_handle,
                                 std::uint8_t shift_value,
                                 std::uint16_t and_mask,
                                 std::uint16_t xor_mask,
                                 std::uint8_t *cto_buffer,
                                 std::uint8_t cto_buffer_length) override;

    peak::StatusType set_calibration_page(peak::HandleType xcp_handle,
                                          std::uint8_t mode,
                                          peak::CalibrationPageType page,
                                          std::uint8_t *cto_buffer,
                                          std::uint8_t cto_buffer_length) override;

    peak::StatusType get_calibration_page(peak::HandleType xcp_handle,
                                          std::uint8_t mode,
                                          std::uint8_t data_segment_number,
                                          std::uint8_t *cto_buffer,
                                          std::uint8_t cto_buffer_length) override;

    peak::StatusType get_paging_processor_information(peak::HandleType xcp_handle,
                                                      std::uint8_t *cto_buffer,
                                                      std::uint8_t cto_buffer_length) override;

    peak::StatusType get_segment_information(peak::HandleType xcp_handle,
                                             std::uint8_t mode,
                                             std::uint8_t segment_number,
                                             std::uint8_t segment_info,
                                             std::uint8_t mapping_index,
                                             std::uint8_t *cto_buffer,
                                             std::uint8_t cto_buffer_length) override;

    peak::StatusType get_page_information(peak::HandleType xcp_handle,
                                          peak::CalibrationPageType page,
                                          std::uint8_t *cto_buffer,
                                          std::uint8_t cto_buffer_length) override;

    peak::StatusType set_segment_mode(peak::HandleType xcp_handle,
                                      std::uint8_t mode,
                                      std::uint8_t segment_number,
                                      std::uint8_t *cto_buffer,
                                      std::uint8_t cto_buffer_length) override;

    peak::StatusType get_segment_mode(peak::HandleType xcp_handle,
                                      std::uint8_t segment_number,
                                      std::uint8_t *cto_buffer,
                                      std::uint8_t cto_buffer_length) override;

    peak::StatusType copy_calibration_page(peak::HandleType xcp_handle,
                                           peak::CalibrationPageType source,
                                           peak::CalibrationPageType destination,
                                           std::uint8_t *cto_buffer,
                                           std::uint8_t cto_buffer_length) override;

    peak::StatusType set_daq_list_pointer(peak::HandleType xcp_handle,
                                          std::uint16_t daq_list_number,
                                          std::uint8_t odt_number,
                                          std::uint8_t odt_entry,
                                          std::uint8_t *cto_buffer,
                                          std::uint8_t cto_buffer_length) override;

    peak::StatusType write_daq_list_entry(peak::HandleType xcp_handle,
                                          peak::ODTEntryType entry,
                                          std::uint8_t *cto_buffer,
                                          std::uint8_t cto_buffer_length) override;

    peak::StatusType write_daq_list_entries(peak::HandleType xcp_handle,
                                            std::uint8_t number_of_elements,
                                            peak::ODTEntryType *elements,
                                            std::uint8_t *cto_buffer,
                                            std::uint8_t cto_buffer_length) override;

    peak::StatusType set_daq_list_mode(peak::HandleType xcp_handle,
                                       std::uint8_t mode,
                                       peak::DAQListConfigType configuration,
                                       std::uint8_t *cto_buffer,
                                       std::uint8_t cto_buffer_length) override;

    peak::StatusType start_stop_daq_list(peak::HandleType xcp_handle,
                                         std::uint8_t mode,
                                         std::uint16_t daq_list_number,
                                         std::uint8_t *cto_buffer,
                                         std::uint8_t cto_buffer_length) override;

    peak::StatusType start_stop_synchronized_daq_list(peak::HandleType xcp_handle,
                                                      std::uint8_t mode,
                                                      std::uint8_t *cto_buffer,
                                                      std::uint8_t cto_buffer_length) override;

    peak::StatusType read_daq_list_entry(peak::HandleType xcp_handle,
                                         std::uint8_t *cto_buffer,
                                         std::uint8_t cto_buffer_length) override;

    peak::StatusType get_daq_clock(peak::HandleType xcp_handle,
                                   std::uint8_t *cto_buffer,
                                   std::uint8_t cto_buffer_length) override;

    peak::StatusType get_daq_processor_information(peak::HandleType xcp_handle,
                                                   std::uint8_t *cto_buffer,
                                                   std::uint8_t cto_buffer_length) override;

    peak::StatusType get_daq_resolution_information(peak::HandleType xcp_handle,
                                                    std::uint8_t *cto_buffer,
                                                    std::uint8_t cto_buffer_length) override;

    peak::StatusType get_daq_list_mode(peak::HandleType xcp_handle,
                                       std::uint16_t daq_list_number,
                                       std::uint8_t *cto_buffer,
                                       std::uint8_t cto_buffer_length) override;

    peak::StatusType get_event_channel_information(peak::HandleType xcp_handle,
                                                   std::uint16_t event_channel_number,
                                                   std::uint8_t *cto_buffer,
                                                   std::uint8_t cto_buffer_length) override;

    peak::StatusType clear_daq_list(peak::HandleType xcp_handle,
                                    std::uint16_t daq_list_number,
                                    std::uint8_t *cto_buffer,
                                    std::uint8_t cto_buffer_length) override;

    peak::StatusType get_daq_list_information(peak::HandleType xcp_handle,
                                              std::uint16_t daq_list_number,
                                              std::uint8_t *cto_buffer,
                                              std::uint8_t cto_buffer_length) override;

    peak::StatusType free_daq_lists(peak::HandleType xcp_handle,
                                    std::uint8_t *cto_buffer,
                                    std::uint8_t cto_buffer_length) override;

    peak::StatusType allocate_daq_lists(peak::HandleType xcp_handle,
                                        std::uint16_t daq_count,
                                        std::uint8_t *cto_buffer,
                                        std::uint8_t cto_buffer_length) override;

    peak::StatusType allocate_odt(peak::HandleType xcp_handle,
                                  std::uint16_t daq_list_number,
                                  std::uint8_t odt_count,
                                  std::uint8_t *cto_buffer,
                                  std::uint8_t cto_buffer_length) override;

    peak::StatusType allocate_odt_entry(peak::HandleType xcp_handle,
                                        std::uint16_t daq_list_number,
                                        std::uint8_t odt_number,
                                        std::uint8_t entries_count,
                                        std::uint8_t *cto_buffer,
                                        std::uint8_t cto_buffer_length) override;

    peak::StatusType program_start(peak::HandleType xcp_handle,
                                   std::uint8_t *cto_buffer,
                                   std::uint8_t cto_buffer_length) override;

    peak::StatusType program_clear(peak::HandleType xcp_handle,
                                   std::uint8_t mode,
                                   std::uint32_t clear_range,
                                   std::uint8_t *cto_buffer,
                                   std::uint8_t cto_buffer_length) override;

    peak::StatusType program(peak::HandleType xcp_handle,
                             std::uint8_t number_of_elements,
                             std::uint8_t *data_buffer,
                             std::uint8_t data_buffer_length,
                             std::uint8_t *cto_buffer,
                             std::uint8_t cto_buffer_length) override;

    peak::StatusType program_reset(peak::HandleType xcp_handle,
                                   std::uint8_t *cto_buffer,
                                   std::uint8_t cto_buffer_length) override;

    peak::StatusType get_program_processor_information(peak::HandleType xcp_handle,
                                                       std::uint8_t *cto_buffer,
                                                       std::uint8_t cto_buffer_length) override;

    peak::StatusType get_sector_information(peak::HandleType xcp_handle,
                                            std::uint8_t mode,
                                            std::uint8_t sector_number,
                                            std::uint8_t *cto_buffer,
                                            std::uint8_t cto_buffer_length) override;

    peak::StatusType program_prepare(peak::HandleType xcp_handle,
                                     std::uint16_t code_size,
                                     std::uint8_t *cto_buffer,
                                     std::uint8_t cto_buffer_length) override;

    peak::StatusType program_format(peak::HandleType xcp_handle,
                                    peak::ProgramFormatType format,
                                    std::uint8_t *cto_buffer,
                                    std::uint8_t cto_buffer_length) override;

    peak::StatusType program_next(peak::HandleType xcp_handle,
                                  std::uint8_t number_of_elements,
                                  std::uint8_t *data_buffer,
                                  std::uint8_t data_length,
                                  std::uint8_t *cto_buffer,
                                  std::uint8_t cto_buffer_length) override;

    peak::StatusType program_max(peak::HandleType xcp_handle,
                                 std::uint8_t *element_buffer,
                                 std::uint8_t *cto_buffer,
                                 std::uint8_t cto_buffer_length) override;

    peak::StatusType program_verify(peak::HandleType xcp_handle,
                                    std::uint8_t mode,
                                    std::uint16_t type,
                                    std::uint32_t value,
                                    std::uint8_t *cto_buffer,
                                    std::uint8_t cto_buffer_length) override;

    peak::StatusType build_checksum(peak::HandleType xcp_handle,
                                    std::uint32_t block_size,
                                    std::uint8_t *cto_buffer,
                                    std::uint8_t cto_buffer_length) override;

    peak::StatusType
    get_error_text(peak::StatusType error_code, std::uint8_t *text_buffer) override;
};

#endif /* #ifndef XCPUI_PEAK_API_H */
