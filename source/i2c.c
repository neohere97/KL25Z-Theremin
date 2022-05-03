/***************************************************************************
 * PES Final Project
 * Author: Alexander Dean
 * Github: https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo/src
 ***************************************************************************/

#include <MKL25Z4.H>
#include "i2c.h"

// i2c lock global variables
int lock_detect = 0;
int i2c_lock = 0;

// I2c Macros
#define I2C_M_START 	I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP  	I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART 	I2C0->C1 |= I2C_C1_RSTA_MASK
#define I2C_TRAN			I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC				I2C0->C1 &= ~I2C_C1_TX_MASK

//Macros for wait states
#define BUSY_ACK 	    while(I2C0->S & 0x01)
#define TRANS_COMP		while(!(I2C0->S & 0x80))
#define I2C_WAIT 			i2c_wait();
#define NACK 	        I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK           I2C0->C1 &= ~I2C_C1_TXAK_MASK

// ------------------------------------------------i2c_init----------------------------------------------------------
/***********************************************************************************
 * function : initialize i2c module
 * parameters : none
 * return : none
 ***********************************************************************************/
void i2c_init(void)
{
    // send clock to i2c module and Port E
    SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
    SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);

    // set alternate funtion to i2c
    PORTE->PCR[24] |= PORT_PCR_MUX(5);
    PORTE->PCR[25] |= PORT_PCR_MUX(5);

    // set baud rate to 100K
    I2C0->F = (I2C_F_ICR(0x10) | I2C_F_MULT(0));

    // enable i2c and master mode
    I2C0->C1 |= (I2C_C1_IICEN_MASK);

    // Select high drive mode
    I2C0->C2 |= (I2C_C2_HDRS_MASK);
}

// ------------------------------------------------i2c-busy---------------------------------------------------------
/***********************************************************************************
 * function : i2c busy on the bus and resets error flags
 * parameters : none
 * return : none
 ***********************************************************************************/
void i2c_busy(void)
{
    // Start Signal
    lock_detect = 0;
    I2C0->C1 &= ~I2C_C1_IICEN_MASK;
    I2C_TRAN;
    I2C_M_START;
    I2C0->C1 |= I2C_C1_IICEN_MASK;
    // Write to clear line
    I2C0->C1 |= I2C_C1_MST_MASK;
    // set MASTER mode
    I2C0->C1 |= I2C_C1_TX_MASK;
    // Set transmit (TX) mode
    I2C0->D = 0xFF;
    while ((I2C0->S & I2C_S_IICIF_MASK) == 0U)
    {
    }
    // wait interrupt
    I2C0->S |= I2C_S_IICIF_MASK;
    // clear interrupt bit

    // Clear arbitration error flag
    I2C0->S |= I2C_S_ARBL_MASK;

    // Send start
    I2C0->C1 &= ~I2C_C1_IICEN_MASK;
    I2C0->C1 |= I2C_C1_TX_MASK;
    // Set transmit (TX) mode
    I2C0->C1 |= I2C_C1_MST_MASK;
    // START signal generated

    I2C0->C1 |= I2C_C1_IICEN_MASK;
    // Wait until start is send

    // Send stop
    I2C0->C1 &= ~I2C_C1_IICEN_MASK;
    I2C0->C1 |= I2C_C1_MST_MASK;
    I2C0->C1 &= ~I2C_C1_MST_MASK;
    // set SLAVE mode
    I2C0->C1 &= ~I2C_C1_TX_MASK;
    // Set Rx
    I2C0->C1 |= I2C_C1_IICEN_MASK;

    // Clear arbitration error & interrupt flag/
    I2C0->S |= I2C_S_IICIF_MASK;
    I2C0->S |= I2C_S_ARBL_MASK;
    lock_detect = 0;
    i2c_lock = 1;
}

// ------------------------------------------------i2c_wait----------------------------------------------------------
/***********************************************************************************
 * function : Checks the bus and lock status and waits, and updates the flag
 * parameters : none
 * return : none
 ***********************************************************************************/
void i2c_wait(void)
{
    lock_detect = 0;
    while (((I2C0->S & I2C_S_IICIF_MASK) == 0) & (lock_detect < 200))
    {
        lock_detect++;
    }
    if (lock_detect >= 200)
        i2c_busy();
    I2C0->S |= I2C_S_IICIF_MASK;
}

// ------------------------------------------------i2c_start----------------------------------------------------------
/***********************************************************************************
 * function : Send start condition for i2c
 * parameters : none
 * return : none
 ***********************************************************************************/
void i2c_start()
{
    I2C_TRAN;
    // set to transmit mode
    I2C_M_START;
    // send start
}

// ------------------------------------------------i2c-read-setup--------------------------------------------------------
/***********************************************************************************
 * function : send device and register addresses
 * parameters : dev -> Device address
 *              address -> read address location
 * return : none
 ***********************************************************************************/
void i2c_read_setup(uint8_t dev, uint8_t address)
{
    I2C0->D = dev;
    // send dev address
    I2C_WAIT

    I2C0->D = address;
    // send read address
    I2C_WAIT
    I2C_M_RSTART;
    I2C0->D = (dev | 0x1);
    // send dev address (read)
    I2C_WAIT

    I2C_REC;
    // set to receive mode
}

//  ------------------------------------------------i2c_repeated-read---------------------------------------------------------
/***********************************************************************************
 * function : Read a byte
 * parameters : isLastRead -> flag which holds last read status
 * return : none
 ***********************************************************************************/
uint8_t i2c_repeated_read(uint8_t isLastRead)
{
    uint8_t data;

    lock_detect = 0;

    // if value was read last time, send nack
    if (isLastRead)
    {
        NACK;
    }
    // else send ack
    else
    {
        ACK;
    }

    // do a dummy read, get data and send stop
    data = I2C0->D;
    I2C_WAIT

    if (isLastRead)
    {
        I2C_M_STOP;
    }
    data = I2C0->D;

    return data;
}

// ------------------------------------------------i2c-read-byte--------------------------------------------------------
/***********************************************************************************
 * function : function to read a single byte
 * parameters : dev -> device address
 *              address-> read address
 * return : byte read
 ***********************************************************************************/
uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
{
    uint8_t data;

    // Transmit mode & send start condition
    I2C_TRAN;
    I2C_M_START;
    // Send Device condition
    I2C0->D = dev;
    I2C_WAIT

    // Send Read Address
    I2C0->D = address;
    I2C_WAIT

    I2C_M_RSTART;
    I2C0->D = (dev | 0x1);
    I2C_WAIT

    // Receive Mode & Nack after read
    I2C_REC;
    NACK;

    // dummy read to get data
    data = I2C0->D;
    I2C_WAIT

    I2C_M_STOP;
    data = I2C0->D;

    return data;
}

#pragma no_inline
// ------------------------------------------------i2c-write-byte--------------------------------------------------------
/***********************************************************************************
 * function : function to write a single byte
 * parameters : dev -> device address
 *              address-> write address
 *              data -> byte to be return
 * return : none
 ***********************************************************************************/
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
{

    // setting to transmit mode
    I2C_TRAN;
    // send start condition & dev address
    I2C_M_START;
    I2C0->D = dev;
    I2C_WAIT

    // send write address and data after ack
    I2C0->D = address;
    I2C_WAIT

    I2C0->D = data;
    I2C_WAIT
    I2C_M_STOP;
}
// ------------------------------------------------End----------------------------------------------------------
