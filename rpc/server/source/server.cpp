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

    virtual xcp_interface_types::status_type operator()() = 0;

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

    typedef xcp_interface_types::status_type (XCPInterface::*func_ptr)(parameters...);

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

    xcp_interface_types::status_type operator()() override
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
#include "can_interface_peak.h"
#include "xcp_peak_interface.h"

class CANServer : public rpc::server
{
    CANInterface &can_interface_;

public:

    CANServer(CANInterface &can_interface, uint16_t port) :
        rpc::server(port), can_interface_(can_interface)
    {
        this->bind(STR(GET_CHANNEL), [&can_interface]() -> can_types::channel_type
        { return can_interface.get_channel();});

        this->bind(STR(GET_BAUD_RATE), [&can_interface]() -> can_types::baud_rate_type
        { return can_interface.get_baud_rate();});

        this->bind(STR(GET_CHANNEL_COUNT), [&can_interface]() -> can_types::channel_type
        { return can_interface.get_channel_count();});
    }
};

class XCPServer : public CANServer
{
    XCPInterface &xcp_interface_;
public:

    explicit XCPServer(CANInterface &can_interface, XCPInterface &xcp_interface, uint16_t port) :
        CANServer(can_interface, port), xcp_interface_(xcp_interface)
    {
        /* XCPInterface getter methods. */
        this->bind(STR(GET_MASTER_IDENTIFIER),
                   [&xcp_interface]() -> xcp_interface_types::identifier_type
                   { return xcp_interface.get_master_identifier(); });
        this->bind(STR(GET_SLAVE_IDENTIFIER),
                   [&xcp_interface]() -> xcp_interface_types::identifier_type
                   { return xcp_interface.get_slave_identifier(); });
        this->bind(STR(GET_BROADCAST_IDENTIFIER),
                   [&xcp_interface]() -> xcp_interface_types::identifier_type
                   { return xcp_interface.get_broadcast_identifier(); });
        this->bind(STR(GET_TIMING_PARAMETER),
                   [&xcp_interface](xcp_interface_types::timing_id_type timing_parameter_id)
                       -> xcp_interface_types::timing_type
                   { return xcp_interface.get_timing_parameter(timing_parameter_id); });

        /* XCPInterface setter methods. */
        this->bind(STR(SET_MASTER_IDENTIFIER),
                   [&xcp_interface](xcp_interface_types::identifier_type identifier)
                   { xcp_interface.set_master_identifier(identifier); });
        this->bind(STR(SET_SLAVE_IDENTIFIER),
                   [&xcp_interface](xcp_interface_types::identifier_type identifier)
                   { xcp_interface.set_slave_identifier(identifier); });
        this->bind(STR(SET_BROADCAST_IDENTIFIER),
                   [&xcp_interface](xcp_interface_types::identifier_type identifier)
                   { xcp_interface.set_broadcast_identifier(identifier); });
        this->bind(STR(SET_TIMING_PARAMETER),
                   [&xcp_interface](xcp_interface_types::timing_id_type timing_id,
                                    xcp_interface_types::timing_type timing)
                   { xcp_interface.set_timing_parameter(timing_id, timing); });

        /* XCPInterface dequeue method. */
        this->bind(STR(GET_CTO),
                   [this]() -> XCPDTOPacket
                   { return this->dequeue_cto(); });

        /* XCPInterface standard methods. */
        this->bind(STR(INITIALIZE_HARDWARE),
                   [&xcp_interface]()
                   { xcp_interface.initialize_hardware(); });
        this->bind(STR(DE_INITIALIZE_HARDWARE),
                   [&xcp_interface](bool wait_for_completion)
                   { xcp_interface.de_initialize_hardware(wait_for_completion); });

        this->bind(STR(CONNECT_COMMAND_CODE),
                   [&xcp_interface](XCP::CONNECT::MODE mode)
                   { xcp_interface.connect(mode); });
        /* XCP protocol standard methods. */
        //this->bind(STR(DISCONNECT),
        //           [&xcp_interface](XCPCTODisconnectPack &cto)
        //           { xcp_interface.disconnect(dynamic_cast<XCPCTODisconnectPack &>(cto)); });
    }

    ~XCPServer() = default;

    XCPDTOPacket dequeue_cto()
    {
        XCPDTOPacket packet;
        auto dto = xcp_interface_.dequeue_dto();
        if (dto)
        {
            packet.status = dto->get_status();
            packet.buffer = dto->get_buffer();
            packet.valid = true;
        }
        else
        {
            packet.valid = false;
        }

        return packet;
    }
};

int main(int argc, char *argv[])
{
    auto api = PEAKAPI();
    auto can_interface = CANInterfacePEAK(1, 500000);
    auto xcp_interface = XCPPEAKInterface(api);
    auto server = XCPServer(can_interface, xcp_interface, 8080);

    server.run();

    return 0;
}
