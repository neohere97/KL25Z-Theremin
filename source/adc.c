/***************************************************************************
 * Getting In Tune
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides, Dean & White Textbook
 *             ADC Caliberation reference from
 *             https://github.com/bingdo
 ***************************************************************************/

#include "adc.h"
#include "MKL25Z4.h"

#define SYS_CLOCK (48E6)
#define HW_AVG_32 (3)

static void caliberate_adc();
// ------------------------------------------------init-tpm1-----------------------------------------------------------
/***********************************************************************************
 * function : Initialize TPM1 peripheral
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_tpm1(int sample_rate)
{
    SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK; // Send clock to tpm

    // Clear the status & count register & load mod value
    TPM1->SC = 0;
    TPM1->CNT = 0;
    TPM1->MOD = TPM_MOD_MOD(SYS_CLOCK / sample_rate);

    // presacler to 1 and counting up
    TPM1->SC = TPM_SC_PS(0) | TPM_SC_CPWMS(0);
}
// ------------------------------------------------init-tpm1-----------------------------------------------------------
/***********************************************************************************
 * function : Initialize TPM1 peripheral
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_adc()
{
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; // send clock to adc

    // Divide Ratio 1, Normal power config, 16-bit conversion & Reset Config 2 Register
    ADC0->CFG1 = ADC_CFG1_ADLPC(0) | ADC_CFG1_ADIV(0) | ADC_CFG1_ADLSMP(0) | ADC_CFG1_MODE(3) | ADC_CFG1_ADICLK(0);
    ADC0->CFG2 = 0;

    caliberate_adc();

    // Trigger by hardware enabled, DMA disabled, external pin for voltage ref
    ADC0->SC2 = ADC_SC2_ADTRG(1) | ADC_SC2_ACFE(0) | ADC_SC2_DMAEN(0) | ADC_SC2_REFSEL(0);

    // EOC interrupt disabled, select input channel 23
    ADC0->SC1[0] = ADC_SC1_AIEN(0) | ADC_SC1_DIFF(0) | ADC_SC1_ADCH(23);

    // ADC triggered with timer interrupt
    SIM->SOPT7 = SIM_SOPT7_ADC0ALTTRGEN(1) | SIM_SOPT7_ADC0TRGSEL(9);
}
// ------------------------------------------------fill-adc-buffer---------------------------------------------------------
/***********************************************************************************
 * function : Read samples from adc and fill the buffer
 * parameters : *buffer        -> Buffer to put in the samples
 *               total_samples -> total samples count
 * return : none
 ***********************************************************************************/
void fill_adc_buff(uint16_t *buff, uint32_t total_samples)
{
    int count = 0;

    // Turning TPM On
    TPM1->SC |= TPM_SC_CMOD(1);

    // Loop to fill the buffer
    // wait for conversion flag and add sample to buffer
    while (count < total_samples)
    {
        while ((ADC0->SC1[0] & ADC_SC1_COCO_MASK) == 0)
            ;
        buff[count] = ADC0->R[0];
        count++;
    }

    // Turning TPM Off
    TPM1->SC &= ~TPM_SC_CMOD_MASK;
}

// ------------------------------------------------caliberate_adc---------------------------------------------------------
/***********************************************************************************
 * function : caliberates teh adc according to the instructions from reference manual
 * parameters : none
 * return : none
 ***********************************************************************************/
void caliberate_adc()
{
    ADC0->SC2 &= ~(ADC_SC2_ADTRG_MASK);
    ADC0->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(HW_AVG_32));
    ADC0->SC3 |= ADC_SC3_CAL_MASK;

    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
        ;

    // calculating plus side caliberation
    uint16_t caliberated_val = 0x0000;

    caliberated_val = ADC0->CLP0;
    caliberated_val += ADC0->CLP1;
    caliberated_val += ADC0->CLP2;
    caliberated_val += ADC0->CLP3;
    caliberated_val += ADC0->CLP4;
    caliberated_val += ADC0->CLPS;
    caliberated_val /= 2;
    caliberated_val |= 0x8000;

    ADC0->PG = caliberated_val;

    // minus side caliberation
    caliberated_val = 0x0000;
    caliberated_val = ADC0->CLM0;
    caliberated_val += ADC0->CLM1;
    caliberated_val += ADC0->CLM2;
    caliberated_val += ADC0->CLM3;
    caliberated_val += ADC0->CLM4;
    caliberated_val += ADC0->CLMS;
    caliberated_val /= 2;
    caliberated_val |= 0x8000;

    ADC0->MG = caliberated_val;
    ADC0->SC3 &= ~(ADC_SC3_CAL_MASK);
    ADC0->SC3 &= ~(ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(HW_AVG_32));
}
// ------------------------------------------------End------------------------------------------------------------
