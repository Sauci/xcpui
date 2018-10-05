//
// Created by sauci on 15/11/2018.
//

#ifndef XCPUI_PEAK_API_MOCK_H
#define XCPUI_PEAK_API_MOCK_H

#include <gmock/gmock.h>

#include "peak_api_interface.h"

struct PEAKAPIMockImpl : public PEAKAPIInterface
{
    virtual const peak::HandleType &get_handle() const
    {
        static peak::HandleType handle_ = 0;
        return handle_;
    }

    virtual peak::StatusType initialize_can_channel(peak::ChannelXCPType *xcp_channel,
                                                    peak::ChannelType channel,
                                                    peak::BaudRateType baud_rate,
                                                    peak::HardwareType hardware_type,
                                                    std::uint32_t io_port, std::uint16_t interrupt)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType initialize_can_channel(peak::ChannelXCPType *xcp_channel,
                                                    peak::ChannelType channel,
                                                    peak::BaudRateFDType baud_rate)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType uninitialize_channel(peak::ChannelXCPType xcp_channel)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType add_slave_on_can(peak::ChannelXCPType xcp_channel,
                                              peak::TransportCANType layer_data,
                                              peak::ProtocolType protocol,
                                              peak::HandleType *xcp_handle)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType remove_slave(peak::HandleType xcp_handle)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType dequeue_packet(peak::HandleType xcp_handle,
                                            peak::QueueType queue,
                                            std::uint8_t *packet_buffer,
                                            std::uint16_t packet_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType reset_queue(peak::HandleType xcp_handle, peak::QueueType queue)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType connect(peak::HandleType xcp_handle, std::uint8_t mode,
                                     std::uint8_t *cto_buffer, std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType disconnect(peak::HandleType xcp_handle,
                                        std::uint8_t *cto_buffer,
                                        std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_status(peak::HandleType xcp_handle,
                                        std::uint8_t *cto_buffer,
                                        std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType synchronize(peak::HandleType xcp_handle,
                                         std::uint8_t *cto_buffer,
                                         std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_communication_mode(peak::HandleType xcp_handle,
                                                    std::uint8_t *cto_buffer,
                                                    std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_identification(peak::HandleType xcp_handle,
                                                std::uint8_t type,
                                                std::uint8_t *cto_buffer,
                                                std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType set_request(peak::HandleType xcp_handle,
                                         std::uint8_t mode,
                                         std::uint16_t session_id,
                                         std::uint8_t *cto_buffer,
                                         std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_seed(peak::HandleType xcp_handle, std::uint8_t mode,
                                      std::uint8_t resource, std::uint8_t *cto_buffer,
                                      std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType unlock(peak::HandleType xcp_handle, std::uint8_t key_length,
                                    std::uint8_t *key, std::uint8_t *cto_buffer,
                                    std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType transport_layer_command(peak::HandleType xcp_handle,
                                                     std::uint8_t sub_command,
                                                     std::uint8_t *parameter_buffer,
                                                     std::uint16_t parameter_buffer_length,
                                                     std::uint8_t *cto_buffer,
                                                     std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType user_command(peak::HandleType xcp_handle,
                                          std::uint8_t sub_command,
                                          std::uint8_t *parameter_buffer,
                                          std::uint16_t parameter_buffer_length,
                                          std::uint8_t *cto_buffer,
                                          std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType set_memory_transfer_address(peak::HandleType xcp_handle,
                                                         std::uint8_t address_extension,
                                                         std::uint32_t address,
                                                         std::uint8_t *cto_buffer,
                                                         std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType upload(peak::HandleType xcp_handle,
                                    std::uint8_t number_of_elements,
                                    std::uint8_t *cto_buffer,
                                    std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType short_upload(peak::HandleType xcp_handle,
                                          std::uint8_t number_of_elements,
                                          std::uint8_t address_extension, std::uint32_t address,
                                          std::uint8_t *cto_buffer, std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType download(peak::HandleType xcp_handle, std::uint8_t number_of_elements,
                                      std::uint8_t *data_buffer, std::uint8_t data_buffer_length,
                                      std::uint8_t *cto_buffer, std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType download_next(peak::HandleType xcp_handle,
                                           std::uint8_t number_of_elements,
                                           std::uint8_t *data_buffer,
                                           std::uint8_t data_buffer_length,
                                           std::uint8_t *cto_buffer, std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType download_max(peak::HandleType xcp_handle,
                                          std::uint8_t *data_buffer,
                                          std::uint8_t *cto_buffer,
                                          std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType modify_bits(peak::HandleType xcp_handle, std::uint8_t shift_value,
                                         std::uint16_t and_mask, std::uint16_t xor_mask,
                                         std::uint8_t *cto_buffer, std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType set_calibration_page(peak::HandleType xcp_handle, std::uint8_t mode,
                                                  peak::CalibrationPageType page,
                                                  std::uint8_t *cto_buffer,
                                                  std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_calibration_page(peak::HandleType xcp_handle, std::uint8_t mode,
                                                  std::uint8_t data_segment_number,
                                                  std::uint8_t *cto_buffer,
                                                  std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_paging_processor_information(peak::HandleType xcp_handle,
                                                              std::uint8_t *cto_buffer,
                                                              std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_segment_information(peak::HandleType xcp_handle, std::uint8_t mode,
                                                     std::uint8_t segment_number,
                                                     std::uint8_t segment_info,
                                                     std::uint8_t mapping_index,
                                                     std::uint8_t *cto_buffer,
                                                     std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_page_information(peak::HandleType xcp_handle,
                                                  peak::CalibrationPageType page,
                                                  std::uint8_t *cto_buffer,
                                                  std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType set_segment_mode(peak::HandleType xcp_handle,
                                              std::uint8_t mode,
                                              std::uint8_t segment_number,
                                              std::uint8_t *cto_buffer,
                                              std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_segment_mode(peak::HandleType xcp_handle,
                                              std::uint8_t segment_number,
                                              std::uint8_t *cto_buffer,
                                              std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType copy_calibration_page(peak::HandleType xcp_handle,
                                                   peak::CalibrationPageType source,
                                                   peak::CalibrationPageType destination,
                                                   std::uint8_t *cto_buffer,
                                                   std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType set_daq_list_pointer(peak::HandleType xcp_handle,
                                                  std::uint16_t daq_list_number,
                                                  std::uint8_t odt_number, std::uint8_t odt_entry,
                                                  std::uint8_t *cto_buffer,
                                                  std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType write_daq_list_entry(peak::HandleType xcp_handle,
                                                  peak::ODTEntryType entry,
                                                  std::uint8_t *cto_buffer,
                                                  std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType write_daq_list_entries(peak::HandleType xcp_handle,
                                                    std::uint8_t number_of_elements,
                                                    peak::ODTEntryType *elements,
                                                    std::uint8_t *cto_buffer,
                                                    std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType set_daq_list_mode(peak::HandleType xcp_handle, std::uint8_t mode,
                                               peak::DAQListConfigType configuration,
                                               std::uint8_t *cto_buffer,
                                               std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType start_stop_daq_list(peak::HandleType xcp_handle,
                                                 std::uint8_t mode,
                                                 std::uint16_t daq_list_number,
                                                 std::uint8_t *cto_buffer,
                                                 std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType start_stop_synchronized_daq_list(peak::HandleType xcp_handle,
                                                              std::uint8_t mode,
                                                              std::uint8_t *cto_buffer,
                                                              std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType read_daq_list_entry(peak::HandleType xcp_handle,
                                                 std::uint8_t *cto_buffer,
                                                 std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_daq_clock(peak::HandleType xcp_handle,
                                           std::uint8_t *cto_buffer,
                                           std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_daq_processor_information(peak::HandleType xcp_handle,
                                                           std::uint8_t *cto_buffer,
                                                           std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_daq_resolution_information(peak::HandleType xcp_handle,
                                                            std::uint8_t *cto_buffer,
                                                            std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_daq_list_mode(peak::HandleType xcp_handle,
                                               std::uint16_t daq_list_number,
                                               std::uint8_t *cto_buffer,
                                               std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_event_channel_information(peak::HandleType xcp_handle,
                                                           std::uint16_t event_channel_number,
                                                           std::uint8_t *cto_buffer,
                                                           std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType clear_daq_list(peak::HandleType xcp_handle,
                                            std::uint16_t daq_list_number,
                                            std::uint8_t *cto_buffer,
                                            std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_daq_list_information(peak::HandleType xcp_handle,
                                                      std::uint16_t daq_list_number,
                                                      std::uint8_t *cto_buffer,
                                                      std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType free_daq_lists(peak::HandleType xcp_handle,
                                            std::uint8_t *cto_buffer,
                                            std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType allocate_daq_lists(peak::HandleType xcp_handle,
                                                std::uint16_t daq_count,
                                                std::uint8_t *cto_buffer,
                                                std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType allocate_odt(peak::HandleType xcp_handle,
                                          std::uint16_t daq_list_number,
                                          std::uint8_t odt_count,
                                          std::uint8_t *cto_buffer,
                                          std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType allocate_odt_entry(peak::HandleType xcp_handle,
                                                std::uint16_t daq_list_number,
                                                std::uint8_t odt_number, std::uint8_t entries_count,
                                                std::uint8_t *cto_buffer,
                                                std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType program_start(peak::HandleType xcp_handle,
                                           std::uint8_t *cto_buffer,
                                           std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType program_clear(peak::HandleType xcp_handle,
                                           std::uint8_t mode,
                                           std::uint32_t clear_range,
                                           std::uint8_t *cto_buffer,
                                           std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType program(peak::HandleType xcp_handle, std::uint8_t number_of_elements,
                                     std::uint8_t *data_buffer, std::uint8_t data_buffer_length,
                                     std::uint8_t *cto_buffer, std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType program_reset(peak::HandleType xcp_handle,
                                           std::uint8_t *cto_buffer,
                                           std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_program_processor_information(peak::HandleType xcp_handle,
                                                               std::uint8_t *cto_buffer,
                                                               std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_sector_information(peak::HandleType xcp_handle, std::uint8_t mode,
                                                    std::uint8_t sector_number,
                                                    std::uint8_t *cto_buffer,
                                                    std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType program_prepare(peak::HandleType xcp_handle,
                                             std::uint16_t code_size,
                                             std::uint8_t *cto_buffer,
                                             std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType program_format(peak::HandleType xcp_handle,
                                            peak::ProgramFormatType format,
                                            std::uint8_t *cto_buffer,
                                            std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType program_next(peak::HandleType xcp_handle,
                                          std::uint8_t number_of_elements,
                                          std::uint8_t *data_buffer, std::uint8_t data_length,
                                          std::uint8_t *cto_buffer, std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType program_max(peak::HandleType xcp_handle,
                                         std::uint8_t *element_buffer,
                                         std::uint8_t *cto_buffer,
                                         std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType program_verify(peak::HandleType xcp_handle, std::uint8_t mode,
                                            std::uint16_t type, std::uint32_t value,
                                            std::uint8_t *cto_buffer,
                                            std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType build_checksum(peak::HandleType xcp_handle,
                                            std::uint32_t block_size,
                                            std::uint8_t *cto_buffer,
                                            std::uint8_t cto_buffer_length)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }

    virtual peak::StatusType get_error_text(peak::StatusType error_code, std::uint8_t *text_buffer)
    {
        return peak::StatusType::DLL_XCP_ERR_OK;
    }
};

struct PEAKAPIMock : public PEAKAPIMockImpl
{
    MOCK_METHOD0(get_handle, peak::HandleType &(void));

    MOCK_METHOD6(initialize_can_channel,
                 peak::StatusType(peak::ChannelXCPType *, peak::ChannelType, peak::BaudRateType,
                                  peak::HardwareType, std::uint32_t, std::uint16_t));

    MOCK_METHOD3(initialize_can_channel,
                 peak::StatusType(peak::ChannelXCPType *, peak::ChannelType, peak::BaudRateFDType));

    MOCK_METHOD1(uninitialize_channel, peak::StatusType(peak::ChannelXCPType));

    MOCK_METHOD4(add_slave_on_can,
                 peak::StatusType(peak::ChannelXCPType, peak::TransportCANType, peak::ProtocolType,
                                  peak::HandleType *));

    MOCK_METHOD1(remove_slave, peak::StatusType(peak::HandleType));

    MOCK_METHOD4(dequeue_packet,
                 peak::StatusType(peak::HandleType, peak::QueueType, std::uint8_t *,
                                  std::uint16_t));

    MOCK_METHOD2(reset_queue, peak::StatusType(peak::HandleType, peak::QueueType));

    MOCK_METHOD4(connect,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t *, std::uint8_t));

    MOCK_METHOD3(disconnect, peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD3(get_status, peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD3(synchronize, peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD3(get_communication_mode,
                 peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD4(get_identification,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t *, std::uint8_t));

    MOCK_METHOD5(set_request,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint16_t, std::uint8_t *,
                                  std::uint8_t));

    MOCK_METHOD5(get_seed,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t, std::uint8_t *,
                                  std::uint8_t));

    MOCK_METHOD5(unlock,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t, std::uint8_t *,
                                  std::uint8_t));

    MOCK_METHOD6(transport_layer_command,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t, std::uint16_t,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD6(user_command,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t, std::uint16_t,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD4(upload,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t *, std::uint8_t));

    MOCK_METHOD6(short_upload,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t, std::uint16_t,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD6(download,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t *, std::uint8_t,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD6(download_next,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t *, std::uint8_t,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD4(get_status,
                 peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t *, std::uint8_t));

    MOCK_METHOD6(modify_bits,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint16_t, std::uint16_t,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD5(set_calibration_page,
                 peak::StatusType(peak::HandleType, std::uint8_t, peak::CalibrationPageType,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD5(get_calibration_page,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t, std::uint8_t *,
                                  std::uint8_t));

    MOCK_METHOD3(get_paging_processor_information,
                 peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD7(get_segment_information,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t, std::uint8_t,
                                  std::uint8_t, std::uint8_t *, std::uint8_t));

    MOCK_METHOD4(get_page_information,
                 peak::StatusType(peak::HandleType, peak::CalibrationPageType, std::uint8_t *,
                                  std::uint8_t));

    MOCK_METHOD5(set_segment_mode,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t, std::uint8_t *,
                                  std::uint8_t));

    MOCK_METHOD4(get_segment_mode,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t *, std::uint8_t));

    MOCK_METHOD5(copy_calibration_page,
                 peak::StatusType(peak::HandleType, peak::CalibrationPageType,
                                  peak::CalibrationPageType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD6(set_daq_list_pointer,
                 peak::StatusType(peak::HandleType, std::uint16_t, std::uint8_t, std::uint8_t,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD4(write_daq_list_entry,
                 peak::StatusType(peak::HandleType, peak::ODTEntryType, std::uint8_t *,
                                  std::uint8_t));

    MOCK_METHOD5(write_daq_list_entries,
                 peak::StatusType(peak::HandleType, std::uint8_t, peak::ODTEntryType *,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD5(set_daq_list_mode,
                 peak::StatusType(peak::HandleType, std::uint8_t, peak::DAQListConfigType,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD5(start_stop_daq_list,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint16_t, std::uint8_t *,
                                  std::uint8_t));

    MOCK_METHOD4(start_stop_synchronized_daq_list,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t *, std::uint8_t));

    MOCK_METHOD3(read_daq_list_entry,
                 peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD3(get_daq_clock, peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD3(get_daq_processor_information,
                 peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD3(get_daq_resolution_information,
                 peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD4(get_daq_list_mode,
                 peak::StatusType(peak::HandleType, std::uint16_t, std::uint8_t *, std::uint8_t));

    MOCK_METHOD4(get_event_channel_information,
                 peak::StatusType(peak::HandleType, std::uint16_t, std::uint8_t *, std::uint8_t));

    MOCK_METHOD4(clear_daq_list,
                 peak::StatusType(peak::HandleType, std::uint16_t, std::uint8_t *, std::uint8_t));

    MOCK_METHOD4(get_daq_list_information,
                 peak::StatusType(peak::HandleType, std::uint16_t, std::uint8_t *, std::uint8_t));

    MOCK_METHOD3(free_daq_lists, peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD4(allocate_daq_lists,
                 peak::StatusType(peak::HandleType, std::uint16_t, std::uint8_t *, std::uint8_t));

    MOCK_METHOD5(allocate_odt,
                 peak::StatusType(peak::HandleType, std::uint16_t, std::uint8_t, std::uint8_t *,
                                  std::uint8_t));

    MOCK_METHOD6(allocate_odt_entry,
                 peak::StatusType(peak::HandleType, std::uint16_t, std::uint8_t, std::uint8_t,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD3(program_start, peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD5(program_clear,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint32_t, std::uint8_t *,
                                  std::uint8_t));

    MOCK_METHOD6(program,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t *, std::uint8_t,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD3(program_reset, peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD3(get_program_processor_information,
                 peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t));

    MOCK_METHOD5(get_sector_information,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t, std::uint8_t *,
                                  std::uint8_t));

    MOCK_METHOD4(program_prepare,
                 peak::StatusType(peak::HandleType, std::uint16_t, std::uint8_t *, std::uint8_t));

    MOCK_METHOD4(program_format,
                 peak::StatusType(peak::HandleType, peak::ProgramFormatType, std::uint8_t *,
                                  std::uint8_t));

    MOCK_METHOD6(program_next,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint8_t *, std::uint8_t,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD4(program_max,
                 peak::StatusType(peak::HandleType, std::uint8_t *, std::uint8_t *, std::uint8_t));

    MOCK_METHOD6(program_verify,
                 peak::StatusType(peak::HandleType, std::uint8_t, std::uint16_t, std::uint32_t,
                                  std::uint8_t *, std::uint8_t));

    MOCK_METHOD4(build_checksum,
                 peak::StatusType(peak::HandleType, std::uint32_t, std::uint8_t *, std::uint8_t));

    MOCK_METHOD2(get_error_text, peak::StatusType(peak::StatusType, std::uint8_t *));
};

#endif // XCPUI_PEAK_API_MOCK_H
