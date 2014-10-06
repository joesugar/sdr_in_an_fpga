#ifndef _ZPU_I2C_H_
#define _ZPU_I2C_H

#include "Arduino.h"

/* Set the base IO address to slot 15
 */
#define I2C_SLOT 15

/* Define PPS signal lines.
 */
#define I2C_SCL 9
#define I2C_SDA 10

/* Define I2C registers.
 */
#define I2C_BASE IO_SLOT(I2C_SLOT)
#define I2C_PRERlo REGISTER(I2C_BASE, 0)
#define I2C_PRERhi REGISTER(I2C_BASE, 1)
#define I2C_CTR REGISTER(I2C_BASE, 2)
#define I2C_TXR REGISTER(I2C_BASE, 3)
#define I2C_RXR REGISTER(I2C_BASE, 3)
#define I2C_CR REGISTER(I2C_BASE, 4)
#define I2C_SR REGISTER(I2C_BASE, 4)
 
/* Define FPGA pins
 */
#define I2C_SCL_O WING_C_7
#define I2C_SCL_I WING_C_6
#define I2C_SDA_O WING_C_5
#define I2C_SDA_I WING_C_4
 
/* I2C core control register bit definitions.
 */
#define CTR_EN 0x80 // i2c core enable bit.
// When set to ‘1’, the core is enabled.
// When set to ‘0’, the core is disabled.
 
#define CTR_IEN 0x40 // i2c core interrupt enable bit.
// When set to ‘1’, interrupt is enabled.
// When set to ‘0’, interrupt is disabled.
 
/* I2C core command register bit definitions.
 */
#define CR_STA  0x80 // generate (repeated) start condition
#define CR_STO  0x40 // generate stop condition
#define CR_RD   0x20 // read from slave
#define CR_WR   0x10 // write to slave
#define CR_ACK  0x08 // when a receiver, send ACK (ACK = ‘0’) or NACK (ACK = ‘1’)
#define CR_IACK 0x01 // interrupt acknowledge. When set, clears a pending interrupt.
 
/* I2C status register bit definitions.
 */
#define SR_RxACK 0x80   // received acknowledge from slave.
// This flag represents acknowledge from the addressed slave.
// ‘1’ = No acknowledge received
// ‘0’ = Acknowledge received
 
#define SR_BUSY 0x40    // i2c bus busy
// ‘1’ after START signal detected
// ‘0’ after STOP signal detected
 
#define SR_AL 0x20      // arbitration lost
// This bit is set when the core lost arbitration.
// Arbitration is lost when:
// a STOP signal is detected, but non requested
// The master drives SDA high, but SDA is low.
 
#define SR_TIP 0x02     // transfer in progress.
// ‘1’ when transferring data
// ‘0’ when transfer complete
 
#define SR_IF 0x01      // interrupt Flag. This bit is set when an
// interrupt is pending, which will cause a
// processor interrupt request if the IEN bit is set.
// The Interrupt Flag is set when:
// one byte transfer has been completed
// arbitration is lost
 
/* I2C core register definitions.
 */
#define R0 0x00
#define R1 0x02
#define R2 0x04
#define R3 0x06
#define R4 0x08
#define R5 0x0A
#define R6 0x0C
#define R7 0x0E
#define R8 0x10
#define R9 0x12
 
/* I2C core read/write flags.
 */
#define I2C_M_WR 0x00
#define I2C_M_RD 0x01
 
/* Define I2C device address.
 */
#define HIGH_SPEED_CLOCK  1
#define LOW_SPEED_CLOCK   0

#define CLOCK_SPEED 96000000
#define LO_SPEED      100000
#define HI_SPEED      400000

#define SUCCESS 0

class ZPU_I2C
{
    public:   
        ZPU_I2C();
        
        void setSpeed(uint32_t fast);
        uint32_t write(uint32_t address, uint32_t registerAddress, uint32_t data);
        uint32_t read(uint32_t address, uint32_t register, uint32_t *dataBuffer);
        
    private:
        
};
    
#endif
