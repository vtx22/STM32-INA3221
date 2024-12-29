#include "INA3221.hpp"

INA3221::INA3221(I2C_HandleTypeDef *hi2c, uint8_t address) : _hi2c(hi2c), _address(address)
{
}

void INA3221::set_shunt_resistor(INA3221_CHANNEL channel, float resistance)
{
    _resistances[(int)channel] = resistance;
}