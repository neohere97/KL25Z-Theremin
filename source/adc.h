/***************************************************************************
 * Getting In Tune
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides, Dean & White Textbook
 ***************************************************************************/

#include <stdint.h>

#ifndef ADC_H_
#define ADC_H_

// constants definition
#define ADC_SAMPLE_FREQUENCY (96000)

// ------------------------------------------------init-tpm1-----------------------------------------------------------
/***********************************************************************************
 * function : Initialize TPM1 peripheral
 * parameters : none
 * return : none
 ***********************************************************************************/

void init_tpm1(int sample);

// ------------------------------------------------init-tpm1-----------------------------------------------------------
/***********************************************************************************
 * function : Initialize TPM1 peripheral
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_adc();

// ------------------------------------------------fill-adc-buffer---------------------------------------------------------
/***********************************************************************************
 * function : Read samples from adc and fill the buffer
 * parameters : *buffer        -> Buffer to put in the samples
 *               total_samples -> total samples count
 * return : none
 ***********************************************************************************/
void fill_adc_buff(uint16_t *buffer, uint32_t total_samples);

#endif