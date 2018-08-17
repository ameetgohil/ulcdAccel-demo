// Library for our MMA8452Q 3-axis accelerometer
// Based on the MMA8452Q Arduino Library

#ifndef MMA8452Q_H
#define MMA8452Q_H

#include "mbed.h"

// Register definitions
#define REG_STATUS          0x00
#define OUT_X_MSB           0x01
#define OUT_X_LSB           0x02
#define OUT_Y_MSB           0x03
#define OUT_Y_LSB           0x04
#define OUT_Z_MSB           0x05
#define OUT_Z_LSB           0x06
#define REG_WHO_AM_I        0x0D
#define REG_XYZ_DATA_CFG    0x0E
#define REG_CTRL_REG1       0x2A

// WHO_AM_I check
#define FACTORY_ID          0x2A

// Scale definitions
#define SCALE_2G            2
#define SCALE_4G            4
#define SCALE_8G            8

//Data rates
#define ODR_800HZ           0
#define ODR_400HZ           1
#define ODR_200HZ           2
#define ODR_100HZ           3
#define ODR_50HZ            4
#define ODR_12_5HZ          5
#define ODR_6_25HZ          6
#define ODR_1_56HZ          7

//Init values
#define DEFAULT_FSR         SCALE_2G
#define DEFAULT_ODR         ODR_800HZ

// Class declaration
class MMA8452Q
{
    public:
        MMA8452Q(PinName sdda, PinName scl, int addr);
        ~MMA8452Q();
        bool init();
        uint8_t available();
        void setScale(uint8_t fsr);
        void setODR(uint8_t odr);
        void standby();
        void active();
        float readX();
        float readY();
        float readZ();
        uint8_t readRegister(uint8_t reg);
        void writeRegister(uint8_t reg, uint8_t data);
        
    private:
        I2C m_i2c;
        int m_addr;
        int scale;
};
    
#endif