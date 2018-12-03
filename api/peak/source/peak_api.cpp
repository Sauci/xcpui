//
// Created by sauci on 15/11/2018.
//

#include "peak_api.h"
#include <iostream>
#include <string>

#define MAP_API(fcn, dll_fcn) PEAKAPI::_f_xcp_##fcn = (c_##fcn)GetProcAddress(xcp_library_, dll_fcn)

HINSTANCE PEAKAPI::basic_library_ = nullptr;
HINSTANCE PEAKAPI::isotp_library_ = nullptr;
HINSTANCE PEAKAPI::xcp_library_ = nullptr;

PEAKAPI::PEAKAPI()
{
    (void)basic_library_;
    (void)isotp_library_;

    std::string dir;

#ifdef _WIN64
    dir = std::string("x64");
#else
    dir = std::string("Win32");
#endif

    std::string file_path = __FILE__;
    std::string cd = file_path.substr(0, file_path.rfind("\\")) + std::string(R"(\..\lib\)");
    auto basic_path = cd + std::string("peak_basic\\") + dir + std::string("\\PCANBasic.dll");
    auto isotp_path = cd + std::string("peak_isotp\\") + dir + std::string("\\PCAN-ISO-TP.dll");
    auto xcp_path = cd + std::string("peak_xcp\\") + dir + std::string("\\PXCP.dll");

    basic_library_ = LoadLibrary(basic_path.c_str());
    isotp_library_ = LoadLibrary(isotp_path.c_str());
    xcp_library_ = LoadLibrary(xcp_path.c_str());

    MAP_API(initialize_can_channel, "XCP_InitializeCanChannel");
    MAP_API(initialize_can_channel_fd, "XCP_InitializeCanChannelFD");
    MAP_API(uninitialize_channel, "XCP_UninitializeChannel");
    MAP_API(add_slave_on_can, "XCP_AddSlaveOnCAN");
    MAP_API(remove_slave, "XCP_RemoveSlave");
    MAP_API(dequeue_packet, "XCP_DequeuePacket");
    MAP_API(reset_queue, "XCP_ResetQueue");

    MAP_API(connect, "XCP_Connect");
    MAP_API(disconnect, "XCP_Disconnect");

    MAP_API(get_status, "XCP_GetStatus");
    MAP_API(synchronize, "XCP_Synchronize");
    MAP_API(get_communication_mode, "XCP_GetCommunicationMode");
    MAP_API(get_identification, "XCP_GetIdentification");
    MAP_API(set_request, "XCP_SetRequest");
    MAP_API(get_seed, "XCP_GetSeed");
    MAP_API(unlock, "XCP_Unlock");
    MAP_API(transport_layer_command, "XCP_TransportLayerCommand");
    MAP_API(user_command, "XCP_UserCommand");

    MAP_API(set_memory_transfer_address, "XCP_SetMemoryTransferAddress");
    MAP_API(upload, "XCP_Upload");
    MAP_API(short_upload, "XCP_ShortUpload");
    MAP_API(download, "XCP_Download");
    MAP_API(download_next, "XCP_DownloadNext");
    MAP_API(download_max, "XCP_DownloadMax");
    MAP_API(modify_bits, "XCP_ModifyBits");

    MAP_API(set_calibration_page, "XCP_SetCalibrationPage");
    MAP_API(get_calibration_page, "XCP_GetCalibrationPage");
    MAP_API(get_paging_processor_information, "XCP_GetPagingProcessorInformation");
    MAP_API(get_segment_information, "XCP_GetSegmentInformation");
    MAP_API(get_page_information, "XCP_GetPageInformation");
    MAP_API(set_segment_mode, "XCP_SetSegmentMode");
    MAP_API(get_segment_mode, "XCP_GetSegmentMode");
    MAP_API(copy_calibration_page, "XCP_CopyCalibrationPage");

    MAP_API(set_daq_list_pointer, "XCP_SetDAQListPointer");
    MAP_API(write_daq_list_entry, "XCP_WriteDAQListEntry");
    MAP_API(write_daq_list_entries, "XCP_WriteDAQListEntries");
    MAP_API(set_daq_list_mode, "XCP_SetDAQListMode");
    MAP_API(start_stop_daq_list, "XCP_StartStopDAQList");
    MAP_API(start_stop_synchronized_daq_list, "XCP_StartStopSynchronizedDAQList");
    MAP_API(read_daq_list_entry, "XCP_ReadDAQListEntry");
    MAP_API(get_daq_clock, "XCP_GetDAQClock");
    MAP_API(get_daq_processor_information, "XCP_GetDAQProcessorInformation");
    MAP_API(get_daq_resolution_information, "XCP_GetDAQResolutionInformation");
    MAP_API(get_daq_list_mode, "XCP_GetDAQListMode");
    MAP_API(get_event_channel_information, "XCP_GetEventChannelInformation");
    MAP_API(clear_daq_list, "XCP_ClearDAQList");
    MAP_API(get_daq_list_information, "XCP_GetDAQListInformation");
    MAP_API(free_daq_lists, "XCP_FreeDAQLists");
    MAP_API(allocate_daq_lists, "XCP_AllocateDAQLists");
    MAP_API(allocate_odt, "XCP_AllocateODT");
    MAP_API(allocate_odt_entry, "XCP_AllocateODTEntry");

    MAP_API(program_start, "XCP_ProgramStart");
    MAP_API(program_clear, "XCP_ProgramClear");
    MAP_API(program, "XCP_Program");
    MAP_API(program_reset, "XCP_ProgramReset");
    MAP_API(get_program_processor_information, "XCP_GetPGMProcessorInformation");
    MAP_API(get_sector_information, "XCP_GetSectorInformation");
    MAP_API(program_prepare, "XCP_ProgramPrepare");
    MAP_API(program_format, "XCP_ProgramFormat");
    MAP_API(program_next, "XCP_ProgramNext");
    MAP_API(program_max, "XCP_ProgramMax");
    MAP_API(program_verify, "XCP_ProgramVerify");
    MAP_API(build_checksum, "XCP_BuildChecksum");
    MAP_API(get_error_text, "XCP_GetErrorText");
}

peak::StatusType PEAKAPI::initialize_can_channel(peak::ChannelXCPType *xcp_channel,
                                                 peak::ChannelType channel,
                                                 peak::BaudRateType baud_rate,
                                                 peak::HardwareType hardware_type,
                                                 std::uint32_t io_port,
                                                 std::uint16_t interrupt)
{
    return PEAKAPI::_f_xcp_initialize_can_channel(
        xcp_channel, channel, baud_rate, hardware_type, io_port, interrupt);
}

peak::StatusType PEAKAPI::initialize_can_channel(peak::ChannelXCPType *xcp_channel,
                                                 peak::ChannelType channel,
                                                 peak::BaudRateFDType baud_rate)
{
    return PEAKAPI::_f_xcp_initialize_can_channel_fd(xcp_channel, channel, baud_rate);
}

peak::StatusType PEAKAPI::uninitialize_channel(peak::ChannelXCPType xcp_channel)
{
    return PEAKAPI::_f_xcp_uninitialize_channel(xcp_channel);
}

peak::StatusType PEAKAPI::add_slave_on_can(peak::ChannelXCPType xcp_channel,
                                           peak::TransportCANType layer_data,
                                           peak::ProtocolType protocol,
                                           peak::HandleType *xcp_handle)
{
    return PEAKAPI::_f_xcp_add_slave_on_can(xcp_channel, layer_data, protocol, xcp_handle);
}

peak::StatusType PEAKAPI::remove_slave(peak::HandleType xcp_handle)
{
    return PEAKAPI::_f_xcp_remove_slave(xcp_handle);
}

peak::StatusType PEAKAPI::dequeue_packet(peak::HandleType xcp_handle,
                                         peak::QueueType queue,
                                         std::uint8_t *packet_buffer,
                                         std::uint16_t packet_buffer_length)
{
    return PEAKAPI::_f_xcp_dequeue_packet(xcp_handle, queue, packet_buffer, packet_buffer_length);
}

peak::StatusType PEAKAPI::reset_queue(peak::HandleType xcp_handle, peak::QueueType queue)
{
    return PEAKAPI::_f_xcp_reset_queue(xcp_handle, queue);
}

peak::StatusType PEAKAPI::connect(peak::HandleType xcp_handle,
                                  std::uint8_t mode,
                                  std::uint8_t *cto_buffer,
                                  std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_connect(xcp_handle, mode, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::disconnect(peak::HandleType xcp_handle,
                                     std::uint8_t *cto_buffer,
                                     std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_disconnect(xcp_handle, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::get_status(peak::HandleType xcp_handle,
                                     std::uint8_t *cto_buffer,
                                     std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_status(xcp_handle, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::synchronize(peak::HandleType xcp_handle,
                                      std::uint8_t *cto_buffer,
                                      std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_synchronize(xcp_handle, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::get_communication_mode(peak::HandleType xcp_handle,
                                                 std::uint8_t *cto_buffer,
                                                 std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_communication_mode(xcp_handle, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::get_identification(peak::HandleType xcp_handle,
                                             std::uint8_t type,
                                             std::uint8_t *cto_buffer,
                                             std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_identification(xcp_handle, type, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::set_request(peak::HandleType xcp_handle,
                                      std::uint8_t mode,
                                      std::uint16_t session_id,
                                      std::uint8_t *cto_buffer,
                                      std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_set_request(xcp_handle, mode, session_id, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::get_seed(peak::HandleType xcp_handle,
                                   std::uint8_t mode,
                                   std::uint8_t resource,
                                   std::uint8_t *cto_buffer,
                                   std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_seed(xcp_handle, mode, resource, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::unlock(peak::HandleType xcp_handle,
                                 std::uint8_t key_length,
                                 std::uint8_t *key,
                                 std::uint8_t *cto_buffer,
                                 std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_unlock(xcp_handle, key_length, key, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::transport_layer_command(peak::HandleType xcp_handle,
                                                  std::uint8_t sub_command,
                                                  std::uint8_t *parameter_buffer,
                                                  std::uint16_t parameter_buffer_length,
                                                  std::uint8_t *cto_buffer,
                                                  std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_transport_layer_command(xcp_handle,
                                                   sub_command,
                                                   parameter_buffer,
                                                   parameter_buffer_length,
                                                   cto_buffer,
                                                   cto_buffer_length);
}

peak::StatusType PEAKAPI::user_command(peak::HandleType xcp_handle,
                                       std::uint8_t sub_command,
                                       std::uint8_t *parameter_buffer,
                                       std::uint16_t parameter_buffer_length,
                                       std::uint8_t *cto_buffer,
                                       std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_user_command(xcp_handle,
                                        sub_command,
                                        parameter_buffer,
                                        parameter_buffer_length,
                                        cto_buffer,
                                        cto_buffer_length);
}

peak::StatusType PEAKAPI::set_memory_transfer_address(peak::HandleType xcp_handle,
                                                      std::uint8_t address_extension,
                                                      std::uint32_t address,
                                                      std::uint8_t *cto_buffer,
                                                      std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_set_memory_transfer_address(xcp_handle, address_extension, address,
                                                       cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::upload(peak::HandleType xcp_handle,
                                 std::uint8_t number_of_elements,
                                 std::uint8_t *cto_buffer,
                                 std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_upload(xcp_handle, number_of_elements, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::short_upload(peak::HandleType xcp_handle,
                                       std::uint8_t number_of_elements,
                                       std::uint8_t address_extension,
                                       std::uint32_t address,
                                       std::uint8_t *cto_buffer,
                                       std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_short_upload(
        xcp_handle, number_of_elements, address_extension, address, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::download(peak::HandleType xcp_handle,
                                   std::uint8_t number_of_elements,
                                   std::uint8_t *data_buffer,
                                   std::uint8_t data_buffer_length,
                                   std::uint8_t *cto_buffer,
                                   std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_download(xcp_handle,
                                    number_of_elements,
                                    data_buffer,
                                    data_buffer_length,
                                    cto_buffer,
                                    cto_buffer_length);
}

peak::StatusType PEAKAPI::download_next(peak::HandleType xcp_handle,
                                        std::uint8_t number_of_elements,
                                        std::uint8_t *data_buffer,
                                        std::uint8_t data_buffer_length,
                                        std::uint8_t *cto_buffer,
                                        std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_download_next(xcp_handle,
                                         number_of_elements,
                                         data_buffer,
                                         data_buffer_length,
                                         cto_buffer,
                                         cto_buffer_length);
}

peak::StatusType PEAKAPI::download_max(peak::HandleType xcp_handle,
                                       std::uint8_t *data_buffer,
                                       std::uint8_t *cto_buffer,
                                       std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_download_max(xcp_handle, data_buffer, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::modify_bits(peak::HandleType xcp_handle,
                                      std::uint8_t shift_value,
                                      std::uint16_t and_mask,
                                      std::uint16_t xor_mask,
                                      std::uint8_t *cto_buffer,
                                      std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_modify_bits(
        xcp_handle, shift_value, and_mask, xor_mask, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::set_calibration_page(peak::HandleType xcp_handle,
                                               std::uint8_t mode,
                                               peak::CalibrationPageType page,
                                               std::uint8_t *cto_buffer,
                                               std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_set_calibration_page(
        xcp_handle, mode, page, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::get_calibration_page(peak::HandleType xcp_handle,
                                               std::uint8_t mode,
                                               std::uint8_t data_segment_number,
                                               std::uint8_t *cto_buffer,
                                               std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_calibration_page(
        xcp_handle, mode, data_segment_number, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::get_paging_processor_information(peak::HandleType xcp_handle,
                                                           std::uint8_t *cto_buffer,
                                                           std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_paging_processor_information(xcp_handle,
                                                            cto_buffer,
                                                            cto_buffer_length);
}

peak::StatusType PEAKAPI::get_segment_information(peak::HandleType xcp_handle,
                                                  std::uint8_t mode,
                                                  std::uint8_t segment_number,
                                                  std::uint8_t segment_info,
                                                  std::uint8_t mapping_index,
                                                  std::uint8_t *cto_buffer,
                                                  std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_segment_information(xcp_handle,
                                                   mode,
                                                   segment_number,
                                                   segment_info,
                                                   mapping_index,
                                                   cto_buffer,
                                                   cto_buffer_length);
}

peak::StatusType PEAKAPI::get_page_information(peak::HandleType xcp_handle,
                                               peak::CalibrationPageType page,
                                               std::uint8_t *cto_buffer,
                                               std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_page_information(xcp_handle, page, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::set_segment_mode(peak::HandleType xcp_handle,
                                           std::uint8_t mode,
                                           std::uint8_t segment_number,
                                           std::uint8_t *cto_buffer,
                                           std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_set_segment_mode(
        xcp_handle, mode, segment_number, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::get_segment_mode(peak::HandleType xcp_handle,
                                           std::uint8_t segment_number,
                                           std::uint8_t *cto_buffer,
                                           std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_segment_mode(xcp_handle,
                                            segment_number,
                                            cto_buffer,
                                            cto_buffer_length);
}

peak::StatusType PEAKAPI::copy_calibration_page(peak::HandleType xcp_handle,
                                                peak::CalibrationPageType source,
                                                peak::CalibrationPageType destination,
                                                std::uint8_t *cto_buffer,
                                                std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_copy_calibration_page(
        xcp_handle, source, destination, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::set_daq_list_pointer(peak::HandleType xcp_handle,
                                               std::uint16_t daq_list_number,
                                               std::uint8_t odt_number,
                                               std::uint8_t odt_entry,
                                               std::uint8_t *cto_buffer,
                                               std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_set_daq_list_pointer(
        xcp_handle, daq_list_number, odt_number, odt_entry, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::write_daq_list_entry(peak::HandleType xcp_handle,
                                               peak::ODTEntryType entry,
                                               std::uint8_t *cto_buffer,
                                               std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_write_daq_list_entry(xcp_handle, entry, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::write_daq_list_entries(peak::HandleType xcp_handle,
                                                 std::uint8_t number_of_elements,
                                                 peak::ODTEntryType *elements,
                                                 std::uint8_t *cto_buffer,
                                                 std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_write_daq_list_entries(
        xcp_handle, number_of_elements, elements, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::set_daq_list_mode(peak::HandleType xcp_handle,
                                            std::uint8_t mode,
                                            peak::DAQListConfigType configuration,
                                            std::uint8_t *cto_buffer,
                                            std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_set_daq_list_mode(
        xcp_handle, mode, configuration, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::start_stop_daq_list(peak::HandleType xcp_handle,
                                              std::uint8_t mode,
                                              std::uint16_t daq_list_number,
                                              std::uint8_t *cto_buffer,
                                              std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_start_stop_daq_list(
        xcp_handle, mode, daq_list_number, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::start_stop_synchronized_daq_list(peak::HandleType xcp_handle,
                                                           std::uint8_t mode,
                                                           std::uint8_t *cto_buffer,
                                                           std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_start_stop_synchronized_daq_list(xcp_handle,
                                                            mode,
                                                            cto_buffer,
                                                            cto_buffer_length);
}

peak::StatusType PEAKAPI::read_daq_list_entry(peak::HandleType xcp_handle,
                                              std::uint8_t *cto_buffer,
                                              std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_read_daq_list_entry(xcp_handle, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::get_daq_clock(peak::HandleType xcp_handle,
                                        std::uint8_t *cto_buffer,
                                        std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_daq_clock(xcp_handle, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::get_daq_processor_information(peak::HandleType xcp_handle,
                                                        std::uint8_t *cto_buffer,
                                                        std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_daq_processor_information(xcp_handle, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::get_daq_resolution_information(peak::HandleType xcp_handle,
                                                         std::uint8_t *cto_buffer,
                                                         std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_daq_resolution_information(xcp_handle,
                                                          cto_buffer,
                                                          cto_buffer_length);
}

peak::StatusType PEAKAPI::get_daq_list_mode(peak::HandleType xcp_handle,
                                            std::uint16_t daq_list_number,
                                            std::uint8_t *cto_buffer,
                                            std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_daq_list_mode(xcp_handle,
                                             daq_list_number,
                                             cto_buffer,
                                             cto_buffer_length);
}

peak::StatusType PEAKAPI::get_event_channel_information(peak::HandleType xcp_handle,
                                                        std::uint16_t event_channel_number,
                                                        std::uint8_t *cto_buffer,
                                                        std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_event_channel_information(xcp_handle,
                                                         event_channel_number,
                                                         cto_buffer,
                                                         cto_buffer_length);
}

peak::StatusType PEAKAPI::clear_daq_list(peak::HandleType xcp_handle,
                                         std::uint16_t daq_list_number,
                                         std::uint8_t *cto_buffer,
                                         std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_clear_daq_list(xcp_handle,
                                          daq_list_number,
                                          cto_buffer,
                                          cto_buffer_length);
}

peak::StatusType PEAKAPI::get_daq_list_information(peak::HandleType xcp_handle,
                                                   std::uint16_t daq_list_number,
                                                   std::uint8_t *cto_buffer,
                                                   std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_daq_list_information(xcp_handle,
                                                    daq_list_number,
                                                    cto_buffer,
                                                    cto_buffer_length);
}

peak::StatusType PEAKAPI::free_daq_lists(peak::HandleType xcp_handle,
                                         std::uint8_t *cto_buffer,
                                         std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_free_daq_lists(xcp_handle, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::allocate_daq_lists(peak::HandleType xcp_handle,
                                             std::uint16_t daq_count,
                                             std::uint8_t *cto_buffer,
                                             std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_allocate_daq_lists(xcp_handle, daq_count, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::allocate_odt(peak::HandleType xcp_handle,
                                       std::uint16_t daq_list_number,
                                       std::uint8_t odt_count,
                                       std::uint8_t *cto_buffer,
                                       std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_allocate_odt(
        xcp_handle, daq_list_number, odt_count, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::allocate_odt_entry(peak::HandleType xcp_handle,
                                             std::uint16_t daq_list_number,
                                             std::uint8_t odt_number,
                                             std::uint8_t entries_count,
                                             std::uint8_t *cto_buffer,
                                             std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_allocate_odt_entry(
        xcp_handle, daq_list_number, odt_number, entries_count, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::program_start(peak::HandleType xcp_handle,
                                        std::uint8_t *cto_buffer,
                                        std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_program_start(xcp_handle, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::program_clear(peak::HandleType xcp_handle,
                                        std::uint8_t mode,
                                        std::uint32_t clear_range,
                                        std::uint8_t *cto_buffer,
                                        std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_program_clear(
        xcp_handle, mode, clear_range, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::program(peak::HandleType xcp_handle,
                                  std::uint8_t number_of_elements,
                                  std::uint8_t *data_buffer,
                                  std::uint8_t data_buffer_length,
                                  std::uint8_t *cto_buffer,
                                  std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_program(xcp_handle,
                                   number_of_elements,
                                   data_buffer,
                                   data_buffer_length,
                                   cto_buffer,
                                   cto_buffer_length);
}

peak::StatusType PEAKAPI::program_reset(peak::HandleType xcp_handle,
                                        std::uint8_t *cto_buffer,
                                        std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_program_reset(xcp_handle, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::get_program_processor_information(peak::HandleType xcp_handle,
                                                            std::uint8_t *cto_buffer,
                                                            std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_program_processor_information(xcp_handle,
                                                             cto_buffer,
                                                             cto_buffer_length);
}

peak::StatusType PEAKAPI::get_sector_information(peak::HandleType xcp_handle,
                                                 std::uint8_t mode,
                                                 std::uint8_t sector_number,
                                                 std::uint8_t *cto_buffer,
                                                 std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_get_sector_information(
        xcp_handle, mode, sector_number, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::program_prepare(peak::HandleType xcp_handle,
                                          std::uint16_t code_size,
                                          std::uint8_t *cto_buffer,
                                          std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_program_prepare(xcp_handle, code_size, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::program_format(peak::HandleType xcp_handle,
                                         peak::ProgramFormatType format,
                                         std::uint8_t *cto_buffer,
                                         std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_program_format(xcp_handle, format, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::program_next(peak::HandleType xcp_handle,
                                       std::uint8_t number_of_elements,
                                       std::uint8_t *data_buffer,
                                       std::uint8_t data_length,
                                       std::uint8_t *cto_buffer,
                                       std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_program_next(
        xcp_handle, number_of_elements, data_buffer, data_length, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::program_max(peak::HandleType xcp_handle,
                                      std::uint8_t *element_buffer,
                                      std::uint8_t *cto_buffer,
                                      std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_program_max(xcp_handle, element_buffer, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::program_verify(peak::HandleType xcp_handle,
                                         std::uint8_t mode,
                                         std::uint16_t type,
                                         std::uint32_t value,
                                         std::uint8_t *cto_buffer,
                                         std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_program_verify(
        xcp_handle, mode, type, value, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::build_checksum(peak::HandleType xcp_handle,
                                         std::uint32_t block_size,
                                         std::uint8_t *cto_buffer,
                                         std::uint8_t cto_buffer_length)
{
    return PEAKAPI::_f_xcp_build_checksum(xcp_handle, block_size, cto_buffer, cto_buffer_length);
}

peak::StatusType PEAKAPI::get_error_text(peak::StatusType error_code, std::uint8_t *text_buffer)
{
    return PEAKAPI::_f_xcp_get_error_text(error_code, text_buffer);
}
