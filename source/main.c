/***************************************************************************
 * PES Final Project
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides, Dean & White Textbook
 ***************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "dac.h"
#include "systick.h"
#include "tone_to_sample.h"
#include "command_processor.h"
#include "uart0.h"
#include "i2c.h"
#include "mma8451.h"
#include <math.h>

// extern float roll, pitch;

// ------------------------------------------------Main------------------------------------------------------------
/***********************************************************************************
 * function : Initializes the peripherals and runs the main loop
 * parameters : none
 * return : none
 ***********************************************************************************/
int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    // Initialize all peripherals
    init_systick();   
    i2c_init();
    init_mma();
    uart_init();
    init_dac();
    init_dma();
    init_tpm0();

    //Start Command Processor
    start_command_processor();
    
   return 0;
}
// ------------------------------------------------End------------------------------------------------------------
