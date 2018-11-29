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
#include "xcp_interface.h"

struct XCPClient : public rpc::client
{
    explicit XCPClient(std::string const &address, uint16_t port) : rpc::client(address, port)
    {
    }

    ~XCPClient() = default;

    const interface_types::interface_type get_interface_type()
    {
        auto tmp_return = rpc::client::call(STR(GET_INTERFACE_TYPE));
        return tmp_return.as<interface_types::interface_type>();
    }

    const interface_types::interface_name_type get_interface_name()
    {
        auto tmp_return = rpc::client::call(STR(GET_INTERFACE_NAME));
        return tmp_return.as<interface_types::interface_name_type>();
    }

    const interface_types::hardware_channel_type get_hardware_channel_count()
    {
        auto tmp_return = rpc::client::call(STR(GET_HARDWARE_CHANNEL_COUNT));
        return tmp_return.as<interface_types::hardware_channel_type>();
    }

    const bool is_plugged_in()
    {
        auto tmp_return = rpc::client::call(STR(IS_PLUGGED_IN));
        return tmp_return.as<bool>();
    }

    const xcp_interface_types::identifier_type get_master_identifier()
    {
        auto tmp_return = rpc::client::call(STR(GET_MASTER_IDENTIFIER));
        return tmp_return.as<xcp_interface_types::identifier_type>();
    }

    const xcp_interface_types::identifier_type get_slave_identifier()
    {
        auto tmp_return = rpc::client::call(STR(GET_SLAVE_IDENTIFIER));
        return tmp_return.as<xcp_interface_types::identifier_type>();
    }

    const xcp_interface_types::identifier_type get_broadcast_identifier()
    {
        auto tmp_return = rpc::client::call(STR(GET_BROADCAST_IDENTIFIER));
        return tmp_return.as<xcp_interface_types::identifier_type>();
    }

    const xcp_interface_types::baud_rate_type get_baud_rate()
    {
        auto tmp_return = rpc::client::call(STR(GET_BAUD_RATE));
        return tmp_return.as<xcp_interface_types::baud_rate_type>();
    }

    const xcp_interface_types::hardware_channel_type get_hardware_channel()
    {
        auto tmp_return = rpc::client::call(STR(GET_HARDWARE_CHANNEL));
        return tmp_return.as<xcp_interface_types::hardware_channel_type>();
    }

    const xcp_interface_types::timing_type
    get_timing_parameter(xcp_interface_types::timing_id_type timing_parameter_id)
    {
        auto tmp_return = rpc::client::call(STR(GET_TIMING_PARAMETER), timing_parameter_id);
        return tmp_return.as<xcp_interface_types::timing_type>();
    }

    XCPDTOPacket dequeue_dto()
    {
        auto tmp_return = rpc::client::call(STR(GET_CTO));
        return tmp_return.as<XCPDTOPacket>();
    }

    void set_master_identifier(xcp_interface_types::identifier_type identifier)
    {
        rpc::client::async_call(STR(SET_MASTER_IDENTIFIER), identifier);
    }

    void set_slave_identifier(xcp_interface_types::identifier_type identifier)
    {
        rpc::client::async_call(STR(SET_SLAVE_IDENTIFIER), identifier);
    }

    void set_broadcast_identifier(xcp_interface_types::identifier_type identifier)
    {
        rpc::client::async_call(STR(SET_BROADCAST_IDENTIFIER), identifier);
    }

    void set_baud_rate(xcp_interface_types::baud_rate_type baud_rate)
    {
        rpc::client::async_call(STR(SET_BAUD_RATE), baud_rate);
    }

    void set_hardware_channel(interface_types::hardware_channel_type hardware_channel)
    {
        rpc::client::async_call(STR(SET_HARDWARE_CHANNEL), hardware_channel);
    }

    void set_timing_parameter(xcp_interface_types::timing_id_type timing_id,
                              xcp_interface_types::timing_type timing)
    {
        rpc::client::async_call(STR(SET_TIMING_PARAMETER), timing_id, timing);
    }

    void initialize_hardware()
    {
        rpc::client::async_call(STR(INITIALIZE_HARDWARE));
    }

    void de_initialize_hardware(bool wait_for_completion)
    {
        rpc::client::async_call(STR(DE_INITIALIZE_HARDWARE), wait_for_completion);
    };

    void connect(XCP::CONNECT::MODE mode)
    {
        rpc::client::async_call(STR(CONNECT_COMMAND_CODE), mode);
    }

    bool cto_completion_handler(XCPCTORequestInterface *cto_message)
    {
        return false;
    }
};

int main(int argc, char *argv[])
{
    /* create a new client instance. */
    auto client = XCPClient("192.168.1.121", 8080);

    /* configure the communication */
    client.set_master_identifier(0x100);
    client.set_slave_identifier(0x200);
    client.set_broadcast_identifier(0x300);
    client.set_baud_rate(500000);
    client.set_hardware_channel(1);
    client.set_timing_parameter(timing_id_type::T1, 2000);
    client.set_timing_parameter(timing_id_type::T2, 2000);
    client.set_timing_parameter(timing_id_type::T3, 2000);
    client.set_timing_parameter(timing_id_type::T4, 2000);
    client.set_timing_parameter(timing_id_type::T5, 2000);
    client.set_timing_parameter(timing_id_type::T6, 2000);
    client.set_timing_parameter(timing_id_type::T7, 2000);

    /* initialize interface. */
    client.initialize_hardware();

    /* start polling for 1000 data transfer objects. */
    unsigned int idx = 0;

    while (idx < 1000)
    {
        auto dto = client.dequeue_dto();

        if (dto.valid)
        {
            std::string str(dto.buffer.begin(), dto.buffer.end());
            std::cout << idx << ": " << str << std::endl;
//            idx++;
        }
    }

    /* de-initialize the interface. */
    client.de_initialize_hardware(false);
}
