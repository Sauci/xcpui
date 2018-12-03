//
// Created by sauci on 30/11/2018.
//

#ifndef XCPUI_CAN_INTERFACE_EXCEPTION_H
#define XCPUI_CAN_INTERFACE_EXCEPTION_H

#include <string>

#include "can_interface_types.h"

template<typename T>
class can_interface_exception : public std::exception
{
protected:

    T value_;
    char buffer_[1024];

    virtual const std::string &get_prefix() const = 0;

    virtual const std::string &get_allowed_range() const = 0;

public:

    explicit can_interface_exception(T value) : std::exception(), value_(value), buffer_()
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
    public can_interface_exception<can_types::baud_rate_type>
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

    using can_interface_exception::can_interface_exception;
};

class invalid_hardware_channel_error
    : public can_interface_exception<can_types::channel_type>
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

    using can_interface_exception::can_interface_exception;
};

#endif //XCPUI_CAN_INTERFACE_EXCEPTION_H
