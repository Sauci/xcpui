//
// Created by sauci on 29/11/2018.
//

#ifndef XCPUI_XCP_PEAK_INTERFACE_TYPES_H
#define XCPUI_XCP_PEAK_INTERFACE_TYPES_H

#include <vector>

#include "peak_api_types.h"

template<typename F, typename ...ARGS>
class XCPCTOMessage : public XCPCTORequestInterface
{
    F function_;
    PEAKAPIInterface &api_;
    std::tuple<ARGS...> parameters_;
    std::vector<std::uint8_t> buffer_ = std::vector<std::uint8_t>(8);

public:

    explicit XCPCTOMessage(PEAKAPIInterface &C, F f, ARGS... args) :
        api_(C),
        function_(f),
        parameters_(std::make_tuple(args...)),
        XCPCTORequestInterface()
    {}

    void operator()() override
    {
        //if (async)
        //{
        //    std::tuple_cat(std::make_tuple(api_, api_->get_handle()), parameters_,
        //                   std::make_tuple((char *)nullptr, (BYTE)buffer_.size()));
        //}
        //else
        //{
        auto p = std::tuple_cat(std::make_tuple(&api_, api_.get_handle()), parameters_,
                                std::make_tuple(buffer_.data(), (BYTE)buffer_.size()));

        std::apply(function_, p);
        //}
    }
};

class XCPDTOMessage : public XCPDTOInterface
{
public:

    void set_status(xcp_interface_types::status_type new_status)
    {
        status = new_status;
    }
};

#endif //XCPUI_XCP_PEAK_INTERFACE_TYPES_H
