/***************************************************************************
 * Getting In Tune
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides, Dean & White Textbook
 ***************************************************************************/

#ifndef SIN_H_
#define SIN_H_

#include <stdint.h>

//Sine function specific header definitions
#define SCALE_FACTOR	(2037)
#define HALF_PI			(3200)
#define PI 				(6399)
#define TWO_PI			(12799)
#define TOTAL_STEPS 	(32)
#define STEP_VAL        (HALF_PI/TOTAL_STEPS)

// ------------------------------------------------get_sin_val----------------------------------------------------------
/***********************************************************************************
 * function : Get the sine value for a particular angle input in radians
 * parameters : radians -> input angle 
 * return : sine value
 ***********************************************************************************/
int32_t get_sin_val(int32_t x);

// ------------------------------------------------get_interpolated_value----------------------------------------------------------
/***********************************************************************************
 * function : When value is not on the look up table, this equation is used to comput the appropriate
 *            value
 * parameters : none
 * return : none
 ***********************************************************************************/
int32_t get_interpolated_value(int32_t x, int32_t x1, int32_t y1, int32_t x2, int32_t y2);

#endif
