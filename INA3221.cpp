#include "INA3221.hpp"

INA3221::INA3221(I2C_HandleTypeDef *hi2c, uint8_t address) : _hi2c(hi2c), _address(address)
{
}

void INA3221::set_shunt_resistor(INA3221_CHANNEL channel, float resistance)
{
    _resistances[(int)channel] = resistance;
}

void INA3221::set_filter_resistor(INA3221_CHANNEL channel, float resistance)
{
    _filters[(int)channel] = resistance;
}

float INA3221::get_bus_voltage(INA3221_CHANNEL channel)
{
    switch (channel)
    {
    case INA3221_CHANNEL::CHANNEL_1:
        return read_reg(INA3221_REGISTER::C1_BUS_VOLTAGE) * 0.001;
        break;
    case INA3221_CHANNEL::CHANNEL_2:
        return read_reg(INA3221_REGISTER::C2_BUS_VOLTAGE) * 0.001;
        break;
    case INA3221_CHANNEL::CHANNEL_3:
        return read_reg(INA3221_REGISTER::C3_BUS_VOLTAGE) * 0.001;
        break;
    }
}

float INA3221::get_shunt_voltage(INA3221_CHANNEL channel)
{
    uint16_t raw = 0;

    switch (channel)
    {
    case INA3221_CHANNEL::CHANNEL_1:
        raw = read_reg(INA3221_REGISTER::C1_SHUNT_VOLTAGE);
        break;
    case INA3221_CHANNEL::CHANNEL_2:
        raw = read_reg(INA3221_REGISTER::C2_SHUNT_VOLTAGE);
        break;
    case INA3221_CHANNEL::CHANNEL_3:
        raw = read_reg(INA3221_REGISTER::C3_SHUNT_VOLTAGE);
        break;
    }

    if (raw & 0x8000)
    {
        uint16_t complement = (~raw) + 1;
        return -(complement * 5e-6);
    }

    return raw * 5e-6;
}

float INA3221::get_current_raw(INA3221_CHANNEL channel)
{
    return get_shunt_voltage(channel) / _resistances[(int)channel];
}

uint16_t INA3221::get_manufacturer_id()
{
    return read_reg(INA3221_REGISTER::MANUFACTURER_ID);
}

uint16_t INA3221::get_die_id()
{
    return read_reg(INA3221_REGISTER::DIE_ID);
}

uint16_t INA3221::read_reg(INA3221_REGISTER reg)
{
    uint16_t data = read_i2c_reg_16(_hi2c, _address, (uint8_t)reg);
    return (data >> 8) + (data << 8);
}