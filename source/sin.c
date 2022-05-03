/***************************************************************************
 * PES Final Project
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides, Dean & White Textbook
 *             linear interpolation refernce from codesansar.com
 ***************************************************************************/

#include <stdint.h>
#include "dac.h"
#include "sin.h"

// lookup table generated using an online generator
static const int16_t lookup_table[TOTAL_STEPS + 1] =
    {0, 100, 200, 299, 397, 495, 591, 686, 780, 871, 960, 1047,
     1132, 1214, 1292, 1367, 1441, 1509, 1575, 1636, 1694, 1747,
     1797, 1842, 1882, 1917, 1949, 1976, 1998, 2015, 2027, 2035,
     2037};

// ------------------------------------------------get_interpolated_value----------------------------------------------------------
/***********************************************************************************
 * function : When value is not on the look up table, this equation is used to comput the appropriate
 *            value
 * parameters : none
 * return : none
 ***********************************************************************************/
int32_t get_interpolated_value(int32_t x0, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    // Standard slope equation, calculate slope and find value at coordinates
    int32_t xdiff, ydiff;
    xdiff = x2 - x1;
    ydiff = y2 - y1;

    return ((((xdiff) / 2) + ((x0 - x1) * (ydiff))) / (xdiff) + y1);
}

// ------------------------------------------------get_sin_val----------------------------------------------------------
/***********************************************************************************
 * function : Get the sine value for a particular angle input in radians
 * parameters : radians -> input angle
 * return : sine value
 ***********************************************************************************/
int32_t get_sin_val(int32_t input_val)
{

    int32_t lookup_index;
    int sign = 1; // flag to keep track of the input angle sign

    // If Radian value is not between -PI and PI, adjusting the values based on
    // sin(x + 2PI) = sin(x)
    while (input_val < -PI)
        input_val += TWO_PI;

    while (input_val > PI)
        input_val -= TWO_PI;

    // Using the relation sin(-x)=-sin(x) when rad is negative
    if (input_val < 0)
    {
        input_val = -input_val;
        sign = -1;
    }

    // Using the relation sin(x)=sin(PI-x), adjust radians if x>(PI/2)
    if (input_val > HALF_PI)
        input_val = PI - input_val;

    // calculating x1, to find if it matches the lookup index available
    lookup_index = input_val / STEP_VAL;
    int32_t x1 = lookup_index * STEP_VAL;

    // no interpolation if x1 and rad are equal
    if (x1 == input_val)
        return sign * lookup_table[lookup_index];

    // In case interpolation is required, it is calculated below and the result is sent
    int32_t x2 = (lookup_index + 1) * STEP_VAL;
    int32_t y0 = get_interpolated_value(input_val, x1, lookup_table[lookup_index], x2, lookup_table[lookup_index + 1]);

    int32_t result = sign * y0;
    return (result);
}
// ------------------------------------------------End----------------------------------------------------------
