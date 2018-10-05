//
// Created by sauci on 15/11/2018.
//

#include <chrono>
#include <cstdint>
#include <gtest/gtest.h>
#include <thread>

#include "peak_api_mock.h"
#include "xcp_peak_interface.h"

using ::testing::_;
using ::testing::Return;
using ::testing::AnyNumber;
using ::testing::AtLeast;

struct XCPPEAKInterfaceTest : public XCPPEAKInterface
{
    PEAKAPIMock mock = PEAKAPIMock();

    XCPPEAKInterfaceTest() : XCPPEAKInterface(mock)
    {
    }

    PEAKAPIMock *api() const
    {
        return (PEAKAPIMock *)XCPPEAKInterface::api_;
    }

    const peak::BaudRateType get_baud_rate_internal() const
    {
        return baud_rate_;
    }

    const peak::ChannelType get_hardware_channel_internal() const
    {
        return hardware_channel_;
    }
};

struct Runner : public ::testing::Test
{
    XCPPEAKInterfaceTest *peak_if;

    const xcp_api_types::identifier_type maximum_29_bit_id = 0b00011111111111111111111111111111u;

protected:

    void SetUp() override
    {
        peak_if = new XCPPEAKInterfaceTest();
    }

    void TearDown() override
    {
        delete peak_if;
    }
};

struct XCPProtocolCommandRunner : public ::testing::Test
{
    XCPPEAKInterfaceTest *peak_if;

protected:

    void SetUp() override
    {
        peak_if = new XCPPEAKInterfaceTest();

        EXPECT_CALL(*peak_if->api(), initialize_can_channel(_, _, _, _, _, _))
            .Times(1).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

        EXPECT_CALL(*peak_if->api(), add_slave_on_can(_, _, _, _))
            .Times(1).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

        EXPECT_CALL(*peak_if->api(), dequeue_packet(_, _, _, _))
            .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

        EXPECT_CALL(*peak_if->api(), reset_queue(_, _))
            .Times(2).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

        EXPECT_CALL(*peak_if->api(), remove_slave(_))
            .Times(1).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

        EXPECT_CALL(*peak_if->api(), uninitialize_channel(_))
            .Times(1).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

        peak_if->initialize_hardware();
    }

    void TearDown() override
    {
        peak_if->de_initialize_hardware(true);
        delete peak_if;
    }
};

TEST_F(Runner, calling_get_interface_type_on_pcan_usb_pro_interface_returns_xcp)
{
    EXPECT_EQ(peak_if->get_interface_type(), interface_types::interface_type::XCP);
}

TEST_F(Runner, calling_get_interface_on_pcan_usb_pro_interface_returns_correct_string)
{
    //EXPECT_EQ(peak_if->get_interface_name(), std::string("PCAN-USB Pro FD"));
}

TEST_F(Runner, calling_get_hardware_channel_count_on_pcan_usb_pro_interface_returns_2)
{
    EXPECT_EQ(peak_if->get_hardware_channel_count(), 2);
}

TEST_F(Runner, calling_what_on_invalid_baud_rate_error_returns_correct_string)
{
    auto error = invalid_baud_rate_error(0);
    EXPECT_EQ(std::string(error.what()),
              std::string(
                  "invalid baud rate [0]. allowed range: 5000 [baud/s] .. 1000000[baud/s]"));
}

TEST_F(Runner, calling_what_on_invalid_identifier_error_returns_correct_string)
{
    auto error = invalid_identifier_error(0);
    EXPECT_EQ(std::string(error.what()),
              std::string("invalid CAN identifier [0]. allowed range: 0x00 [-] .. 0x1FFFFFFF [-]"));
}

TEST_F(Runner, calling_what_on_invalid_hardware_channel_error_returns_correct_string)
{
    auto error = invalid_hardware_channel_error(0);
    EXPECT_EQ(std::string(error.what()),
              std::string("invalid hardware channel [0]. allowed range: 1 [-] .. 2 [-]"));
}

TEST_F(Runner, calling_what_on_invalid_command_error_returns_correct_string)
{
    auto error = invalid_command_error(0);
    EXPECT_EQ(std::string(error.what()), std::string("invalid XCP command [0]. allowed range: -"));
}

TEST_F(Runner, calling_set_master_identifier_with_29_bit_identifier_sets_master_identifier)
{
    EXPECT_NO_THROW(peak_if->set_master_identifier(0));
    EXPECT_EQ(peak_if->get_master_identifier(), 0);

    EXPECT_NO_THROW(peak_if->set_master_identifier(maximum_29_bit_id));
    EXPECT_EQ(peak_if->get_master_identifier(), maximum_29_bit_id);
}

TEST_F(Runner, calling_set_master_identifier_with_invalid_29_bit_identifier_throws_an_exception)
{
    EXPECT_THROW(peak_if->set_master_identifier(maximum_29_bit_id + 1), invalid_identifier_error);
}

TEST_F(Runner, calling_set_slave_identifier_with_29_bit_identifier_sets_slave_identifier)
{
    EXPECT_NO_THROW(peak_if->set_slave_identifier(0));
    EXPECT_EQ(peak_if->get_slave_identifier(), 0);

    EXPECT_NO_THROW(peak_if->set_slave_identifier(maximum_29_bit_id));
    EXPECT_EQ(peak_if->get_slave_identifier(), maximum_29_bit_id);
}

TEST_F(Runner, calling_set_slave_identifier_with_invalid_29_bit_identifier_throws_an_exception)
{
    EXPECT_THROW(peak_if->set_slave_identifier(maximum_29_bit_id + 1), invalid_identifier_error);
}

TEST_F(Runner, calling_set_broadcast_identifier_with_29_bit_identifier_sets_broadcast_identifier)
{
    EXPECT_NO_THROW(peak_if->set_broadcast_identifier(0));
    EXPECT_EQ(peak_if->get_broadcast_identifier(), 0);

    EXPECT_NO_THROW(peak_if->set_broadcast_identifier(maximum_29_bit_id));
    EXPECT_EQ(peak_if->get_broadcast_identifier(), maximum_29_bit_id);
}

TEST_F(Runner, calling_set_broadcast_identifier_with_invalid_29_bit_identifier_throws_an_exception)
{
    EXPECT_THROW(peak_if->set_broadcast_identifier(maximum_29_bit_id + 1),
                 invalid_identifier_error);
}

TEST_F(Runner, calling_set_baud_rate_and_get_baud_rate_set_and_get_value_correctly)
{
    EXPECT_NO_THROW(peak_if->set_baud_rate(5000));
    EXPECT_EQ(peak_if->get_baud_rate(), 5000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_5000);
    EXPECT_NO_THROW(peak_if->set_baud_rate(10000));
    EXPECT_EQ(peak_if->get_baud_rate(), 10000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_10000);
    EXPECT_NO_THROW(peak_if->set_baud_rate(20000));
    EXPECT_EQ(peak_if->get_baud_rate(), 20000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_20000);
    EXPECT_NO_THROW(peak_if->set_baud_rate(33000));
    EXPECT_EQ(peak_if->get_baud_rate(), 33000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_33000);
    EXPECT_NO_THROW(peak_if->set_baud_rate(47000));
    EXPECT_EQ(peak_if->get_baud_rate(), 47000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_47000);
    EXPECT_NO_THROW(peak_if->set_baud_rate(50000));
    EXPECT_EQ(peak_if->get_baud_rate(), 50000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_50000);
    EXPECT_NO_THROW(peak_if->set_baud_rate(83000));
    EXPECT_EQ(peak_if->get_baud_rate(), 83000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_83000);
    EXPECT_NO_THROW(peak_if->set_baud_rate(95000));
    EXPECT_EQ(peak_if->get_baud_rate(), 95000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_95000);
    EXPECT_NO_THROW(peak_if->set_baud_rate(100000));
    EXPECT_EQ(peak_if->get_baud_rate(), 100000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_100000);
    EXPECT_NO_THROW(peak_if->set_baud_rate(125000));
    EXPECT_EQ(peak_if->get_baud_rate(), 125000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_125000);
    EXPECT_NO_THROW(peak_if->set_baud_rate(250000));
    EXPECT_EQ(peak_if->get_baud_rate(), 250000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_250000);
    EXPECT_NO_THROW(peak_if->set_baud_rate(500000));
    EXPECT_EQ(peak_if->get_baud_rate(), 500000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_500000);
    EXPECT_NO_THROW(peak_if->set_baud_rate(800000));
    EXPECT_EQ(peak_if->get_baud_rate(), 800000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_800000);
    EXPECT_NO_THROW(peak_if->set_baud_rate(1000000));
    EXPECT_EQ(peak_if->get_baud_rate(), 1000000);
    EXPECT_EQ(peak_if->get_baud_rate_internal(), peak::BaudRateType::BAUD_RATE_1000000);
}

TEST_F(Runner, calling_set_baud_tate_with_invalid_baud_rate_throws_an_exception)
{
    EXPECT_THROW(peak_if->set_baud_rate(0), invalid_baud_rate_error);
}

TEST_F(Runner, calling_set_hardware_channel_and_get_hardware_channel_set_and_get_value_correctly)
{
    EXPECT_NO_THROW(peak_if->set_hardware_channel(1));
    EXPECT_EQ(peak_if->get_hardware_channel(), 1);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_1);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(2));
    EXPECT_EQ(peak_if->get_hardware_channel(), 2);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_2);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(3));
    EXPECT_EQ(peak_if->get_hardware_channel(), 3);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_3);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(4));
    EXPECT_EQ(peak_if->get_hardware_channel(), 4);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_4);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(5));
    EXPECT_EQ(peak_if->get_hardware_channel(), 5);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_5);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(6));
    EXPECT_EQ(peak_if->get_hardware_channel(), 6);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_6);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(7));
    EXPECT_EQ(peak_if->get_hardware_channel(), 7);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_7);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(8));
    EXPECT_EQ(peak_if->get_hardware_channel(), 8);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_8);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(9));
    EXPECT_EQ(peak_if->get_hardware_channel(), 9);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_9);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(10));
    EXPECT_EQ(peak_if->get_hardware_channel(), 10);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_10);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(11));
    EXPECT_EQ(peak_if->get_hardware_channel(), 11);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_11);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(12));
    EXPECT_EQ(peak_if->get_hardware_channel(), 12);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_12);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(13));
    EXPECT_EQ(peak_if->get_hardware_channel(), 13);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_13);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(14));
    EXPECT_EQ(peak_if->get_hardware_channel(), 14);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_14);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(15));
    EXPECT_EQ(peak_if->get_hardware_channel(), 15);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_15);
    EXPECT_NO_THROW(peak_if->set_hardware_channel(16));
    EXPECT_EQ(peak_if->get_hardware_channel(), 16);
    EXPECT_EQ(peak_if->get_hardware_channel_internal(), peak::ChannelType::CHANNEL_BUS_USB_16);
}

TEST_F(Runner, calling_set_hardware_channel_with_invalid_hardware_channel_throws_an_exception)
{
    EXPECT_THROW(peak_if->set_hardware_channel(0), invalid_hardware_channel_error);
}

TEST_F(Runner, calling_set_timing_parameter_and_get_timing_parameter_set_and_get_value_correctly)
{
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_type::timing_parameter_id_type::T1,
                                                  1000));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_type::timing_parameter_id_type::T1),
              1000);
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_type::timing_parameter_id_type::T2,
                                                  1001));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_type::timing_parameter_id_type::T2),
              1001);
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_type::timing_parameter_id_type::T3,
                                                  1002));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_type::timing_parameter_id_type::T3),
              1002);
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_type::timing_parameter_id_type::T4,
                                                  1004));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_type::timing_parameter_id_type::T4),
              1004);
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_type::timing_parameter_id_type::T5,
                                                  1005));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_type::timing_parameter_id_type::T5),
              1005);
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_type::timing_parameter_id_type::T6,
                                                  1006));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_type::timing_parameter_id_type::T6),
              1006);
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_type::timing_parameter_id_type::T7,
                                                  1007));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_type::timing_parameter_id_type::T7),
              1007);
}

TEST_F(Runner, calling_initalize_hardware_with_valid_api_calls_does_not_throw_an_exception)
{
    EXPECT_CALL(*peak_if->api(), initialize_can_channel(_, _, _, _, _, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(*peak_if->api(), add_slave_on_can(_, _, _, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(*peak_if->api(), reset_queue(_, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(*peak_if->api(), remove_slave(_))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(*peak_if->api(), uninitialize_channel(_))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_NO_THROW(peak_if->initialize_hardware());
    peak_if->de_initialize_hardware(false);
}

TEST_F(Runner, calling_de_initalize_hardware_with_valid_api_calls_does_not_throw_an_exception)
{
    EXPECT_CALL(*peak_if->api(), initialize_can_channel(_, _, _, _, _, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(*peak_if->api(), add_slave_on_can(_, _, _, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(*peak_if->api(), reset_queue(_, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(*peak_if->api(), remove_slave(_))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(*peak_if->api(), uninitialize_channel(_))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->initialize_hardware();
    EXPECT_NO_THROW(peak_if->de_initialize_hardware(false));
}

TEST_F(Runner, calling_initalize_hardware_starts_try_dequeing_dto_packets)
{
    EXPECT_CALL(*peak_if->api(), initialize_can_channel(_, _, _, _, _, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(*peak_if->api(), add_slave_on_can(_, _, _, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(*peak_if->api(), dequeue_packet(_, peak::QueueType::DTO, _, _))
        .Times(AtLeast(1)).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_QUEUE_EMPTY));

    EXPECT_CALL(*peak_if->api(), reset_queue(_, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(*peak_if->api(), remove_slave(_))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(*peak_if->api(), uninitialize_channel(_))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_NO_THROW(peak_if->initialize_hardware());
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_NO_THROW(peak_if->de_initialize_hardware(false));
}

TEST_F(XCPProtocolCommandRunner, calling_connect_with_valid_mode_parameter_calls_connect)
{
    EXPECT_CALL(*peak_if->api(), connect(_, xcp_types::connect::MODE::NORMAL, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(*peak_if->api(),
                connect(_, xcp_types::connect::MODE::USER_DEFINED, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->connect(xcp_types::connect::MODE::NORMAL);
    peak_if->connect(xcp_types::connect::MODE::USER_DEFINED);
}

TEST_F(XCPProtocolCommandRunner, calling_disconnect_calls_disconnect)
{
    EXPECT_CALL(*peak_if->api(), disconnect(_, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->disconnect();
}

TEST_F(XCPProtocolCommandRunner, calling_get_status_calls_get_status)
{
    EXPECT_CALL(*peak_if->api(), get_status(_, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->get_status();
}

TEST_F(XCPProtocolCommandRunner, calling_synch_calls_synch)
{
    EXPECT_CALL(*peak_if->api(), synchronize(_, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->synch();
}

TEST_F(XCPProtocolCommandRunner, calling_get_comm_mode_info_calls_get_comm_mode_info)
{
    EXPECT_CALL(*peak_if->api(), get_communication_mode(_, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->get_comm_mode_info();
}

TEST_F(XCPProtocolCommandRunner, calling_get_id_calls_get_id)
{
    EXPECT_CALL(*peak_if->api(),
                get_identification(_, xcp_types::get_id::TYPE::ASCII_TEXT, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(*peak_if->api(),
                get_identification(_, xcp_types::get_id::TYPE::ASAM_MC2_NO_PATH, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(*peak_if->api(),
                get_identification(_, xcp_types::get_id::TYPE::ASAM_MC2_PATH, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(*peak_if->api(),
                get_identification(_, xcp_types::get_id::TYPE::ASAM_MC2_URL, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(*peak_if->api(),
                get_identification(_, xcp_types::get_id::TYPE::ASAM_MC2_UPLOAD, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(*peak_if->api(),
                get_identification(_, xcp_types::get_id::TYPE::USER_DEFINED_128, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(*peak_if->api(),
                get_identification(_, xcp_types::get_id::TYPE::USER_DEFINED_255, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->get_id(xcp_types::get_id::TYPE::ASCII_TEXT);
    peak_if->get_id(xcp_types::get_id::TYPE::ASAM_MC2_NO_PATH);
    peak_if->get_id(xcp_types::get_id::TYPE::ASAM_MC2_PATH);
    peak_if->get_id(xcp_types::get_id::TYPE::ASAM_MC2_URL);
    peak_if->get_id(xcp_types::get_id::TYPE::ASAM_MC2_UPLOAD);
    peak_if->get_id(xcp_types::get_id::TYPE::USER_DEFINED_128);
    peak_if->get_id(xcp_types::get_id::TYPE::USER_DEFINED_255);
}

TEST_F(XCPProtocolCommandRunner, calling_set_request_calls_set_request)
{
    EXPECT_CALL(*peak_if->api(),
                set_request(_, xcp_types::set_request::CLEAR_DAQ_REQ, _, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(*peak_if->api(),
                set_request(_, xcp_types::set_request::STORE_CAL_REQ, _, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(*peak_if->api(),
                set_request(_, xcp_types::set_request::STORE_DAQ_REQ, _, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(*peak_if->api(),
                set_request(_, _, 0x0000u, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(*peak_if->api(),
                set_request(_, _, 0xFFFFu, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->set_request(xcp_types::set_request::CLEAR_DAQ_REQ, 1);
    peak_if->set_request(xcp_types::set_request::STORE_CAL_REQ, 2);
    peak_if->set_request(xcp_types::set_request::STORE_DAQ_REQ, 3);
    peak_if->set_request(xcp_types::set_request::STORE_DAQ_REQ |
                         xcp_types::set_request::CLEAR_DAQ_REQ, 0x0000u);
    peak_if->set_request(xcp_types::set_request::STORE_DAQ_REQ |
                         xcp_types::set_request::CLEAR_DAQ_REQ, 0xFFFFu);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
