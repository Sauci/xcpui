//
// Created by sauci on 30/11/2018.
//

#include <gtest/gtest.h>

#include "usb_interface_peak.h"

TEST(get_interface_name, calling_get_interface_name_on_connected_pcanusbpro_returns_the_name)
{
    auto usb_interface = new USBInterfacePEAK();
    ASSERT_EQ(usb_interface->get_interface_name(), std::string("PCAN-USB Pro FD"));
    delete usb_interface;
}

TEST(is_plugged_in, calling_is_plugged_in_on_connected_pcanusbpro_returns_true)
{
    auto usb_interface = new USBInterfacePEAK();
    ASSERT_EQ(usb_interface->is_plugged_in(), true);
    delete usb_interface;
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
