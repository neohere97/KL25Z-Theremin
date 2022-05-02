/***************************************************************************
 * Getting In Tune
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides, Dean & White Textbook
 ***************************************************************************/

#ifndef TONE_TO_SAMPLE_H_
#define TONE_TO_SAMPLE_H_

#include <stdint.h>

// ---------------------------------------tone_to_samples--------------------------------------------------
/***********************************************************************************
 * function : function to test the get sine val function
 * parameters :
 * return : none
 ***********************************************************************************/
size_t tone_to_samples(int freq_note, uint16_t *buffer, uint32_t buf_size);

#endif
