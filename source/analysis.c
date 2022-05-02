/***************************************************************************
 * Getting In Tune
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides, Dean & White Textbook
 ***************************************************************************/

#include <stdint.h>
#include "adc.h"
#include "autocorrelate.h"
#include "fsl_debug_console.h"

#define BITS_PER_SAMPLE (1) 
// ------------------------------------------------analyse-adc-samples----------------------------------------------------------
/***********************************************************************************
 * function : Analyse ADC samples and print stats to serial port
 * parameters : *samples_array -> array filled with samples to analyse
 *               num_samples   -> total number of samples
 * return : none
 ***********************************************************************************/
void analyse_adc_samples(uint16_t *samples_array, uint32_t num_samples)
{
    int i = 0;
    uint32_t max = 0;
    uint32_t total = 0;
    uint32_t min = 65536;
    uint32_t samples_per_cycle;

    // Find min and max from the adc samples
    while (i < num_samples)
    {
        if (samples_array[i] > max)
            max = samples_array[i];

        if (samples_array[i] < min)
            min = samples_array[i];

        total = total + samples_array[i]; // Total for average
        i++;
    }

    samples_per_cycle = autocorrelate_detect_period(samples_array, num_samples, BITS_PER_SAMPLE);
    PRINTF("min=%u max=%u avg=%u period=%d samples frequency=%d Hz\r\n\n", min, max, total / num_samples, samples_per_cycle, ADC_SAMPLE_FREQUENCY / samples_per_cycle);
}
// ------------------------------------------------End-----------------------------------------------------------
