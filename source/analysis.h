/***************************************************************************
 * Getting In Tune
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides, Dean & White Textbook
 ***************************************************************************/

#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include <stdint.h>

// ------------------------------------------------analyse-adc-samples----------------------------------------------------------
/***********************************************************************************
 * function : Analyse ADC samples and print stats to serial port
 * parameters : *samples_array -> array filled with samples to analyse
 *               num_samples   -> total number of samples
 * return : none
 ***********************************************************************************/
void analyse_adc_samples(uint16_t *samples_array, uint32_t num_samples);

#endif 
