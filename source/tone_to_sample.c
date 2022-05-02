/***************************************************************************
 * Getting In Tune
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides, Dean & White Textbook
 ***************************************************************************/

#include <stdio.h>
#include "tone_to_sample.h"
#include "fsl_debug_console.h"
#include "dac.h"
#include "sin.h"

#define DAC_PLAY_FREQ (48000)

// ---------------------------------------tone_to_samples--------------------------------------------------
/***********************************************************************************
 * function : function to test the get sine val function
 * parameters :
 * return : none
 ***********************************************************************************/
uint32_t tone_to_samples(int tone, uint16_t *buffer, uint32_t buf_size)
{
    int32_t val,rads;
    int i = 0;

    // calculating total samples required to play a tone
    int samples_per_cycle = (DAC_PLAY_FREQ + (tone / 2)) / tone;
    int total_num_samples = samples_per_cycle * (buf_size / samples_per_cycle);

    // filling the buffer with sine values
    while (i < total_num_samples)
    {
        rads = i * TWO_PI / samples_per_cycle;
        val = get_sin_val(rads) + SCALE_FACTOR;
        buffer[i] = val;
        i++;
    }

    PRINTF("Generated %d samples at %d Hz; computed period=%d samples\r\n",
           total_num_samples, tone, samples_per_cycle);

    return total_num_samples;
}
// ---------------------------------------End--------------------------------------------------
