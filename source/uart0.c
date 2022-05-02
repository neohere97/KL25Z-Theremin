/***************************************************************************
 * Breakfast Serial
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides & White Textbook
 ***************************************************************************/

#include "uart0.h"
#include "MKL25Z4.h"
#include "cbfifo.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// UART Configs
#define UART_OVERSAMPLE_RATE 16
#define SYS_CLOCK 48000000
#define BAUD_RATE 38400

// Print character definitions
#define CR 0xD
#define SPACE 0x20
#define BACKSPACE 0x8

// ------------------------------------------------Uart-init-----------------------------------------------------------
/***********************************************************************************
 * function : Initialized the UART0 by configuring all the registers
 * parameters : none
 * return : none
 ***********************************************************************************/
void uart_init()
{

    uint16_t sbr;

    // Enable UART0 Clock
    SIM->SCGC4 |= SIM_SCGC4_UART0(1);

    // Enable PORTA clock
    SIM->SCGC5 |= SIM_SCGC5_PORTA(1);

    UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

    // Select MCGFLLCLK as UART0 clock
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
    SIM->SOPT2 &= ~(SIM_SOPT2_PLLFLLSEL_MASK);

    PORTA->PCR[1] |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); /* PTA1 as ALT2 (UART0) */
    PORTA->PCR[2] |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); /* PTA2 as ALT2 (UART0) */

    sbr = (uint16_t)((SYS_CLOCK) / (BAUD_RATE * UART_OVERSAMPLE_RATE));
    UART0->BDH &= ~UART0_BDH_SBR_MASK;
    UART0->BDH |= UART0_BDH_SBR(sbr >> 8); 
    // sets BDH and configures UART in 2 stop bit mode
    UART0->BDH |= UART0_BDH_SBNS(1) | UART0_BDH_LBKDIE(0) | UART0_BDH_RXEDGIE(0);
    UART0->BDL = UART0_BDL_SBR(sbr);
    UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE - 1);

    // Configure Serial Port as 8-N-1
    // (8 data bits, No parity and 1 stop bit)
    UART0->C1 = 0x00;

    // // Configure Tx/Rx Interrupts
    UART0->C2 |= UART_C2_TIE(0);  // Tx Interrupt disabled
    UART0->C2 |= UART_C2_TCIE(0); // Tx Complete Interrupt disabled
    UART0->C2 |= UART_C2_RIE(1);  // Rx Interrupt enabled
    // Configure Transmitter/Receiever
    UART0->C2 |= UART_C2_TE(1); // Tx Enabled
    UART0->C2 |= UART_C2_RE(1); // Rx Enabled

    // // Enable UART0 Interrupt
    NVIC_EnableIRQ(UART0_IRQn);
}

// ------------------------------------------------_Sys-write-----------------------------------------------------------
/***********************************************************************************
 * function : system function which is called by printf,puts,putchar. Ties printf to uart
 * parameters : *buf -> buffer to be written
 *              size -> size of the buffer
 * return : 0 if success -1 if failed
 ***********************************************************************************/
int __sys_write(int handle, char *buf, int size)
{

    // counters to keep track of no of bytes queued and remaining
    int no_bytes_queued_total = 0, no_of_bytes_queued = 0;

    // staying in the while loop till all the data is queued
    while (size)
    {

        no_of_bytes_queued = cbfifo_enqueue(TX_CBFIFO, &buf[no_bytes_queued_total], size);

        // Enabling TE interrupt after enqueue
        if (~(UART0->C2 & UART0_C2_TIE_MASK))
            UART0->C2 |= UART_C2_TIE(1);
        no_bytes_queued_total += no_of_bytes_queued;
        size -= no_of_bytes_queued;
    }
    return 0;
}

// ------------------------------------------------__sys-readc----------------------------------------------------------
/***********************************************************************************
 * function : Reads a byte from the buffer and sends it back to the upper calling function
 * parameters : none
 * return : return the data or 0 in case there is nothing to be read
 ***********************************************************************************/
int __sys_readc(void)
{
    uint8_t byte;
    if (cbfifo_dequeue(RX_CBFIFO, &byte, 1))
    {
        return byte;
    }
    else
        return 0;
}

// ------------------------------------------------UART0-IRQHandler-----------------------------------------------------------
/***********************************************************************************
 * function : Interrupt service routine to handle UART interrupts
 * parameters : none
 * return : none
 ***********************************************************************************/
void UART0_IRQHandler(void)
{
    uint8_t byte;
    if (UART0->S1 & UART0_S1_RDRF_MASK)
    { // byte ready to be read; enqueue it
        byte = UART0->D;
        cbfifo_enqueue(RX_CBFIFO, &byte, 1);
        // Echoing the received byte
        putchar(byte);

        // If the character is a backspace update the terminal
        if (byte == BACKSPACE)
        {
            putchar(SPACE);
            putchar(BACKSPACE);
        }
    }
    // transmitter interrupt enabled
    if ((UART0->C2 & UART0_C2_TIE_MASK) &&
        (UART0->S1 & UART0_S1_TDRE_MASK))
    {
        // tx buffer empty
        // dequeing 1 byte from the buffer
        if (cbfifo_dequeue(TX_CBFIFO, &byte, 1))
        { // a byte is waiting to tx
            UART0->D = byte;
        }
        else
        {
            // If the FIFO is empty, disable the interrupt
            UART0->C2 &= ~UART0_C2_TIE_MASK;
        }
    }
}

// ------------------------------------------------End------------------------------------------------------------
