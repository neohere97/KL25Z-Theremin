/***************************************************************************
 * PES Final Project
 * Author: Alexander Dean
 * Github: https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo/src
 ***************************************************************************/

#include <MKL25Z4.H>
#include "mma8451.h"
#include "i2c.h"
#include <math.h>
#include "systick.h"

// Global data variables
int16_t acc_X = 0, acc_Y = 0, acc_Z = 0;
float roll = 0.0, pitch = 0.0;

// Accelerometer Register Maps & Math macros
#define MMA_ADDR (0x3A)
#define REG_XHI (0x01)
#define REG_XLO (0x02)
#define REG_YHI (0x03)
#define REG_YLO (0x04)
#define REG_ZHI	(0x05)
#define REG_ZLO (0x06)
#define REG_WHOAMI (0x0D)
#define REG_CTRL1  (0x2A)
#define REG_CTRL4  (0x2D)
#define WHOAMI (0x1A)
#define COUNTS_PER_G (4096.0)
#define M_PI (3.14159265)

// 1 tick is 10ms so 10 ticks is 100ms
#define MILLISECONDS_100 (10) 

// accelerometer data ready flag
extern uint32_t DATA_READY;

// ------------------------------------------------init-mma--------------------------------------------------------
/***********************************************************************************
 * function : initialize accelerometer 
 * parameters : none
 * return : none
 ***********************************************************************************/
int init_mma()
{
    // set active mode, 14 bit samples and 800 Hz ODR
    i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);
    return 1;
}

// ------------------------------------------------read-full-xyz-------------------------------------------------------
/***********************************************************************************
 * function : Read acceleration in all 3 cardinal directions
 * parameters : none
 * return : none
 ***********************************************************************************/
void read_full_xyz()
{
    int i;
    uint8_t data[6];
    int16_t temp[3];

    i2c_start();
    i2c_read_setup(MMA_ADDR, REG_XHI);

    // Read five bytes in repeated mode
    for (i = 0; i < 5; i++)
    {
        data[i] = i2c_repeated_read(0);
    }
    // Read last byte ending repeated mode
    data[i] = i2c_repeated_read(1);

    for (i = 0; i < 3; i++)
    {
        temp[i] = (int16_t)((data[2 * i] << 8) | data[2 * i + 1]);
    }

    // Align for 14 bits
    acc_X = temp[0] / 4;
    acc_Y = temp[1] / 4;
    acc_Z = temp[2] / 4;
}

// ------------------------------------------------convert-xyz-to-roll-pitch-----------------------------------------------------
/***********************************************************************************
 * function : calculate roll and pitch based on acceleration data in 3 dimensions
 * parameters : none
 * return : none
 ***********************************************************************************/
void convert_xyz_to_roll_pitch(void)
{
    float ax = acc_X / COUNTS_PER_G,
          ay = acc_Y / COUNTS_PER_G,
          az = acc_Z / COUNTS_PER_G;

    roll = atan2(ay, az) * 180 / M_PI;
    pitch = atan2(ax, sqrt(ay * ay + az * az)) * 180 / M_PI;
}
// ------------------------------------------------End---------------------------------------------------------
