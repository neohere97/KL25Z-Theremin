/***************************************************************************
 * Getting In Tune
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides, Dean & White Textbook
 ***************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "core_cm0plus.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "dac.h"
#include "systick.h"
#include "tone_to_sample.h"

// Frequency tones map
#define A5 (880)
#define A4 (440)
#define A3 (220)
#define A2 (110)
#define D5 (587)
#define D4 (293)
#define D3 (147)
#define E5 (659)
#define E4 (330)
#define E3 (165)
#define G4 (392)
#define G5 (784)
#define G6 (1568)



#define ONE_SECOND (100) // 100 count measures to 1 sec, since we get timer interrupt at appx every 10ms

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
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif

    uint16_t total_samples,time, tone_counter = 0;
    uint16_t dac_buff[BUFF_SIZE];
    uint16_t music_notes[6] = {A2,D3,G4,E5, E4, G5};

    // Initialize all peripherals
    init_systick();
    init_dac();
    init_dma();
    init_tpm0();

    reset_timer();

    while (1)
    {
        // change frequency every one second
        time = get_timer();
        if (time > ONE_SECOND)
        {
            tone_counter++;
            if (tone_counter == 6)
                tone_counter = 0;
            reset_timer();
        }

        total_samples = tone_to_samples(music_notes[tone_counter], dac_buff, BUFF_SIZE);

        // Get DMA and DAC working
        buffer_data_copy(dac_buff, total_samples);
        start_tone();
    }
    return 0;
}
// ------------------------------------------------End------------------------------------------------------------
