#pragma once

#include "INA3221_reg.hpp"

#include <cstdint>
#include "I2C.h"

// Use the following flags for compiling the right library, e.g.: -D STM32F1
#if defined(STM32F0)
#include "stm32f0xx_hal.h"
#elif defined(STM32F1)
#include "stm32f1xx_hal.h"
#elif defined(STM32F2)
#include "stm32f2xx_hal.h"
#elif defined(STM32F3)
#include "stm32f3xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#elif defined(STM32F7)
#include "stm32f7xx_hal.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#else
#error "Unsupported STM32 microcontroller. Make sure you build with -D STM32F1 for example!"
#endif

class INA3221
{
public:
    INA3221(I2C_HandleTypeDef *hi2c, uint8_t address);

    void set_shunt_resistor(INA3221_CHANNEL channel, float resistance);
    void set_filter_resistor(INA3221_CHANNEL channel, float resistance);

    float get_bus_voltage(INA3221_CHANNEL channel);
    float get_shunt_voltage_raw(INA3221_CHANNEL channel);
    float get_shunt_voltage_corrected(INA3221_CHANNEL channel);
    float get_shunt_voltage_offset(INA3221_CHANNEL channel);
    float get_current_raw(INA3221_CHANNEL channel);
    float get_current_corrected(INA3221_CHANNEL channel);

    uint16_t get_manufacturer_id();
    uint16_t get_die_id();

private:
    I2C_HandleTypeDef *_hi2c = nullptr;
    uint8_t _address;

    uint16_t read_reg(INA3221_REGISTER reg);

    float _resistances[3] = {0.01f, 0.01f, 0.01f};
    float _filters[3] = {0, 0, 0};
};