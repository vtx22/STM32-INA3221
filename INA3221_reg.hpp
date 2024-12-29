#pragma once

#include <cstdint>

enum class INA3221_REGISTER
{
    CONFIG = 0x00,

    C1_SHUNT_VOLTAGE = 0x01,
    C1_BUS_VOLTAGE = 0x02,

    C2_SHUNT_VOLTAGE = 0x03,
    C2_BUS_VOLTAGE = 0x04,

    C3_SHUNT_VOLTAGE = 0x05,
    C4_BUS_VOLTAGE = 0x06,

    C1_CRIT_LIMIT = 0x07,
    C1_WARN_LIMIT = 0x08,
    C1_SHUNT_VOLTAGE = 0x09,
    C1_BUS_VOLTAGE = 0x0A,
    C1_SHUNT_VOLTAGE = 0x0B,
    C1_BUS_VOLTAGE = 0x0C,

    SHUNT_VOL_SUM = 0x0D,
    SHUNT_VOL_SUM_LIM = 0x0E,

    MASK_ENABLE = 0x0F,

    PV_UPPER_LIMIT = 0x10,
    PV_LOWER_LIMIT = 0x11,

    MANUFACTURER_ID = 0xFE,
    DIE_ID = 0xFF,

};

enum class INA3221_CHANNEL
{
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,
};