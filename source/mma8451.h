/***************************************************************************
 * PES Final Project
 * Author: Alexander Dean
 * Github: https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo/src
 ***************************************************************************/

#ifndef MMA8451_H
#define MMA8451_H
#include <stdint.h>

// Function declarations, check c file for descriptions
int init_mma(void);
void read_full_xyz(void);
void convert_xyz_to_roll_pitch(void);

extern float roll, pitch;
extern int16_t acc_X, acc_Y, acc_Z;

#endif