#include "ZPU_I2C.h"
#include "Arduino.h"

ZPU_I2C::ZPU_I2C()
{
    setSpeed(LOW_SPEED_CLOCK);
}

void ZPU_I2C::setSpeed(uint32_t clock_speed)
{
    /* Register value calculation taken from 
     * I2C-Master Core Specification.
     */
    uint32_t reg_value = (clock_speed == LOW_SPEED_CLOCK) ?
        (CLOCK_SPEED / (5 * LO_SPEED)) - 1 :
        (CLOCK_SPEED / (5 * HI_SPEED)) - 1;
        
    I2C_PRERhi = (reg_value >> 8) & 0x00FF;
    I2C_PRERlo = (reg_value >> 0) & 0x00FF;
}

uint32_t ZPU_I2C::write(
    uint32_t address, uint32_t register_address, uint32_t data)
{
    int state_value = 0;
 
    /* Write the device address.
     */
    if (state_value == 0)
    {
        I2C_TXR = address << 1;
        I2C_CR = CR_WR | CR_STA;
        while ((I2C_SR & SR_TIP) != 0);
        state_value = ((I2C_SR & SR_RxACK) == 0) ? 1 : 0;
    }
    
    /* Write the register to which data is to be sent.
     */
    if (state_value == 1)
    {
        I2C_TXR = R0;
        I2C_CR = CR_WR;
        while ((I2C_SR & SR_TIP) != 0);
        state_value = ((I2C_SR & SR_RxACK) == 0) ? 2 : 0;
    }
 
    /* Write the value.
     */
    if (state_value == 2)
    {
       I2C_TXR = data;
       I2C_CR = CR_WR | CR_STO;
       while ((I2C_SR & SR_TIP) != 0);
       state_value = ((I2C_SR & SR_RxACK) == 0) ? 3 : 0;
    }
    return SUCCESS;
}

uint32_t ZPU_I2C::read(
    uint32_t address, uint32_t register_address, uint32_t *data_buffer)
{
    /* Read the value back from the device.
     */   
    uint32_t state_value = 0;
 
    /* Write the device device address.
     */
    if (state_value == 0)
    {
        I2C_TXR = (address << 1);
        I2C_CR = CR_WR | CR_STA;
        while ((I2C_SR & SR_TIP) != 0);
        state_value = ((I2C_SR & SR_RxACK) == 0) ? 1 : 0;
    }
    
    /* Write the register from which data is to be read.
     */
    if (state_value == 1)
    {
        I2C_TXR = register_address;
        I2C_CR = CR_WR;
        while ((I2C_SR & SR_TIP) != 0);
        state_value = ((I2C_SR & SR_RxACK) == 0) ? 2 : 0;
    }
 
    /* Write the device read address.
     */
    if (state_value == 2)
    {
        I2C_TXR = (address << 1) | 0x01;
        I2C_CR = CR_WR | CR_STA;
        while ((I2C_SR & SR_TIP) != 0);
        state_value = ((I2C_SR & SR_RxACK) == 0) ? 3 : 0;
    }
 
    /* Read the value.
     */
    uint32_t value = 0;
    if (state_value = 3)
    {
        I2C_CR = CR_RD | CR_STO | CR_ACK;
        while ((I2C_SR & SR_TIP) != 0);
        value = I2C_RXR;
        state_value = 4;
    }
    
    /* Stop on error.
     */
    if (state_value == 0)
    {
        I2C_CR = CR_STO;
        while ((I2C_SR & SR_TIP) != 0);
    }
    *data_buffer = value;
    return SUCCESS;
}
