//
// Created by sauci on 15/11/2018.
//

#ifndef XCPUI_XCP_INTERFACE_H
#define XCPUI_XCP_INTERFACE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <algorithm>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

#include "interface_interface.h"
#include "xcp_interface_types.h"

using namespace xcp_interface_type;

/**
 * @brief interface API defining a common behavior for all XCP devices.
 *
 * this API should be used as follow:
 * - use the set_* methods to configure the communication and protocol parameters, such as the
 *   communication channel, the baud rate the different CAN identifiers and the timeout parameters.
 * - use the initialize_hardware method to initialize the interfaced hardware with the parameters
 *   defined previously.
 * - use the XCP related methods such as connect as required by the application.
 */
struct XCPInterface : public InterfaceInterface
{
    const interface_types::interface_type get_interface_type() override
    {
        return interface_types::interface_type::XCP;
    }

    const interface_types::interface_name_type get_interface_name() override
    {
        return interface_types::interface_name_type("TODO: get dynamic value from dll");
    }

    const interface_types::hardware_channel_type get_hardware_channel_count() override
    {
        return 2;
    }

    const bool is_plugged_in() override
    {
        return true; // TODO: get dynamic value from dll
    }

    /**
     * @brief returns the master CAN identifier.
     * @returns master CAN identifier
     */
    virtual const identifier_type get_master_identifier() = 0;

    /**
     * @brief returns the slave CAN identifier.
     * @returns slave CAN identifier
     */
    virtual const identifier_type get_slave_identifier() = 0;

    /**
     * @brief returns the broadcast CAN identifier.
     * @returns broadcast CAN identifier
     */
    virtual const identifier_type get_broadcast_identifier() = 0;

    /**
     * @brief returns the baud rate value.
     * @returns baud rate value [baud/s]
     */
    virtual const baud_rate_type get_baud_rate() = 0;

    /**
     * @brief returns the hardware channel value.
     * @returns hardware channel value [-]
     */
    virtual const hardware_channel_type get_hardware_channel() = 0;

    /**
     * @brief returns the timing parameter value.
     * @param timing_parameter_id the identifier of the timing parameter (T1 .. T7)
     * @returns timing parameter value [ms]
     */
    virtual const timing_parameter_type
    get_timing_parameter(timing_parameter_id_type timing_parameter_id) = 0;

    virtual void dequeue_cto(bool &is_valid, status_type &status, buffer_type &buffer) = 0;

    /**
     * @brief sets the master CAN identifier.
     * @param identifier master CAN identifier
     * @throw invalid_identifier_error if the requested CAN identifier is invalid
     */
    virtual void set_master_identifier(identifier_type identifier) = 0;

    /**
     * @brief sets the slave CAN identifier.
     * @param identifier slave CAN identifier
     * @throw invalid_identifier_error if the requested CAN identifier is invalid
     */
    virtual void set_slave_identifier(identifier_type identifier) = 0;

    /**
     * @brief sets the broadcast CAN identifier.
     * @param identifier broadcast CAN identifier
     * @throw invalid_identifier_error if the requested CAN identifier is invalid
     */
    virtual void set_broadcast_identifier(identifier_type identifier) = 0;

    /**
     * @brief sets the communication baud rate.
     * @param baud_rate baud rate [baud/s]
     * @throw invalid_baud_rate_error if the requested baud rate is invalid
     */
    virtual void set_baud_rate(baud_rate_type baud_rate) = 0;

    /**
     * @brief sets the hardware channel.
     * @param hardware_channel hardware channel (starting from 1)
     * @throw invalid_hardware_channel_error if the requested hardware channel is invalid
     */
    virtual void set_hardware_channel(hardware_channel_type hardware_channel) = 0;

    /**
     * @brief sets the communication timeout parameter.
     * @param timing_parameter_id identifier if the timing parameter [T1 .. T7]
     * @param timing_parameter communication timeout parameter
     */
    virtual void set_timing_parameter(timing_parameter_id_type timing_parameter_id,
                                      timing_parameter_type timing_parameter) = 0;

    /**
     * @brief initializes the hardware.
     * @param placeholder placeholder parameter, not used
     * @throw invalid_command_error if any initialization command does not executes as expected
     *
     * @note this function shall be called once all communication and protocol parameters are set.
     */
    virtual void initialize_hardware() = 0;

    /**
     * @brief de-initializes the hardware.
     * @param wait_for_completion if true, this method will only return once all queued commands
     * will be executed. if false, this method will discard the queued commands and return
     * immediately.
     * @throw invalid_command_error if any de-initialization command does not executes as expected
     *
     * @note this function shall be called once initialize_hardware has been called.
     */
    virtual void de_initialize_hardware(bool wait_for_completion) = 0;

    /**
     * @brief this command establishes a continuous, logical, point-to-point connection with a slave
     * device.
     * during a running XCP session (CONNECTED) this command has no influence on any configuration
     * of the XCP slave driver.
     * a slave device does not respond to any other commands (except auto detection) unless it is in
     * the state CONNECTED.
     * with a CONNECT(Mode = Normal), the master can start an XCP communication with the slave.
     * with a CONNECT(Mode = user defined), the master can start an XCP communication with
     * the slave and at the same time tell the slave that it should go into a special (user defined)
     * mode.
     */
    virtual void connect(xcp_types::connect::MODE mode) = 0;

    /**
     * @brief this command brings the slave to the DISCONNECTED state.
     */
    virtual void disconnect() = 0;

    /**
     * @brief this command returns all current status information of the slave device. this includes
     * the status of the resource protection, pending store requests and the general status of data
     * acquisition and stimulation.
     */
    virtual void get_status() = 0;

    /**
     * @brief this command is used to synchronize command execution after timeout conditions. the
     * SYNCH command will always have a negative response with the error code ERR_CMD_SYNCH. there
     * is no other command using this error code, therefore the response to a SYNCH command may be
     * distinguished from the response to any other command.
     */
    virtual void synch() = 0;

    /**
     * @brief this command returns optional information on different communication modes supported
     * by the slave.
     */
    virtual void get_comm_mode_info() = 0;

    /**
     * @brief this command is used for automatic session configuration and for slave device
     * identification.
     */
    virtual void get_id(xcp_types::get_id::TYPE request_identification) = 0;

    /**
     * @brief this command is used to request to save to non-volatile memory.
     */
    virtual void set_request(xcp_types::set_request::MODE mode,
                             xcp_types::set_request::SESSION_CONFIGURATION_ID session_configuration_id) = 0;

    /**
     * @brief
     */
    virtual void get_seed(peak::HandleType xcp_handle, BYTE mode, BYTE resource) = 0;

    /**
     * @brief
     */
    virtual void unlock(peak::HandleType xcp_handle, BYTE key_length, BYTE *key) = 0;

    /**
     * @brief
     */
    virtual void set_mta(BYTE address_extension, DWORD address) = 0;

    /**
     * @brief
     */
    virtual void upload(BYTE number_of_elements) = 0;

    /**
     * @brief
     */
    virtual void short_upload(BYTE number_of_elements, BYTE address_extension, DWORD address) = 0;

    /**
     * @brief
     */
    virtual void build_checksum(DWORD block_size) = 0;

    /**
     * @brief
     */
    virtual void transport_layer_cmd(BYTE sub_command,
                                     BYTE *parameter_buffer,
                                     WORD parameter_buffer_length) = 0;

    /**
     * @brief
     */
    virtual void user_cmd(BYTE sub_command,
                          BYTE *parameter_buffer,
                          WORD parameter_buffer_length) = 0;

    /**
     * @brief
     */
    virtual void download(peak::HandleType xcp_handle,
                          BYTE number_of_elements,
                          BYTE *data_buffer,
                          BYTE data_buffer_length) = 0;

    /**
     * @brief
     */
    virtual void download_next(BYTE number_of_elements,
                               BYTE *data_buffer,
                               BYTE data_buffer_length) = 0;

    /**
     * @brief
     */
    virtual void download_max(BYTE *data_buffer) = 0;

    /**
     * @brief
     */
    virtual void modify_bits(BYTE shift_value, WORD and_mask, WORD xor_mask) = 0;

    /**
     * @brief
     */
    virtual void set_calibration_page(BYTE mode, peak::CalibrationPageType page) = 0;

    /**
     * @brief
     */
    virtual void get_calibration_page(BYTE mode, BYTE data_segment_number) = 0;

    /**
     * @brief
     */
    virtual void get_paging_processor_information(peak::HandleType xcp_handle) = 0;

    /**
     * @brief
     */
    virtual void get_segment_information(BYTE mode,
                                         BYTE segment_number,
                                         BYTE segment_info,
                                         BYTE mapping_index) = 0;

    /**
     * @brief
     */
    virtual void get_page_information(peak::CalibrationPageType page) = 0;

    /**
     * @brief
     */
    virtual void set_segment_mode(BYTE mode, BYTE segment_number) = 0;

    /**
     * @brief
     */
    virtual void get_segment_mode(BYTE segment_number) = 0;

    /**
     * @brief
     */
    virtual void copy_calibration_page(peak::CalibrationPageType source,
                                       peak::CalibrationPageType destination) = 0;

    /**
     * @brief
     */
    virtual void set_daq_list_pointer(WORD daq_list_number, BYTE odt_number, BYTE odt_entry) = 0;

    /**
     * @brief
     */
    virtual void write_daq_list_entry(peak::ODTEntryType entry) = 0;

    /**
     * @brief
     */
    virtual void write_daq_list_entries(BYTE number_of_elements, peak::ODTEntryType *elements) = 0;

    /**
     * @brief
     */
    virtual void set_daq_list_mode(BYTE mode, peak::DAQListConfigType configuration) = 0;

    /**
     * @brief
     */
    virtual void start_stop_daq_list(BYTE mode, WORD daq_list_number) = 0;

    /**
     * @brief
     */
    virtual void start_stop_synchronized_daq_list(BYTE mode) = 0;

    /**
     * @brief
     */
    virtual void read_daq_list_entry(peak::HandleType xcp_handle) = 0;

    /**
     * @brief
     */
    virtual void get_daq_clock(peak::HandleType xcp_handle) = 0;

    /**
     * @brief
     */
    virtual void get_daq_processor_information(peak::HandleType xcp_handle) = 0;

    /**
     * @brief
     */
    virtual void get_daq_resolution_information(peak::HandleType xcp_handle) = 0;

    /**
     * @brief
     */
    virtual void get_daq_list_mode(WORD daq_list_number) = 0;

    /**
     * @brief
     */
    virtual void get_event_channel_information(WORD event_channel_number) = 0;

    /**
     * @brief
     */
    virtual void clear_daq_list(WORD daq_list_number) = 0;

    /**
     * @brief
     */
    virtual void get_daq_list_information(WORD daq_list_number) = 0;

    /**
     * @brief
     */
    virtual void free_daq_lists(peak::HandleType xcp_handle) = 0;

    /**
     * @brief
     */
    virtual void allocate_daq_lists(WORD daq_count) = 0;

    /**
     * @brief
     */
    virtual void allocate_odt(WORD daq_list_number, BYTE odt_count) = 0;

    /**
     * @brief
     */
    virtual void allocate_odt_entry(WORD daq_list_number, BYTE odt_number, BYTE entries_count) = 0;

    /**
     * @brief
     */
    virtual void program_start(peak::HandleType xcp_handle) = 0;

    /**
     * @brief
     */
    virtual void program_clear(BYTE mode, DWORD clear_range) = 0;

    /**
     * @brief
     */
    virtual void program(BYTE number_of_elements, BYTE *data_buffer, BYTE data_buffer_length) = 0;

    /**
     * @brief
     */
    virtual void program_reset(peak::HandleType xcp_handle) = 0;

    /**
     * @brief
     */
    virtual void get_program_processor_information(peak::HandleType xcp_handle) = 0;

    /**
     * @brief
     */
    virtual void get_sector_information(BYTE mode, BYTE sector_number) = 0;

    /**
     * @brief
     */
    virtual void program_prepare(WORD code_size) = 0;

    /**
     * @brief
     */
    virtual void program_format(peak::ProgramFormatType format) = 0;

    /**
     * @brief
     */
    virtual void program_next(BYTE number_of_elements, BYTE *data_buffer, BYTE data_length) = 0;

    /**
     * @brief
     */
    virtual void program_max(BYTE *element_buffer) = 0;

    /**
     * @brief
     */
    virtual void program_verify(BYTE mode, WORD type, DWORD value) = 0;

    /**
     * @brief callback called each time the interface received a message.
     * @param type the type of the message (CTO/DTO)
     * @param message the message
     *
     * @returns boolean value to indicate if the message object should be deleted or not.
     *     @retval true: the message will be deleted
     *     @retval false: the message will not be deleted
     */
    virtual bool cto_completion_handler(message_type type, XCPMessageInterface *message) = 0;
};

template<typename T>
class xcp_interface_exception : public std::exception
{
protected:

    T value_;
    char buffer_[1024];

    virtual const std::string &get_prefix() const = 0;

    virtual const std::string &get_allowed_range() const = 0;

public:

    explicit xcp_interface_exception(T value) : std::exception(), value_(value), buffer_()
    {
    }

    const char *what() const noexcept override
    {
        auto string = this->get_prefix() + std::string(" [") + std::to_string(this->value_) +
                      std::string("]. allowed range: ") + this->get_allowed_range();
        memset((void *)&buffer_[0], 0, sizeof(buffer_) / sizeof(buffer_[0]));
        size_t size = string.size() < (sizeof(buffer_) - 1) ? string.size() : sizeof(buffer_) - 1;
        std::copy(string.begin(), string.begin() + size, (char *)&buffer_[0]);
        return &buffer_[0];
    }
};

class invalid_baud_rate_error :
    public xcp_interface_exception<baud_rate_type>
{
    std::string prefix_ = std::string("invalid baud rate");
    std::string allowed_range_ = std::string("5000 [baud/s] .. 1000000[baud/s]");

protected:

    const std::string &get_prefix() const override
    {
        return prefix_;
    }

    const std::string &get_allowed_range() const override
    {
        return allowed_range_;
    }

public:

    using xcp_interface_exception::xcp_interface_exception;
};

class invalid_identifier_error :
    public xcp_interface_exception<identifier_type>
{
    std::string prefix_ = std::string("invalid CAN identifier");
    std::string allowed_range_ = std::string("0x00 [-] .. 0x1FFFFFFF [-]");

protected:

    const std::string &get_prefix() const override
    {
        return prefix_;
    }

    const std::string &get_allowed_range() const override
    {
        return allowed_range_;
    }

public:

    using xcp_interface_exception::xcp_interface_exception;
};

class invalid_hardware_channel_error
    : public xcp_interface_exception<interface_types::hardware_channel_type>
{
    std::string prefix_ = std::string("invalid hardware channel");
    std::string allowed_range_ = std::string("1 [-] .. 2 [-]");

protected:

    const std::string &get_prefix() const override
    {
        return prefix_;
    }

    const std::string &get_allowed_range() const override
    {
        return allowed_range_;
    }

public:

    using xcp_interface_exception::xcp_interface_exception;
};

struct invalid_timing_parameter_error :
    public xcp_interface_exception<timing_parameter_id_type>
{
    std::string prefix_ = std::string("invalid timing parameter");
    std::string allowed_range_ = std::string("1 [-] .. 7 [-]");

protected:

    const std::string &get_prefix() const override
    {
        return prefix_;
    }

    const std::string &get_allowed_range() const override
    {
        return allowed_range_;
    }

public:

    using xcp_interface_exception::xcp_interface_exception;
};

struct invalid_command_error :
    public xcp_interface_exception<status_type>
{
    std::string prefix_ = std::string("invalid XCP command");
    std::string allowed_range_ = std::string("-");

protected:

    const std::string &get_prefix() const override
    {
        return prefix_;
    }

    const std::string &get_allowed_range() const override
    {
        return allowed_range_;
    }

public:

    using xcp_interface_exception::xcp_interface_exception;
};

#endif //XCPUI_XCP_INTERFACE_H
