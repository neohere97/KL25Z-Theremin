/***************************************************************************
 * Breakfast Serial
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides & White Textbook & PES Assignment 2
 ***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "MKL25Z4.h"
#include "cbfifo.h"

#define CB_SIZE 255

// These are the core buffer definitions, since the buffer is of fixed size
// CB_SIZE has been statically defined to be 255
// The core definitions have been extended to support 2 cirular buffers
uint8_t cb_buffer[2][CB_SIZE];
uint8_t read_index[2] = {0, 0};
uint8_t write_index[2] = {0, 0};
uint8_t end_index[2] = {CB_SIZE, CB_SIZE};

// -------------------------------------------------------CB-FIFO-ENQUEUE-----------------------------------------------------------------------
/***********************************************************************************
 * function : Enqueues bytes into the buffer
 * parameters : buff_no -> Macro defs, RX or TX Buffer
 *              *buf    -> Pointer to buffer source
 *              nbytes  -> No of bytes to be enqueued
 * return : No of bytes enqueued
 ***********************************************************************************/
size_t cbfifo_enqueue(int buff_no, void *buf, size_t nbyte)
{
    //disabling interrupt since this is a critical seciton of the code
    NVIC_DisableIRQ(UART0_IRQn);
    // checking if the input buffer data is null
    if (buf == NULL)
        return -1;
    // type casting the data to uint8_t to make it compatible and generic
    uint8_t *data = buf;
    int i;
    // only when write index is behind end index, data is enqueued
    for (i = 0; i < nbyte && write_index[buff_no] < end_index[buff_no]; i++)
    {
        cb_buffer[buff_no][write_index[buff_no]++] = *data++;
    }    
    NVIC_EnableIRQ(UART0_IRQn);
    return (size_t)i;
}
// -------------------------------------------------------CB-FIFO-DEQUEUE-----------------------------------------------------------------------
/***********************************************************************************
 * function : Dequeues bytes into the buffer
 * parameters : buff_no -> Macro defs, RX or TX Buffer
 *              *buf    -> Pointer to destination buffer
 *              nbytes  -> No of bytes to be dequeued
 * return : No of bytes to be dequeued
 ***********************************************************************************/
size_t cbfifo_dequeue(int buff_no, void *buf, size_t nbyte)
{
    //disabling interrupt, critical section of the code
    NVIC_DisableIRQ(UART0_IRQn);
    // null buffer pointer check
    if (buf == NULL)
        return -1;

    uint8_t *data = buf;

    int i;

    for (i = 0; i < nbyte; i++)
    {
        // When read_index equals write index all the elements have been dequeued
        if (read_index[buff_no] == write_index[buff_no])
        {
            goto next_step;
        }
        // writing the data back into the write buffer from the cb buffer
        *data++ = cb_buffer[buff_no][i];
        // incrementing the read buffer to account for dequeued elements
        read_index[buff_no]++;
    }
    // In this step the buffer is reset such that remaining elements are brought back to the
    // beginning of the buffer. Alternate algorithm to doing the wrapped subtraction.
next_step:
    if (write_index[buff_no] > read_index[buff_no])
    {
        size_t temp_write_index = write_index[buff_no];
        for (int j = 0; j < temp_write_index - read_index[buff_no]; j++)
        {
            cb_buffer[buff_no][j] = cb_buffer[buff_no][j + read_index[buff_no]];
        }
        // resetting the write index to the new buffer which has been reset
        write_index[buff_no] = write_index[buff_no] - read_index[buff_no];
    }
    // In case of write_index being same as the read index, all the elements have been dequeued and both point at the first location
    else if (write_index[buff_no] == read_index[buff_no])
    {
        write_index[buff_no] = 0;
    }
    // resetting read index to 0 everytime since remaining elements are brought back to starting from the first location
    read_index[buff_no] = 0;

    NVIC_EnableIRQ(UART0_IRQn);
    return (size_t)i;
}
// -------------------------------------------------------CB-FIFO-LENGTH---------------------------------------------------------------------
/***********************************************************************************
 * function : Returns the CBFIFO Length 
 * parameters : buff_no -> Macro defs, RX or TX Buffer *             
 * return : length of the buffer
 ***********************************************************************************/
size_t cbfifo_length(int buff_no)
{
    // because of the dequeue reset function everytime the length of the buffer will be same as the write index
    return (size_t)(write_index[buff_no]);
}
// -------------------------------------------------------CB-FIFO-CAPACITY-----------------------------------------------------------------------
/***********************************************************************************
 * function : Returns the CBFIFO capacity 
 * parameters : none             
 * return : capacity of the buffer
 ***********************************************************************************/
size_t cbfifo_capacity()
{
    // buffer size has been statically allocated to be 128 bytes in commons.h
    return (size_t)CB_SIZE;
}
// -------------------------------------------------------CB-FIFO-RESET---------------------------------------------------------------------
/***********************************************************************************
 * function : Resets the buffer indexes
 * parameters : buff_no -> Macro defs, RX or TX Buffer *             
 * return : None
 ***********************************************************************************/
void cbfifo_reset(int buff_no)
{
    read_index[buff_no] = 0;
    write_index[buff_no] = 0;
}
// -----------------------------------------------------------END-----------------------------------------------------------------------
