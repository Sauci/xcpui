//
// Created by sauci on 15/11/2018.
//

#include "gtest/gtest.h"

#include "../include/peak_api.h"

class APICall : public ::testing::Test
{
    public:
    PEAKAPI *dll;

    protected:
    APICall() : testing::Test()
    {
        APICall::dll = nullptr;
    }

    void SetUp() override
    {
        APICall::dll = new PEAKAPI();
    }

    void TearDown() override
    {
        delete APICall::dll;
    }
};

TEST_F(APICall, initalize_can_channel)
{
    peak::ChannelXCPType channel_xcp;

    APICall::dll->initialize_can_channel(&channel_xcp,
                                         peak::ChannelType::CHANNEL_BUS_USB_1,
                                         peak::BaudRateType::BAUD_RATE_5000,
                                         0,
                                         0,
                                         0);
}

TEST_F(APICall, initialize_can_channel_fd)
{
    peak::ChannelXCPType channel_xcp = 0;

    const char *bit_rate =
        "f_clock=80000000,"
        "nom_brp=10,"
        "nom_tseg1=5,"
        "nom_tseg2=2,"
        "nom_sjw=1,"
        "data_brp=4,"
        "data_tseg1=7,"
        "data_tseg2=2,"
        "data_sjw=1";

    APICall::dll->initialize_can_channel(&channel_xcp,
                                         peak::ChannelType::CHANNEL_BUS_USB_1,
                                         (peak::BaudRateFDType)&bit_rate[0]);
}

TEST_F(APICall, uninitialize_channel)
{
    APICall::dll->uninitialize_channel(0);
}

TEST_F(APICall, add_slave_on_can)
{
    peak::TransportCANType layer_data = {0, 0, 0, false};

    peak::ProtocolType protocol = {0, 0, 0, 0, 0, 0, 0, false, 0};

    peak::HandleType xcp_handle = 0;

    APICall::dll->add_slave_on_can(0, layer_data, protocol, &xcp_handle);
}

TEST_F(APICall, remove_slave)
{
    peak::HandleType xcp_handle = 0;

    APICall::dll->remove_slave(xcp_handle);
}

TEST_F(APICall, dequeue_packet)
{
    APICall::dll->dequeue_packet(0, peak::QueueType::CTO, nullptr, 0);
}

TEST_F(APICall, reset_queue)
{
    APICall::dll->reset_queue(0, peak::QueueType::CTO);
}

TEST_F(APICall, connect)
{
    APICall::dll->connect(0, 0, nullptr, 0);
}

TEST_F(APICall, disconnect)
{
    APICall::dll->disconnect(0, nullptr, 0);
}

TEST_F(APICall, get_status)
{
    APICall::dll->get_status(0, nullptr, 0);
}

TEST_F(APICall, synchronize)
{
    APICall::dll->synchronize(0, nullptr, 0);
}

TEST_F(APICall, get_communication_mode)
{
    APICall::dll->get_communication_mode(0, nullptr, 0);
}

TEST_F(APICall, get_identification)
{
    APICall::dll->get_identification(0, 0, nullptr, 0);
}

TEST_F(APICall, set_request)
{
    APICall::dll->set_request(0, 0, 0, nullptr, 0);
}

TEST_F(APICall, get_seed)
{
    APICall::dll->get_seed(0, 0, 0, nullptr, 0);
}

TEST_F(APICall, unlock)
{
    APICall::dll->unlock(0, 0, nullptr, nullptr, 0);
}

TEST_F(APICall, transport_layer_command)
{
    APICall::dll->transport_layer_command(0, 0, nullptr, 0, nullptr, 0);
}

TEST_F(APICall, user_command)
{
    APICall::dll->user_command(0, 0, nullptr, 0, nullptr, 0);
}

TEST_F(APICall, set_memory_transfer_address)
{
    APICall::dll->set_memory_transfer_address(0, 0, 0, nullptr, 0);
}

TEST_F(APICall, upload)
{
    APICall::dll->upload(0, 0, nullptr, 0);
}

TEST_F(APICall, short_upload)
{
    APICall::dll->short_upload(0, 0, 0, 0, nullptr, 0);
}

TEST_F(APICall, download)
{
    APICall::dll->download(0, 0, nullptr, 0, nullptr, 0);
}

TEST_F(APICall, download_next)
{
    APICall::dll->download_next(0, 0, nullptr, 0, nullptr, 0);
}

TEST_F(APICall, download_max)
{
    APICall::dll->download_max(0, nullptr, nullptr, 0);
}

TEST_F(APICall, modify_bits)
{
    APICall::dll->modify_bits(0, 0, 0, 0, nullptr, 0);
}

TEST_F(APICall, set_calibration_page)
{
    peak::CalibrationPageType calibration_page = {0, 0};

    APICall::dll->set_calibration_page(0, 0, calibration_page, nullptr, 0);
}

TEST_F(APICall, get_calibration_page)
{
    APICall::dll->get_calibration_page(0, 0, 0, nullptr, 0);
}

TEST_F(APICall, get_paging_processor_information)
{
    APICall::dll->get_paging_processor_information(0, nullptr, 0);
}

TEST_F(APICall, get_segment_information)
{
    APICall::dll->get_segment_information(0, 0, 0, 0, 0, nullptr, 0);
}

TEST_F(APICall, get_page_information)
{
    peak::CalibrationPageType calibration_page = {0, 0};

    APICall::dll->get_page_information(0, calibration_page, nullptr, 0);
}

TEST_F(APICall, set_segment_mode)
{
    APICall::dll->set_segment_mode(0, 0, 0, nullptr, 0);
}

TEST_F(APICall, get_segment_mode)
{
    APICall::dll->get_segment_mode(0, 0, nullptr, 0);
}

TEST_F(APICall, copy_calibration_page)
{
    peak::CalibrationPageType calibration_page_source = {0, 0};
    peak::CalibrationPageType calibration_page_destination = {0, 0};

    APICall::dll->copy_calibration_page(
        0, calibration_page_source, calibration_page_destination, nullptr, 0);
}

TEST_F(APICall, set_daq_list_pointer)
{
    APICall::dll->set_daq_list_pointer(0, 0, 0, 0, nullptr, 0);
}

TEST_F(APICall, write_daq_list_entry)
{
    peak::ODTEntryType odt_entry = {0, 0, 0, 0};

    APICall::dll->write_daq_list_entry(0, odt_entry, nullptr, 0);
}

TEST_F(APICall, write_daq_list_entries)
{
    peak::ODTEntryType elements[1];

    APICall::dll->write_daq_list_entries(
        0, sizeof(elements) / sizeof(*elements), &elements[0], nullptr, 0);
}

TEST_F(APICall, set_daq_list_mode)
{
    peak::DAQListConfigType configuration = {0, 0, 0, 0};

    APICall::dll->set_daq_list_mode(0, 0, configuration, nullptr, 0);
}

TEST_F(APICall, start_stop_daq_list)
{
    APICall::dll->start_stop_daq_list(0, 0, 0, nullptr, 0);
}

TEST_F(APICall, start_stop_synchronized_daq_list)
{
    APICall::dll->start_stop_synchronized_daq_list(0, 0, nullptr, 0);
}

TEST_F(APICall, read_daq_list_entry)
{
    APICall::dll->read_daq_list_entry(0, nullptr, 0);
}

TEST_F(APICall, get_daq_clock)
{
    APICall::dll->get_daq_clock(0, nullptr, 0);
}

TEST_F(APICall, get_daq_processor_information)
{
    APICall::dll->get_daq_processor_information(0, nullptr, 0);
}

TEST_F(APICall, get_daq_resolution_information)
{
    APICall::dll->get_daq_resolution_information(0, nullptr, 0);
}

TEST_F(APICall, get_daq_list_mode)
{
    APICall::dll->get_daq_list_mode(0, 0, nullptr, 0);
}

TEST_F(APICall, get_event_channel_information)
{
    APICall::dll->get_event_channel_information(0, 0, nullptr, 0);
}

TEST_F(APICall, clear_daq_list)
{
    APICall::dll->clear_daq_list(0, 0, nullptr, 0);
}

TEST_F(APICall, get_daq_list_information)
{
    APICall::dll->get_daq_list_information(0, 0, nullptr, 0);
}

TEST_F(APICall, free_daq_lists)
{
    APICall::dll->free_daq_lists(0, nullptr, 0);
}

TEST_F(APICall, allocate_daq_lists)
{
    APICall::dll->allocate_daq_lists(0, 0, nullptr, 0);
}

TEST_F(APICall, allocate_odt)
{
    APICall::dll->allocate_odt(0, 0, 0, nullptr, 0);
}

TEST_F(APICall, allocate_odt_entry)
{
    APICall::dll->allocate_odt_entry(0, 0, 0, 0, nullptr, 0);
}

TEST_F(APICall, program_start)
{
    APICall::dll->program_start(0, nullptr, 0);
}

TEST_F(APICall, program_clear)
{
    APICall::dll->program_clear(0, 0, 0, nullptr, 0);
}

TEST_F(APICall, program)
{
    APICall::dll->program(0, 0, nullptr, 0, nullptr, 0);
}

TEST_F(APICall, program_reset)
{
    APICall::dll->program_reset(0, nullptr, 0);
}

TEST_F(APICall, get_program_processor_information)
{
    APICall::dll->get_program_processor_information(0, nullptr, 0);
}

TEST_F(APICall, get_sector_information)
{
    APICall::dll->get_sector_information(0, 0, 0, nullptr, 0);
}

TEST_F(APICall, program_prepare)
{
    APICall::dll->program_prepare(0, 0, nullptr, 0);
}

TEST_F(APICall, program_format)
{
    peak::ProgramFormatType program_format = {0, 0, 0, 0};

    APICall::dll->program_format(0, program_format, nullptr, 0);
}

TEST_F(APICall, program_next)
{
    APICall::dll->program_next(0, 0, nullptr, 0, nullptr, 0);
}

TEST_F(APICall, program_max)
{
    APICall::dll->program_max(0, nullptr, nullptr, 0);
}

TEST_F(APICall, program_verify)
{
    APICall::dll->program_verify(0, 0, 0, 0, nullptr, 0);
}

TEST_F(APICall, build_checksum)
{
    APICall::dll->build_checksum(0, 0, nullptr, 0);
}

TEST_F(APICall, get_error_text)
{
    char text_buffer[256];

    APICall::dll->get_error_text(peak::StatusType::DLL_XCP_ERR_ACCESS_DENIED,
                                 (std::uint8_t *)&text_buffer);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
