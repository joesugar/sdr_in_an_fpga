#include "ZPU_I2C.h"
#include "Arduino.h"

/* Constructor.
 */
ZPU_I2C::ZPU_I2C(uint32_t scl_o, uint32_t scl_i, uint32_t sda_o, uint32_t sda_i)
{
    /* Define pins and PPS.
     */
    pinMode(scl_o, OUTPUT);                 // Define FPGA pin as an output
    pinModePPS(scl_o, HIGH);                //
    outputPinForFunction(scl_o, I2C_SCL);   // Connect FPGA pin to PPS bit

    pinMode(scl_i, INPUT);                  // Define FPGA pin as an input
    inputPinForFunction(scl_i, I2C_SCL);    // Connect FPGA pin to PPS bit

    pinMode(sda_o, OUTPUT);
    pinModePPS(sda_o, HIGH);
    outputPinForFunction(sda_o, I2C_SDA);

    pinMode(sda_i, INPUT);
    inputPinForFunction(sda_i, I2C_SDA);    
}


/* Enable the core.
 */
void ZPU_I2C::begin()
{ 
    /* Enable the core and set I2C speed.
     */
    setSpeed(LO_SPEED);
    I2C_CTR = CTR_EN;
}

/* Enable the core.
 */
void ZPU_I2C::begin(uint32_t clock_speed)
{
    setSpeed(clock_speed);
    I2C_CTR = CTR_EN;
}

/* Disable the core.
 */
void ZPU_I2C::end()
{
    /* Disable the core.
     */
    I2C_CTR = 0x00;
}


/* Set I2C clock speed.
 */
void ZPU_I2C::setSpeed(uint32_t clock_speed)
{
    /* Register value calculation taken from 
     * I2C-Master Core Specification.
     */
    uint32_t reg_value = (CLOCK_SPEED / (5 * clock_speed)) - 1;
    I2C_PRERhi = (reg_value >> 8) & 0x00FF;
    I2C_PRERlo = (reg_value >> 0) & 0x00FF;
}


/* Write value to a specified register.
 */
uint32_t ZPU_I2C::write(
    uint32_t i2c_address, uint32_t register_address, uint32_t data)
{
    int state_value = 0;
 
    /* Write the device address.
     */
    if (state_value == 0)
    {
        I2C_TXR = i2c_address << 1;
        I2C_CR = CR_WR | CR_STA;
        while ((I2C_SR & SR_TIP) != 0);
        state_value = ((I2C_SR & SR_RxACK) == 0) ? 1 : 0;
    }
    
    /* Write the register to which data is to be sent.
     */
    if (state_value == 1)
    {
        I2C_TXR = register_address;
        I2C_CR = CR_WR;
        while ((I2C_SR & SR_TIP) != 0);
        state_value = ((I2C_SR & SR_RxACK) == 0) ? 2 : 1;
    }
 
    /* Write the value.
     */
    if (state_value == 2)
    {
       I2C_TXR = data;
       I2C_CR = CR_WR | CR_STO;
       while ((I2C_SR & SR_TIP) != 0);
       state_value = ((I2C_SR & SR_RxACK) == 0) ? 3 : 2;
    }
    return (state_value == 0) ? ~SUCCESS : SUCCESS;
}


/* Read value from a specified register.
 */
uint32_t ZPU_I2C::read(
    uint32_t i2c_address, uint32_t register_address, uint32_t *data_buffer)
{
    /* Read the value back from the device.
     */   
    uint32_t state_value = 0;
 
    /* Write the device device address.
     */
    if (state_value == 0)
    {
        I2C_TXR = (i2c_address << 1);
        I2C_CR = CR_WR;
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
        I2C_TXR = (i2c_address << 1) | 0x01;
        I2C_CR = CR_WR | CR_STA;
        while ((I2C_SR & SR_TIP) != 0);
        state_value = ((I2C_SR & SR_RxACK) == 0) ? 3 : 0;
    }
 
    /* Read the value.
     */
    if (state_value = 3)
    {
        I2C_CR = CR_RD | CR_STO | CR_ACK;
        while ((I2C_SR & SR_TIP) != 0);
        state_value = ((I2C_SR & SR_RxACK) == 0) ? 4 : 0;
        *data_buffer = I2C_RXR;
    }
    /* Or stop on error (state_value == 0)
     */
    else
    {
        I2C_CR = CR_STO;
        while ((I2C_SR & SR_TIP) != 0);
    }
    
    return (state_value == 0) ? ~SUCCESS : SUCCESS;
}


/* Ping a specified address.
 */
uint32_t ZPU_I2C::ping(
    uint32_t address)
{
    int state_value = 0;
 
    /* Write the device address.
     */
    if (state_value == 0)
    {
        I2C_TXR = (address << 1);
        I2C_CR = CR_WR | CR_STA;
        while ((I2C_SR & SR_TIP) != 0);
        state_value = ((I2C_SR & SR_RxACK) == 0) ? 1 : 0;
    }
 
    /* Send stop on successful ack.
     */
    if (state_value == 1)
    {
       I2C_CR = CR_STO;
       while ((I2C_SR & SR_TIP) != 0);
    }
    
    /* Return SUCCESS or FAILURE
     */
    return (state_value == 0) ? ~SUCCESS : SUCCESS;
}
