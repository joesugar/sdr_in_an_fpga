#include <ZPU_I2C.h>
#include <ZPU_WM8731.h>

//ZPU_I2C(uint32_t sclk_o, uint32_t sclk_i, uint32_t sdat_o, uint32_t sdat_i);
ZPU_I2C zpu_i2c(WING_A_12, WING_A_13, WING_A_14, WING_A_15);

//ZPU_WM8731(uint32_t clock_i, uint32_t bclk_o, uint32_t dac_data_o, uint32_t dac_lrc_o);
ZPU_WM8731 zpu_wm8731(WING_A_8, WING_A_9, WING_A_10, WING_A_11);

void setup()
{
    Serial.begin(57600);
    
    Serial.print("Press enter to begin...\r\n");
    while (!Serial.available());
    while (Serial.available())
    Serial.read();
    
    Serial.print("Enabling the I2C...\r\n");
    zpu_i2c.begin(5000);
    
    Serial.print("Searching for address...\r\n");
    for (uint32_t i = 0; i < 128; i++)
    {
        if (zpu_i2c.ping(i) == SUCCESS)
        {
            Serial.print("Address found at ");
            Serial.print(i);
            Serial.print("\r\n");
        }
    }
    
    Serial.print("Configuring the codec...\r\n");
    if (zpu_wm8731.begin(&zpu_i2c) != SUCCESS)
        Serial.print("Error configuring WM8731\r\n");
    Serial.print("Codec configured.\r\n");

}

uint32_t inc32 = 1486;    // 1 kHz
uint32_t acc32 = 0;
uint32_t i = 0;
void loop()
{
    if (!zpu_wm8731.dacFifoIsFull())
    {
        zpu_wm8731.writeSample(acc32, acc32);
        acc32 += inc32;
        acc32 = acc32 & 0x0000FFFF;
    }
}
