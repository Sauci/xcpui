#if 0
#include "boost/asio.hpp"
#include "Xcp.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <functional>

#include "Queue.h"
#include "boost/lockfree/queue.hpp"
#include "boost/thread.hpp"

using boost::asio::ip::tcp;

class TcpClient
{
private:

    boost::asio::io_service _io_service;
    tcp::socket _socket;

    Queue<std::tuple<int, BYTE>> tcp_queue;
    boost::thread *tcp_rx;
    boost::thread *tcp_tx;

    void read_from_server()
    {
    }

    void write_to_server()
    {
        while (true)
        {
            auto packet = tcp_queue.pop();

            boost::system::error_code error;
            //boost::asio::write(_socket,
            //                   boost::asio::buffer(packet),
            //                   boost::asio::transfer_exactly(8),
            //                   error);
        }
    }

public:

    TcpClient() : _socket(_io_service)
    {
        tcp::resolver resolver{_io_service};
        tcp::resolver::results_type endpoints = resolver.resolve("localhost", "5555");

        boost::asio::connect(_socket, endpoints);
    }

    ~TcpClient()
    {
        delete tcp_rx;
        delete tcp_tx;
    }

    boost::asio::io_context::count_type run()
    {
        tcp_rx = new boost::thread(boost::bind(&TcpClient::read_from_server, this));
        tcp_tx = new boost::thread(boost::bind(&TcpClient::write_to_server, this));
        auto tmp_return = _io_service.run();
        tcp_rx->join();
        tcp_tx->join();
        return tmp_return;
    }

    void connect(BYTE mode)
    {
        tcp_queue.push(std::tuple<int, BYTE>(0, mode));
    }
};

void tcp_write_to_server(Queue<std::tuple<int, BYTE>> *queue)
{
    boost::asio::io_context io;
    try
    {
        tcp::resolver resolver{io};
        tcp::resolver::results_type endpoints = resolver.resolve("localhost", "5555");

        tcp::socket socket{io};
        boost::asio::connect(socket, endpoints);

        for (;;)
        {
            auto data = queue->pop();
            std::array<char, 8> can_data;

            boost::system::error_code error;
            boost::asio::read(socket, boost::asio::buffer(can_data),
                              boost::asio::transfer_exactly(8), error);

            if (error == boost::asio::error::eof)
            {
                continue; // connection closed cleanly by peer
            }
            else if (error)
            {
                throw boost::system::system_error(error);
            }
            else
            {
                std::cout << can_data.data() << std::endl;
            }
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

#endif

#include <iostream>

#include "rpc/client.h"
#include "shared.h"
#include "xcp_peak_interface.h"

struct XCPClient : public rpc::client, public XCPInterface
{
    explicit XCPClient(std::string const &address, uint16_t port = 5000) :
        rpc::client(address, port), XCPInterface()
    {
    }

    ~XCPClient() = default;

    const interface_types::interface_type get_interface_type() override
    {
        auto tmp_return = rpc::client::call(STR(GET_INTERFACE_TYPE));
        return tmp_return.as<interface_types::interface_type>();
    }

    const interface_types::interface_name_type get_interface_name() override
    {
        auto tmp_return = rpc::client::call(STR(GET_INTERFACE_NAME));
        return tmp_return.as<interface_types::interface_name_type>();
    }

    const interface_types::hardware_channel_type get_hardware_channel_count() override
    {
        auto tmp_return = rpc::client::call(STR(GET_HARDWARE_CHANNEL_COUNT));
        return tmp_return.as<interface_types::hardware_channel_type>();
    }

    const bool is_plugged_in() override
    {
        auto tmp_return = rpc::client::call(STR(IS_PLUGGED_IN));
        return tmp_return.as<bool>();
    }

    const xcp_interface_type::identifier_type get_master_identifier() override
    {
        auto tmp_return = rpc::client::call(STR(GET_MASTER_IDENTIFIER));
        return tmp_return.as<xcp_interface_type::identifier_type>();
    }

    const xcp_interface_type::identifier_type get_slave_identifier() override
    {
        auto tmp_return = rpc::client::call(STR(GET_SLAVE_IDENTIFIER));
        return tmp_return.as<xcp_interface_type::identifier_type>();
    }

    const xcp_interface_type::identifier_type get_broadcast_identifier() override
    {
        auto tmp_return = rpc::client::call(STR(GET_BROADCAST_IDENTIFIER));
        return tmp_return.as<xcp_interface_type::identifier_type>();
    }

    const xcp_interface_type::baud_rate_type get_baud_rate() override
    {
        auto tmp_return = rpc::client::call(STR(GET_BAUD_RATE));
        return tmp_return.as<xcp_interface_type::baud_rate_type>();
    }

    const xcp_interface_type::hardware_channel_type get_hardware_channel() override
    {
        auto tmp_return = rpc::client::call(STR(GET_HARDWARE_CHANNEL));
        return tmp_return.as<xcp_interface_type::hardware_channel_type>();
    }

    const xcp_interface_type::timing_parameter_type
    get_timing_parameter(xcp_interface_type::timing_parameter_id_type timing_parameter_id) override
    {
        auto tmp_return = rpc::client::call(STR(GET_TIMING_PARAMETER), timing_parameter_id);
        return tmp_return.as<xcp_interface_type::timing_parameter_type>();
    }

    void dequeue_cto(bool &is_valid,
                     xcp_api_types::status_type &status,
                     xcp_interface_type::buffer_type &buffer) override
    {
        auto tmp_return = rpc::client::call(STR(GET_CTO), is_valid, status, buffer);
    }

    void set_master_identifier(xcp_api_types::identifier_type identifier) override
    {
        rpc::client::async_call(STR(SET_MASTER_IDENTIFIER), identifier);
    }

    void set_slave_identifier(xcp_api_types::identifier_type identifier) override
    {
        rpc::client::async_call(STR(SET_SLAVE_IDENTIFIER), identifier);
    }

    void set_broadcast_identifier(xcp_api_types::identifier_type identifier) override
    {
        rpc::client::async_call(STR(SET_BROADCAST_IDENTIFIER), identifier);
    }

    void set_baud_rate(xcp_api_types::baud_rate_type baud_rate) override
    {
        rpc::client::async_call(STR(SET_BAUD_RATE), baud_rate);
    }

    void set_hardware_channel(interface_types::hardware_channel_type hardware_channel) override
    {
        rpc::client::async_call(STR(SET_HARDWARE_CHANNEL), hardware_channel);
    }

    void set_timing_parameter(xcp_api_types::timing_parameter_id_type timing_parameter_id,
                              xcp_api_types::timing_parameter_type timing_parameter) override
    {
        rpc::client::async_call(STR(SET_TIMING_PARAMETER), timing_parameter_id, timing_parameter);
    }

    void initialize_hardware() override
    {
        rpc::client::async_call(STR(INITIALIZE_HARDWARE));
    }

    void de_initialize_hardware(bool wait_for_completion) override
    {
        rpc::client::async_call(STR(DE_INITIALIZE_HARDWARE), wait_for_completion);
    };

    void connect(xcp_types::connect::MODE mode) override
    {
        rpc::client::async_call(STR(CONNECT), mode);
    }

    void disconnect() override
    {
        rpc::client::async_call(STR(DISCONNECT));
    }

    void get_status() override
    {
        rpc::client::async_call(STR(GET_STATUS));
    }

    void synch() override
    {
        rpc::client::async_call(STR(SYNCH));
    }

    void get_comm_mode_info() override
    {
        rpc::client::async_call(STR(GET_COMM_MODE_INFO));
    }

    void get_id(xcp_types::get_id::TYPE request_identification) override
    {
        rpc::client::async_call(STR(GET_ID));
    }

    void set_request(xcp_types::set_request::MODE mode,
                     xcp_types::set_request::SESSION_CONFIGURATION_ID session_configuration_id) override
    {
        rpc::client::async_call(STR(SET_REQUEST), mode, session_configuration_id);
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

    bool
    cto_completion_handler(message_type message_type, XCPMessageInterface *cto_message) override
    {
        return false;
    }
};

int main(int argc, char *argv[])
{
    auto client = XCPClient("192.168.1.121", 8080);

    client.set_master_identifier(12);
    return 0;
}
