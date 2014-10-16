/* WN87831 ZPU_INO library
*/
#include "ZPU_WM8731.h"

/* Constructor
 */
ZPU_WM8731::ZPU_WM8731(uint32_t clock_i, uint32_t bclk_o, uint32_t dac_data_o, uint32_t dac_lrc_o)
{
    pinMode(clock_i, INPUT );                      // Define FPGA pin as an input
    inputPinForFunction(clock_i, WM8731_CLOCK);    // Connect FPGA line to PPS bit

    pinMode(bclk_o, OUTPUT);
    pinModePPS(bclk_o, HIGH);
    outputPinForFunction(bclk_o, WM8731_BCLK);
    
    pinMode(dac_data_o, OUTPUT);
    pinModePPS(dac_data_o, HIGH);
    outputPinForFunction(dac_data_o, WM8731_DAC_DATA);
    
    pinMode(dac_lrc_o, OUTPUT);
    pinModePPS(dac_lrc_o, HIGH);
    outputPinForFunction(dac_lrc_o, WM8731_DAC_LRC);
}

/* Initialize and enable the codec.
 */
uint32_t ZPU_WM8731::begin(ZPU_I2C* zpu_i2c)
{
    /* Reset the codec and load the registers.
     */
    while ((*zpu_i2c).write(WM8731_I2C_ADDRESS, RESET_REGISTER, 0x00) != SUCCESS);
    while ((*zpu_i2c).write(WM8731_I2C_ADDRESS, LEFT_LINE_IN, 0x97) != SUCCESS);
    while ((*zpu_i2c).write(WM8731_I2C_ADDRESS, RIGHT_LINE_IN, 0x97) != SUCCESS);
    while ((*zpu_i2c).write(WM8731_I2C_ADDRESS, LEFT_HEADLINE_OUT, 0x79) != SUCCESS);
    while ((*zpu_i2c).write(WM8731_I2C_ADDRESS, RIGHT_HEADLINE_OUT, 0x79) != SUCCESS);
    while ((*zpu_i2c).write(WM8731_I2C_ADDRESS, ANALOG_AUDIO_PATH, 0x30) != SUCCESS);
    while ((*zpu_i2c).write(WM8731_I2C_ADDRESS, DIGITAL_AUDIO_PATH, 0x00) != SUCCESS);
    while ((*zpu_i2c).write(WM8731_I2C_ADDRESS, POWER_DOWN, 0x00) != SUCCESS);
    while ((*zpu_i2c).write(WM8731_I2C_ADDRESS, DIGITAL_AUDIO_FORMAT, 0x13) != SUCCESS);
    while ((*zpu_i2c).write(WM8731_I2C_ADDRESS, SAMPLING_CONTROL, 0x20) != SUCCESS);
    while ((*zpu_i2c).write(WM8731_I2C_ADDRESS, ACTIVE_CONTROL, 0x01) != SUCCESS);
    return SUCCESS;
}

/* Shut down the codec.
 */
void ZPU_WM8731::end(ZPU_I2C* zpu_i2c)
{
    (*zpu_i2c).write(WM8731_I2C_ADDRESS, ACTIVE_CONTROL, 0x00);    
}

/* Return FIFO status register.
 */
uint32_t ZPU_WM8731::getFifoStatus()
{
    return WM8731_STATUS;
}

/* Return TRUE if DAC FIFO is empty.
 */
uint32_t ZPU_WM8731::dacFifoIsEmpty()
{
    return WM8731_STATUS & DAC_FIFO_EMPTY;
}

/* Return TRUE if DAC FIFO is full.
 */
uint32_t ZPU_WM8731::dacFifoIsFull()
{
    return WM8731_STATUS & DAC_FIFO_FULL;
}

/* Write a pair of samples to the DAC.
 */
void ZPU_WM8731::writeSample(uint32_t left, uint32_t right)
{
    WM8731_SAMPLE_IN = (((left & 0x0000FFFF) << 16) | ((right & 0x0000FFFF) << 0));
}

/* Write a pair of samples to the DAC.
 */
void ZPU_WM8731::writeSample(uint32_t sample)
{
    WM8731_SAMPLE_IN = sample;
}
