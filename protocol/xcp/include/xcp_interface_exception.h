//
// Created by sauci on 29/11/2018.
//

#ifndef XCPUI_XCP_INTERFACE_EXCEPTION_H
#define XCPUI_XCP_INTERFACE_EXCEPTION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "xcp_interface_types.h"

using namespace xcp_interface_types;

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

struct invalid_timing_parameter_error :
    public xcp_interface_exception<timing_id_type>
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

#endif //XCPUI_XCP_INTERFACE_EXCEPTION_H
