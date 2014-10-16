#ifndef _ZPU_WM8731_H_
#define _ZPU_WM8731_H_

#include "ZPU_I2C.h"
#include "Arduino.h"

/* Set the base IO address to slot 14
 */
#define WM8731_SLOT 14

/* Define registers.
*/
#define WM8731_BASE IO_SLOT(WM8731_SLOT)
#define WM8731_SAMPLE_IN REGISTER(WM8731_BASE, 0)
#define WM8731_SAMPLE_OUT REGISTER(WM8731_BASE, 0)
#define WM8731_STATUS REGISTER(WM8731_BASE, 1)

/* Define PPS signal lines.
 */
#define WM8731_CLOCK     2        // Clock in from the WM8731
#define WM8731_BCLK     11        // WM8731 BCLK out from the Papilio
#define WM8731_DAC_DATA 12        // WM8731 DAC data from the Papilio
#define WM8731_DAC_LRC  13        // WM8731 DAC LRC (left/right channel) signal from the Papilio

/* Define FPGA pins
 * CLOCK_I      Connect the clock signal from the chip to this pin
 * BCLK_O       Connect the BCLK signal from the Papilio to this pin
 * DAC_DATA_O   Connect the DAC data signal from the Papilio to this pin
 * DAC_LRC_O    Connect the left/right signal from the Papilio to this pin
 */

#define DAC_FIFO_EMPTY 0x01
#define DAC_FIFO_FULL  0x02

#define WM8731_SAMPLE_RATE 44100
#define WM8731_I2C_ADDRESS  0x1A

#define LEFT_LINE_IN            (0x00)  // 0x97
#define RIGHT_LINE_IN           (0x02)  // 0x97
#define LEFT_HEADLINE_OUT       (0x04)  // 0x79
#define RIGHT_HEADLINE_OUT      (0x06)  // 0x79
#define ANALOG_AUDIO_PATH       (0x08)  // 0x30
#define DIGITAL_AUDIO_PATH      (0x0A)  // 0x00
#define POWER_DOWN              (0x0C)  // 0x00 (all power downs disabled)
#define DIGITAL_AUDIO_FORMAT    (0x0E)  // 0x13 (DSP audio format)
#define SAMPLING_CONTROL        (0x10)  // 0x20 (44100 Hz, normal mode)
#define ACTIVE_CONTROL          (0x12)  // 0x00 (not active)
#define RESET_REGISTER          (0x0F)  // None

class ZPU_WM8731
{
    public:   
        ZPU_WM8731(uint32_t clock_i, uint32_t bclk_o, uint32_t dac_data_o, uint32_t dac_lrc_o);   
        uint32_t begin(ZPU_I2C* zpu_i2c);
        void end(ZPU_I2C* zpu_i2c);
        
        uint32_t getFifoStatus();
        uint32_t dacFifoIsEmpty();
        uint32_t dacFifoIsFull();
        
        void writeSample(uint32_t left, uint32_t right);
        void writeSample(uint32_t sample);
        
    private:
};

#endif
