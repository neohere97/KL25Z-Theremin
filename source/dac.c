/***************************************************************************
 * PES Final Project
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides, Dean & White Textbook
 *			   Most peripherals configurations are from Dean Text book
			   and lecture slides
 ***************************************************************************/

#include "dac.h"
#include <stdint.h>
#include <stdio.h>
#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include <string.h>

#define ANALOG_OUT_PIN (30) 
#define SYS_CLOCK (48E6)

static uint16_t samples_buffer[BUFF_SIZE];
static uint32_t count = 0;

extern volatile uint8_t continue_playing_flag;

// ------------------------------------------------init_dac----------------------------------------------------------
/***********************************************************************************
 * function : initialize dac peripheral
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_dac()
{
	// send clocks to  dac0 and porte
	SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// Set pin 30 as analog function
	PORTE->PCR[ANALOG_OUT_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[ANALOG_OUT_PIN] |= PORT_PCR_MUX(3);

	// Resetting and Enabling dac
	DAC0->C1 = 0;
	DAC0->C2 = 0;
	DAC0->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
}

// ------------------------------------------------init_dma----------------------------------------------------------
/***********************************************************************************
 * function : initialize dma peripheral
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_dma()
{
	// send clocks to dma and dmamux
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	DMAMUX0->CHCFG[0] = 0x00;

	// Enable interrupt, increment SAR, destination and source 16-bits, single transfer
	DMA0->DMA[0].DCR = DMA_DCR_EINT_MASK | DMA_DCR_SINC_MASK | DMA_DCR_SSIZE(2) | DMA_DCR_DSIZE(2) |
					   DMA_DCR_ERQ_MASK | DMA_DCR_CS_MASK;

	// Enable Interrupts
	NVIC_SetPriority(DMA0_IRQn, 2);
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	NVIC_EnableIRQ(DMA0_IRQn);

	// Enable dmamux only when tpm0 overflow happens
	DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(54);
}
// ------------------------------------------------init_tpm0----------------------------------------------------------
/***********************************************************************************
 * function : initialize tpm0 peripheral
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_tpm0()
{
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK; // send clock to tpm0

	// Using (MCGFLLCLK/2) clock source
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);
	TPM0->SC = 0;

	// Setting the mod value to get 48Khz interrupts
	TPM0->MOD = TPM_MOD_MOD(SYS_CLOCK / (SYS_CLOCK / 1000));
	TPM0->CNT = 0;
	// UP counting mode and prescalar to 1
	TPM0->SC = TPM_SC_PS(0) | TPM_SC_CPWMS(0) | TPM_SC_CMOD(1) | TPM_SC_DMA_MASK;
}
// ------------------------------------------------buffer_data_copy----------------------------------------------------------
/***********************************************************************************
 * function : Makes a copy of the buffer data
 * parameters : none
 * return : none
 ***********************************************************************************/
void buffer_data_copy(uint16_t *sample_buf, uint32_t sample_count) 
{
	// Disable LPTPM counter
	TPM0->SC &= ~TPM_SC_CMOD_MASK;

	//making copy of the buffer and count
	count = sample_count;
	memcpy(samples_buffer, sample_buf, count * 2);

	// Incrementing LPTPM counter on every LPTPM counter clock
	TPM0->SC |= TPM_SC_CMOD(1);
}
// ------------------------------------------------start_tone----------------------------------------------------------
/***********************************************************************************
 * function : configure & Enable the DMA0 Engine
 * parameters : none
 * return : none
 ***********************************************************************************/
void start_tone()
{
	// Loading source and destination addresses
	DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t)samples_buffer);
	DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t)(&(DAC0->DAT[0])));

	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(count * 2);

	// set flag after DMA transfer completion
	DMA0->DMA[0].DSR_BCR &= ~DMA_DSR_BCR_DONE_MASK;

	// Enable DMA
	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;
}
// ------------------------------------------------DMA0_IRQHandler----------------------------------------------------------
/***********************************************************************************
 * function : Interrupt handling for DMA0 finish interrupt
 * parameters : none
 * return : none
 ***********************************************************************************/
void DMA0_IRQHandler(void)
{
	// clearing the flag and restarting the dma transfer
	DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;
	
	// Global flag to control repeated playback of tone
	if(continue_playing_flag){
	start_tone();	
	}
}
// ------------------------------------------------reset-dma---------------------------------------------------------
/***********************************************************************************
 * function : Resets DMA to initial state, turning it off
 * parameters : none
 * return : none
 ***********************************************************************************/
void reset_dma(){
	DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(54);
}
// ------------------------------------------------End----------------------------------------------------------
