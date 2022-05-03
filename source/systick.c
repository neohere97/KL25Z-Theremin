/***************************************************************************
 * PES Final Project
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides, Dean Book
 ***************************************************************************/

#include <stdint.h>
#include "systick.h"
#include "MKL25Z4.h"
#include "core_cm0plus.h"

#define SYSTICK_LOAD_VAL (30000)

//Global struct holding time
struct system_ticker {
    ticktime_t now;
    ticktime_t timer; 
}  ticker;

// ---------------------------------------Init-Systick--------------------------------------------------
/***********************************************************************************
 * function : Initializes the System Clock and global variable
 * parameters : none
 * return : none
***********************************************************************************/
void init_systick(void){
    
    
    SysTick->LOAD = (SYSTICK_LOAD_VAL); //every tick is 10ms

    //Setting the priority for the interrupt
    NVIC_SetPriority(SysTick_IRQn, 1);
    
    SysTick->VAL = 0;
    //Setting intiial now value to 0
    ticker.now = 0;    
    //Selects alternate clock source with 3MHz frequency
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}
// ---------------------------------------Reset-Timer--------------------------------------------------
/***********************************************************************************
 * function : This function sets the timer to 0
 * parameters : none
 * return : none
***********************************************************************************/
void reset_timer(void){
    ticker.timer = 0;
}
// ---------------------------------------Get-Timer--------------------------------------------------
/***********************************************************************************
 * function : This function returns the currer timer ticks
 * parameters : none
 * return : none
***********************************************************************************/
ticktime_t get_timer(void){    
    return (uint32_t)(ticker.timer);
}
// ----------------------------------------Now--------------------------------------------------------
/***********************************************************************************
 * function : This function returns now time in ms,
 * parameters : none
 * return : none
***********************************************************************************/
ticktime_t Now(void){    
    return (ticker.now);
}
// ----------------------------------------SysTick-Interrupt-Handler-------------------------------------------------------
/***********************************************************************************
 * function : This function is used to turn off all the LEDs
 * parameters : none
 * return : none
***********************************************************************************/
void SysTick_Handler(){
    ticker.now+=1;
    ticker.timer+=1;   
}
// ----------------------------------------End---------------------------------------------------------