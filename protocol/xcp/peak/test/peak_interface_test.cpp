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

class XCPPEAKInterfaceTest : public XCPPEAKInterface
{
public:

    using XCPPEAKInterface::XCPPEAKInterface;

    const peak::BaudRateType get_baud_rate_internal() const
    {
        return XCPPEAKInterface::baud_rate_;
    }

    const peak::ChannelType get_hardware_channel_internal() const
    {
        return XCPPEAKInterface::hardware_channel_;
    }
};

struct Runner : public ::testing::Test
{
    PEAKAPIMock mock;
    XCPPEAKInterfaceTest *peak_if = nullptr;

    const xcp_interface_types::identifier_type maximum_29_bit_id = 0b00011111111111111111111111111111u;

protected:

    void SetUp() override
    {
        peak_if = new XCPPEAKInterfaceTest(mock);
    }

    void TearDown() override
    {
        delete peak_if;
    }
};

struct XCPProtocolCommandRunner : public ::testing::Test
{
    PEAKAPIMock mock;
    XCPPEAKInterfaceTest *peak_if = nullptr;

protected:

    void SetUp() override
    {
        peak_if = new XCPPEAKInterfaceTest(mock);

        EXPECT_CALL(mock, initialize_can_channel(_, _, _, _, _, _))
            .Times(1).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

        EXPECT_CALL(mock, add_slave_on_can(_, _, _, _))
            .Times(1).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

        EXPECT_CALL(mock, dequeue_packet(_, _, _, _))
            .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

        EXPECT_CALL(mock, reset_queue(_, _))
            .Times(2).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

        EXPECT_CALL(mock, remove_slave(_))
            .Times(1).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

        EXPECT_CALL(mock, uninitialize_channel(_))
            .Times(1).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

        peak_if->initialize_hardware();
    }

    void TearDown() override
    {
        peak_if->de_initialize_hardware(true);
        delete peak_if;
    }
};

TEST_F(Runner, calling_get_interface_on_pcan_usb_pro_interface_returns_correct_string)
{
    //EXPECT_EQ(peak_if->get_interface_name(), std::string("PCAN-USB Pro FD"));
}

TEST_F(Runner, calling_what_on_invalid_identifier_error_returns_correct_string)
{
    auto error = invalid_identifier_error(0);
    EXPECT_EQ(std::string(error.what()),
              std::string("invalid CAN identifier [0]. allowed range: 0x00 [-] .. 0x1FFFFFFF [-]"));
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

TEST_F(Runner, calling_set_timing_parameter_and_get_timing_parameter_set_and_get_value_correctly)
{
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_types::timing_id_type::T1,
                                                  1000));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_types::timing_id_type::T1),
              1000);
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_types::timing_id_type::T2,
                                                  1001));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_types::timing_id_type::T2),
              1001);
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_types::timing_id_type::T3,
                                                  1002));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_types::timing_id_type::T3),
              1002);
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_types::timing_id_type::T4,
                                                  1004));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_types::timing_id_type::T4),
              1004);
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_types::timing_id_type::T5,
                                                  1005));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_types::timing_id_type::T5),
              1005);
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_types::timing_id_type::T6,
                                                  1006));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_types::timing_id_type::T6),
              1006);
    EXPECT_NO_THROW(peak_if->set_timing_parameter(xcp_interface_types::timing_id_type::T7,
                                                  1007));
    EXPECT_EQ(peak_if->get_timing_parameter(xcp_interface_types::timing_id_type::T7),
              1007);
}

TEST_F(Runner, calling_initalize_hardware_with_valid_api_calls_does_not_throw_an_exception)
{
    EXPECT_CALL(mock, initialize_can_channel(_, _, _, _, _, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock, add_slave_on_can(_, _, _, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock, reset_queue(_, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock, remove_slave(_))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock, uninitialize_channel(_))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_NO_THROW(peak_if->initialize_hardware());
    peak_if->de_initialize_hardware(false);
}

TEST_F(Runner, calling_de_initalize_hardware_with_valid_api_calls_does_not_throw_an_exception)
{
    EXPECT_CALL(mock, initialize_can_channel(_, _, _, _, _, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock, add_slave_on_can(_, _, _, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock, reset_queue(_, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock, dequeue_packet(_, _, _, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock, remove_slave(_))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock, uninitialize_channel(_))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->initialize_hardware();
    EXPECT_NO_THROW(peak_if->de_initialize_hardware(false));
}

TEST_F(Runner, calling_initalize_hardware_starts_try_dequeing_dto_packets)
{
    EXPECT_CALL(mock, initialize_can_channel(_, _, _, _, _, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock, add_slave_on_can(_, _, _, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock, dequeue_packet(_, peak::QueueType::DTO, _, _))
        .Times(AtLeast(1)).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_QUEUE_EMPTY));

    EXPECT_CALL(mock, reset_queue(_, _))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock, remove_slave(_))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock, uninitialize_channel(_))
        .Times(AnyNumber()).WillRepeatedly(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_NO_THROW(peak_if->initialize_hardware());
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_NO_THROW(peak_if->de_initialize_hardware(false));
}

TEST_F(XCPProtocolCommandRunner, calling_connect_with_valid_mode_parameter_calls_connect)
{
    EXPECT_CALL(mock, connect(_, XCP::CONNECT::NORMAL, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    EXPECT_CALL(mock,
                connect(_, XCP::CONNECT::USER_DEFINED, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->connect(XCP::CONNECT::NORMAL);
    peak_if->connect(XCP::CONNECT::USER_DEFINED);
}
#if 0

TEST_F(XCPProtocolCommandRunner, calling_disconnect_calls_disconnect)
{
    EXPECT_CALL(mock, disconnect(_, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->disconnect();
}

TEST_F(XCPProtocolCommandRunner, calling_get_status_calls_get_status)
{
    EXPECT_CALL(mock, get_status(_, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->get_status();
}

TEST_F(XCPProtocolCommandRunner, calling_synch_calls_synch)
{
    EXPECT_CALL(mock, synchronize(_, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->synch();
}

TEST_F(XCPProtocolCommandRunner, calling_get_comm_mode_info_calls_get_comm_mode_info)
{
    EXPECT_CALL(mock, get_communication_mode(_, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));

    peak_if->get_comm_mode_info();
}

TEST_F(XCPProtocolCommandRunner, calling_get_id_calls_get_id)
{
    EXPECT_CALL(mock,
                get_identification(_, xcp_types::get_id::TYPE::ASCII_TEXT, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(mock,
                get_identification(_, xcp_types::get_id::TYPE::ASAM_MC2_NO_PATH, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(mock,
                get_identification(_, xcp_types::get_id::TYPE::ASAM_MC2_PATH, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(mock,
                get_identification(_, xcp_types::get_id::TYPE::ASAM_MC2_URL, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(mock,
                get_identification(_, xcp_types::get_id::TYPE::ASAM_MC2_UPLOAD, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(mock,
                get_identification(_, xcp_types::get_id::TYPE::USER_DEFINED_128, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(mock,
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
    EXPECT_CALL(mock,
                set_request(_, xcp_types::set_request::CLEAR_DAQ_REQ, _, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(mock,
                set_request(_, xcp_types::set_request::STORE_CAL_REQ, _, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(mock,
                set_request(_, xcp_types::set_request::STORE_DAQ_REQ, _, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(mock,
                set_request(_, _, 0x0000u, _, _))
        .Times(1).WillOnce(Return(peak::StatusType::DLL_XCP_ERR_OK));
    EXPECT_CALL(mock,
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

#endif

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
