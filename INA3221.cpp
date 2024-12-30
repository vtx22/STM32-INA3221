#include "INA3221.hpp"

INA3221::INA3221(I2C_HandleTypeDef *hi2c, uint8_t address) : _hi2c(hi2c), _address(address)
{
}

/*
Set the shunt resistor value
@param channel INA Channel
@param resistance Shunt resistance value [Ohm]
*/
void INA3221::set_shunt_resistor(INA3221_CHANNEL channel, float resistance)
{
    _resistances[(int)channel] = resistance;
}

/*
Set the channel filter resistance which is connected to IN- and IN+

@param channel INA Channel
@param resistance Filter resistance value (of single line) [Ohm]
*/
void INA3221::set_filter_resistor(INA3221_CHANNEL channel, float resistance)
{
    _filters[(int)channel] = resistance;
}

/*
Get the current bus voltage measurement

@param channel INA channel
@return Bus voltage of given channel [V]
*/
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

/*
Get the current shunt voltage

@param channel INA channel
@return Shunt voltage of given channel [V]
*/
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

/*
Get the raw current which is calculated by: current = shunt_voltage / shunt_resistance

No offset correction is performed. See get_current_corrected

@param channel INA channel
@return Measured raw/uncorrected current for given channel [A]
*/
float INA3221::get_current_raw(INA3221_CHANNEL channel)
{
    return get_shunt_voltage(channel) / _resistances[(int)channel];
}

/*
Get the corrected current

This method corrects the shunt voltage by estimating the bias current induced shunt voltage offset.
This method should be used for maximum accuracy, especially when low currents are measured and when filter resistors are used.
This method is slightly slower than get_current_raw.

@param channel INA channel
@return Current measurement which was corrected using the estimated shunt voltage offset [A]
*/
float INA3221::get_current_corrected(INA3221_CHANNEL channel)
{
    float Rf = _filters[(int)channel];
    float Rs = _resistances[(int)channel];
    float Ub = get_bus_voltage(channel);
    float Um = get_shunt_voltage(channel);
    float r_in = 670e3;
    float i_bias = 10e-6;

    return (Um * (Rf + r_in) - Rf * Ub) / (r_in * Rs) + i_bias;
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