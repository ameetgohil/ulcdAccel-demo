// Library for our MMA8452Q 3-axis accelerometer
// Based on the MMA8452Q Arduino Library

#include "mbed.h"
#include "MMA8452Q.h"

// Constructor
MMA8452Q::MMA8452Q(PinName sda, PinName scl, int addr) : m_i2c(sda, scl), m_addr(addr)
{
    //Initialize members
    scale = DEFAULT_FSR;
}
    
// Destructor
MMA8452Q::~MMA8452Q()
{
}

//Initialization
bool MMA8452Q::init()
{
    //Check to make sure the chip's ID matches the factory ID
    uint8_t c = readRegister(REG_WHO_AM_I);
    if( c != FACTORY_ID ) {
        return false;
    }
    
    //Set default scale and data rate
    standby();
    setScale(DEFAULT_FSR);
    setODR(DEFAULT_ODR);
    active();
    
    return true;
}

// Set the full-scale range for x,y, and z data
void MMA8452Q::setScale(uint8_t fsr)
{
    uint8_t config = readRegister(REG_XYZ_DATA_CFG);
    scale = fsr;
    config &= 0xFC; //Mask out FSR bits
    fsr = fsr >> 2; //Trick to translate scale to FSR bits
    fsr &= 0x03; //Mask out acceptable FSRs
    config |= fsr; //Write FSR bits to config byte
    writeRegister(REG_XYZ_DATA_CFG, config);  // write config back to register
}

//Set the Output Data Rate
void MMA8452Q::setODR(uint8_t odr)
{
    uint8_t ctrl = readRegister(REG_CTRL_REG1);
    ctrl &= 0xCF; //Mask out data rate bits
    odr &= 0x07; //Mask out acceptable ODRs
    ctrl |= (odr << 3); //Write ODR bits to control byte
    writeRegister(REG_CTRL_REG1, ctrl); //write control back to register
}

// Set accelerometer into standby mode
void MMA8452Q::standby()
{
    uint8_t c = readRegister(REG_CTRL_REG1);
    c &= ~(0x01); //Clear bit 0 to go into standby
    writeRegister(REG_CTRL_REG1, c); //Write back to CONTROL register
}

// Set accelerometer inito active mode
void MMA8452Q::active()
{
    uint8_t c = readRegister(REG_CTRL_REG1);
    c |= 0x01; // Set bit 0 to go into active mode
    writeRegister(REG_CTRL_REG1, c); // Write back to CONTROL register
}

// Read X registers
float MMA8452Q::readX()
{
    int16_t x = 0;
    float cx = 0;
    
    // Read MSB and LSB from X registers
    x = readRegister(OUT_X_MSB);
    x = x<<8;
    x |= readRegister(OUT_X_LSB);
    x = x>>4;
    
    //Calculate human readable X
    cx = (float)x / (float)2048 * (float)(scale);
    
    return cx;
}

// Read Y registers
float MMA8452Q::readY()
{
    int16_t y = 0;
    float cy = 0;
    
    // Read MSB and LSB from Y registers
    y = readRegister(OUT_Y_MSB);
    y = y<<8;
    y |= readRegister(OUT_Y_LSB);
    y = y>>4;
    
    //Calculate human readable Y
    cy = (float)y / (float)2048 * (float)(scale);
    
    return cy;
}

// Read Z registers
float MMA8452Q::readZ()
{
    int16_t z = 0;
    float cz = 0;
    
    // Read MSB and LSB from Z registers
    z = readRegister(OUT_Z_MSB);
    z = z<<8;
    z |= readRegister(OUT_Z_LSB);
    z = z>>4;
    
    //Calculate human readable Z
    cz = (float)z / (float)2048 * (float)(scale);
    
    return cz;
}

uint8_t MMA8452Q::readRegister(uint8_t reg)
{
    uint8_t dev_addr;
    uint8_t data;
    
    // I2C address are bits [6..1] in the transmitted byte, so we shift by 1
    dev_addr = m_addr <<1;
    
    //Write device address with a trailing 'write' bit
    m_i2c.start();
    m_i2c.write(dev_addr & 0xFE);
    
    //Write register address
    m_i2c.write(reg);
    
    //Write a start bit and device addresss with a trailing 'read' bit
    m_i2c.start();
    m_i2c.write(dev_addr | 0x01);
    
    //Read single byte from  I2C device
    data = m_i2c.read(0);
    m_i2c.stop();
    
    return data;
}


void MMA8452Q::writeRegister(uint8_t reg, uint8_t data)
{
    uint8_t dev_addr;
    
    //I2C address are bits [6..1] in transmitted byte, so we shift by 1
    dev_addr = m_addr << 1;
    
    //Write device address with a trailing 'write' bit
    m_i2c.start();
    m_i2c.write(dev_addr & 0xFE);
    
    //Write register address
    m_i2c.write(reg);
    
    //Write the data the register
    m_i2c.write(data);
    m_i2c.stop();
    
}

        