/***************************************************************************
 * PES Final Project
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides, Dean & White Textbook
 ***************************************************************************/

#include <stdint.h>


#ifndef SYSTICK_H_
#define SYSTICK_H_

typedef uint32_t ticktime_t;

// ---------------------------------------Init-Systick--------------------------------------------------
/***********************************************************************************
 * function : Initializes the System Clock and global variable
 * parameters : none
 * return : none
***********************************************************************************/
void init_systick();

// ----------------------------------------Now--------------------------------------------------------
/***********************************************************************************
 * function : This function returns now time in ms,
 * parameters : none
 * return : none
***********************************************************************************/
ticktime_t Now();

// ---------------------------------------Reset-Timer--------------------------------------------------
/***********************************************************************************
 * function : This function sets the timer to 0
 * parameters : none
 * return : none
***********************************************************************************/
void reset_timer();

// ---------------------------------------Get-Timer--------------------------------------------------
/***********************************************************************************
 * function : This function returns the currer timer ticks
 * parameters : none
 * return : none
***********************************************************************************/
ticktime_t get_timer();

#endif 
