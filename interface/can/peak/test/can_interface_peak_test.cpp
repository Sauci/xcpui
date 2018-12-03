//
// Created by sauci on 30/11/2018.
//

#include <gtest/gtest.h>

#include "can_interface_peak.h"
#include "can_interface_exception.h"

TEST(can_interface_exception, calling_what_on_invalid_baud_rate_error_returns_correct_string)
{
    auto error = invalid_baud_rate_error(0);
    EXPECT_EQ(std::string(error.what()),
              std::string(
                  "invalid baud rate [0]. allowed range: 5000 [baud/s] .. 1000000[baud/s]"));
}

TEST(get_baud_rate, calling_get_baud_rate_on_connected_interface_returns_the_baudrate)
{
    const can_types::baud_rate_type baud_rates[] =
        {1000000,
         800000,
         500000,
         250000,
         125000,
         100000,
         95000,
         83000,
         50000,
         47000,
         33000,
         20000,
         10000,
         5000};
    for (unsigned idx = 0x00u; idx < (sizeof(baud_rates) / sizeof(baud_rates[0])); idx++)
    {
        auto can_interface = CANInterfacePEAK(1, baud_rates[idx]);
        ASSERT_EQ(can_interface.get_baud_rate(), baud_rates[idx]);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
