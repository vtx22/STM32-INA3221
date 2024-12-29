#include "INA3221.hpp"

INA3221::INA3221(I2C_HandleTypeDef *hi2c, uint8_t address) : _hi2c(hi2c), _address(address)
{
}

void INA3221::set_shunt_resistor(INA3221_CHANNEL channel, float resistance)
{
    _resistances[(int)channel] = resistance;
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