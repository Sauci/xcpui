//
// Created by sauci on 03/12/2018.
//

#include "usb_interface_peak.h"

#define UNDEFINED_STRING "undefined"

static HINSTANCE dll = nullptr;

TPCANStatus (__stdcall *c_can_get_value)(TPCANHandle, TPCANParameter, void *, std::uint32_t);

USBInterfacePEAK::USBInterfacePEAK()
{
#ifdef _WIN64
    std::string dir = std::string("x64");
#else
    std::string dir = std::string("Win32");
#endif

    std::string file_path = __FILE__;
    std::string cd = file_path.substr(0, file_path.rfind('\\')) + std::string(R"(\..\lib\)");
    auto dll_path = cd + dir + std::string("\\PCANBasic.dll");
    dll = LoadLibrary(dll_path.c_str());

    c_can_get_value = (decltype(c_can_get_value))GetProcAddress(dll, "CAN_GetValue");
}

USBInterfacePEAK::~USBInterfacePEAK()
{
    FreeLibrary(dll);
    dll = nullptr;
}

const HINSTANCE &USBInterfacePEAK::get_dll() const
{
    return dll;
}

const usb_types::interface_name_type USBInterfacePEAK::get_interface_name()
{
    usb_types::interface_name_type interface_name(128, 0);

    auto status = get_value(TPCANHandle::CHANNEL_BUS_USB_1,
                            TPCANParameter::PCAN_HARDWARE_NAME,
                            interface_name.data(),
                            interface_name.size());
    if (status != TPCANStatus::PCAN_ERROR_OK)
    {
        return usb_types::interface_name_type(UNDEFINED_STRING);
    }
    else
    {
        return interface_name.substr(0, interface_name.find_last_not_of('\0') + 1);;
    }
}

const bool USBInterfacePEAK::is_plugged_in()
{
    return get_interface_name() != usb_types::interface_name_type(UNDEFINED_STRING);
}

TPCANStatus USBInterfacePEAK::get_value(TPCANHandle channel,
                                        TPCANParameter parameter,
                                        void *data,
                                        std::uint32_t size)
{
    return c_can_get_value(channel, parameter, data, size);
}
