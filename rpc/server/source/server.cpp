#if 0

#include <functional>
#include <map>
#include <any>
#include <string>
#include <tuple>
#include <iostream>

#include "tcpip/tcpip.h"

#include <msgpack.hpp>
#include "xcp_peak_interface.h"
#include "xcp_message.h"

typedef enum : BYTE
{
    INVALID = 0,
    INITIALIZE_CAN_CHANNEL = 1,
    INITIALIZE_CAN_CHANNEL_FD = 2,
    UNINITIALIZE_CHANNEL = 3,
    ADD_SLAVE_ON_CAN = 4,
    REMOVE_SLAVE = 5,
    DEQUEUE_PACKET = 6,
    RESET_QUEUE = 7,

    SET_MASTER_ID = 8,
    SET_SLAVE_ID = 9,
    SET_BROADCAST_ID = 10,

    SET_PROTOCOL_T1_PARAMETER = 11,
    SET_PROTOCOL_T2_PARAMETER = 12,
    SET_PROTOCOL_T3_PARAMETER = 13,
    SET_PROTOCOL_T4_PARAMETER = 14,
    SET_PROTOCOL_T5_PARAMETER = 15,
    SET_PROTOCOL_T6_PARAMETER = 16,
    SET_PROTOCOL_T7_PARAMETER = 17
} PeakCommand;

MSGPACK_ADD_ENUM(PeakCommand);

class CommandInterface : public msgpack::packer<msgpack::sbuffer>
{
protected:

    msgpack::sbuffer buffer_;

public:

    CommandInterface() : msgpack::packer<msgpack::sbuffer>(&buffer_)
    {
    }

    virtual xcp_api_types::status_type operator()() = 0;

    virtual void update_parameters(msgpack::object &object) = 0;
};

class CommandFactory
{
    static std::map<PeakCommand, CommandInterface *> command_map_;

public:

    void register_class(PeakCommand command, CommandInterface *interface)
    {
        command_map_.insert(std::pair<PeakCommand, CommandInterface *>(command, interface));
    }

    CommandInterface *operator()(PeakCommand command)
    {
        return command_map_[command];
    }
};

std::map<PeakCommand, CommandInterface *>CommandFactory::command_map_;

CommandFactory command_factory;

template<PeakCommand command, typename ... parameters>
class Command : public CommandInterface
{
    static constexpr PeakCommand command_ = command;
    std::tuple<parameters...> parameters_;

protected:

    typedef xcp_api_types::status_type (XCPInterface::*func_ptr)(parameters...);

    func_ptr function;

public:

    Command() : CommandInterface()
    {
        command_factory.register_class(command, this);

        function = nullptr;

        this->pack_array(2);
        this->pack(command_);
    }

    explicit Command(parameters ... args) : CommandInterface()
    {
        command_factory.register_class(command, this);

        function = nullptr;

        parameters_ = std::make_tuple(args...);

        this->pack_array(2);
        this->pack(command_);
        this->pack(parameters_);
    }

    void update_parameters(msgpack::object &object) override
    {
        object.convert(parameters_);
    }

    xcp_api_types::status_type operator()() override
    {
        return 0;//std::apply(function, std::tuple_cat(std::make_tuple(this), parameters_));
    }

    const msgpack::sbuffer &get_buffer() const
    {
        return buffer_;
    }
};

class PacketStream
{
public:

    typedef void (*event_callback)(CommandInterface *interface);

private:

    msgpack::unpacker unpacker_;
    event_callback cb_;

public:

    explicit PacketStream(event_callback cb)
    {
        cb_ = cb;
    }

    void operator<<(const msgpack::sbuffer &string_buffer)
    {
        unpacker_.reserve_buffer(string_buffer.size());
        memcpy(unpacker_.buffer(), string_buffer.data(), string_buffer.size());
        unpacker_.buffer_consumed(string_buffer.size());

        msgpack::object_handle object_handle;

        while (unpacker_.next(object_handle))
        {
            PeakCommand command_type;
            auto object = object_handle.get();

            object.via.array.ptr[0].convert(command_type);

            CommandInterface *command = command_factory(command_type);
            command->update_parameters(object.via.array.ptr[1]);
            cb_(command);
        }
    }
};

void event_callback(CommandInterface *command_interface)
{
    auto return_value = command_interface->operator()();
    std::cout << return_value << std::endl;
}
#endif

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "rpc/server.h"
#include "shared.h"
#include "xcp_peak_interface.h"

class XCPServer : public rpc::server
{
public:

    explicit XCPServer(XCPInterface &xcp_interface, uint16_t port = 5000) : rpc::server(port)
    {
        /* InterfaceInterface methods. */
        this->bind(STR(GET_INTERFACE_TYPE), [&xcp_interface]() -> interface_types::interface_type
        { return xcp_interface.get_interface_type(); });
        this->bind(STR(GET_INTERFACE_NAME),
                   [&xcp_interface]() -> interface_types::interface_name_type
                   { return xcp_interface.get_interface_name(); });
        this->bind(STR(GET_HARDWARE_CHANNEL_COUNT),
                   [&xcp_interface]() -> interface_types::hardware_channel_type
                   { return xcp_interface.get_hardware_channel_count(); });
        this->bind(STR(IS_PLUGGED_IN),
                   [&xcp_interface]() -> bool
                   { return xcp_interface.is_plugged_in(); });

        /* XCPInterface getter methods. */
        this->bind(STR(GET_MASTER_IDENTIFIER),
                   [&xcp_interface]() -> xcp_api_types::identifier_type
                   { return xcp_interface.get_master_identifier(); });
        this->bind(STR(GET_SLAVE_IDENTIFIER),
                   [&xcp_interface]() -> xcp_api_types::identifier_type
                   { return xcp_interface.get_slave_identifier(); });
        this->bind(STR(GET_BROADCAST_IDENTIFIER),
                   [&xcp_interface]() -> xcp_api_types::identifier_type
                   { return xcp_interface.get_broadcast_identifier(); });
        this->bind(STR(GET_BAUD_RATE),
                   [&xcp_interface]() -> xcp_interface_type::baud_rate_type
                   { return xcp_interface.get_baud_rate(); });
        this->bind(STR(GET_HARDWARE_CHANNEL),
                   [&xcp_interface]() -> xcp_interface_type::hardware_channel_type
                   { return xcp_interface.get_hardware_channel(); });
        this->bind(STR(GET_TIMING_PARAMETER),
                   [&xcp_interface](xcp_interface_type::timing_parameter_id_type timing_parameter_id)
                   -> xcp_interface_type::hardware_channel_type
                   { return xcp_interface.get_timing_parameter(timing_parameter_id); });

        /* XCPInterface setter methods. */
        this->bind(STR(SET_MASTER_IDENTIFIER),
                   [&xcp_interface](xcp_api_types::identifier_type identifier)
                   { xcp_interface.set_master_identifier(identifier); });
        this->bind(STR(SET_SLAVE_IDENTIFIER),
                   [&xcp_interface](xcp_api_types::identifier_type identifier)
                   { xcp_interface.set_slave_identifier(identifier); });
        this->bind(STR(SET_BROADCAST_IDENTIFIER),
                   [&xcp_interface](xcp_api_types::identifier_type identifier)
                   { xcp_interface.set_broadcast_identifier(identifier); });
        this->bind(STR(SET_BAUD_RATE),
                   [&xcp_interface](xcp_api_types::baud_rate_type baud_rate)
                   { xcp_interface.set_baud_rate(baud_rate); });
        this->bind(STR(SET_HARDWARE_CHANNEL),
                   [&xcp_interface](interface_types::hardware_channel_type hardware_channel)
                   { xcp_interface.set_hardware_channel(hardware_channel); });
        this->bind(STR(SET_TIMING_PARAMETER),
                   [&xcp_interface](xcp_api_types::timing_parameter_id_type timing_parameter_id,
                                    xcp_api_types::timing_parameter_type timing_parameter)
                   { xcp_interface.set_timing_parameter(timing_parameter_id, timing_parameter); });

        /* XCPInterface standard methods. */
        this->bind(STR(INITIALIZE_HARDWARE),
                   [&xcp_interface]()
                   { xcp_interface.initialize_hardware(); });
        this->bind(STR(DE_INITIALIZE_HARDWARE),
                   [&xcp_interface](bool wait_for_completion)
                   { xcp_interface.de_initialize_hardware(wait_for_completion); });

        /* XCP protocol standard methods. */
        this->bind(STR(CONNECT),
                   [&xcp_interface](xcp_types::connect::MODE mode)
                   { xcp_interface.connect(mode); });
        this->bind(STR(DISCONNECT),
                   [&xcp_interface]()
                   { xcp_interface.disconnect(); });
    }

    ~XCPServer() = default;
/*
    const interface_types::interface_type get_interface_type() override
    {
        return XCPInterface::get_interface_type();
    }

    const interface_types::interface_name_type get_interface_name() override
    {
        return XCPInterface::get_interface_name();
    }

    const interface_types::hardware_channel_type get_hardware_channel_count() override
    {
        return XCPInterface::get_hardware_channel_count();
    }

    const bool is_plugged_in() override
    {
        return XCPInterface::is_plugged_in();
    }

    const xcp_interface_type::identifier_type get_master_identifier() override
    {
        return XCPInterface::get_master_identifier();
    }

    const xcp_interface_type::identifier_type get_slave_identifier() override
    {
        return XCPInterface::get_slave_identifier();
    }

    const xcp_interface_type::identifier_type get_broadcast_identifier() override
    {
        return XCPInterface::get_broadcast_identifier();
    }

    const xcp_interface_type::baud_rate_type get_baud_rate() override
    {
        return XCPInterface::get_baud_rate();
    }

    const xcp_interface_type::hardware_channel_type get_hardware_channel() override
    {
        return XCPInterface::get_hardware_channel();
    }

    const xcp_interface_type::timing_parameter_type
    get_timing_parameter(xcp_interface_type::timing_parameter_id_type timing_parameter_id) override
    {
        return XCPInterface::get_timing_parameter(timing_parameter_id);
    }

    void set_master_identifier(xcp_api_types::identifier_type identifier) override
    {
        XCPInterface::set_master_identifier(identifier);
    }

    void set_slave_identifier(xcp_api_types::identifier_type identifier) override
    {
        XCPInterface::set_slave_identifier(identifier);
    }

    void set_broadcast_identifier(xcp_api_types::identifier_type identifier) override
    {
        XCPInterface::set_broadcast_identifier(identifier);
    }

    void set_baud_rate(xcp_api_types::baud_rate_type baud_rate) override
    {
        XCPInterface::set_baud_rate(baud_rate);
    }

    void set_hardware_channel(interface_types::hardware_channel_type hardware_channel) override
    {
        XCPInterface::set_hardware_channel(hardware_channel);
    }

    void set_timing_parameter(xcp_api_types::timing_parameter_id_type timing_parameter_id,
                              xcp_api_types::timing_parameter_type timing_parameter) override
    {
        XCPInterface::set_timing_parameter(timing_parameter_id, timing_parameter);
    }

    void initialize_hardware(xcp_api_types::placeholder_type placeholder) override
    {
        XCPInterface::initialize_hardware(placeholder);
    }

    void de_initialize_hardware(bool wait_for_completion) override
    {
        XCPInterface::de_initialize_hardware(wait_for_completion);
    };

    void connect(xcp_protocol_types::connect::mode_type mode) override
    {
        XCPInterface::connect(mode);
    }

    void disconnect() override
    {
        XCPInterface::disconnect();
    }

    bool cto_completion_handler(message_type message_type,
                                XCPCTOMessageInterface<XCPInterface> *cto_message) override
    {
        return false;
    }
    */
};

int main(int argc, char *argv[])
{
    auto api = PEAKAPI();
    auto interface = XCPPEAKInterface(api);
    auto server = XCPServer(interface, 8080);

    server.async_run();

    while (true)
    {
        std::cout << interface.get_master_identifier() << std::endl;
    }

    return 0;
}
