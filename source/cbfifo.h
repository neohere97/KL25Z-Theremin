/***************************************************************************
 * Breakfast Serial
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides & White Textbook
 ***************************************************************************/

#ifndef _CBFIFO_H_
#define _CBFIFO_H_

#include <stdlib.h>  

//Macro definitions for TX and RX buffers
#define TX_CBFIFO 0
#define RX_CBFIFO 1

/***********************************************************************************
 * function : Enqueues bytes into the buffer
 * parameters : buff_no -> Macro defs, RX or TX Buffer
 *              *buf    -> Pointer to buffer source
 *              nbytes  -> No of bytes to be enqueued
 * return : No of bytes enqueued
 ***********************************************************************************/
size_t cbfifo_enqueue(int buff_no,void *buf, size_t nbyte);


/***********************************************************************************
 * function : Dequeues bytes into the buffer
 * parameters : buff_no -> Macro defs, RX or TX Buffer
 *              *buf    -> Pointer to destination buffer
 *              nbytes  -> No of bytes to be dequeued
 * return : No of bytes to be dequeued
 ***********************************************************************************/
size_t cbfifo_dequeue(int buff_no,void *buf, size_t nbyte);

/***********************************************************************************
 * function : Returns the CBFIFO Length 
 * parameters : buff_no -> Macro defs, RX or TX Buffer *             
 * return : length of the buffer
 ***********************************************************************************/
size_t cbfifo_length(int buff_no);

/***********************************************************************************
 * function : Returns the CBFIFO capacity 
 * parameters : none             
 * return : capacity of the buffer
 ***********************************************************************************/
size_t cbfifo_capacity();


/***********************************************************************************
 * function : Resets the buffer indexes
 * parameters : buff_no -> Macro defs, RX or TX Buffer *             
 * return : None
 ***********************************************************************************/
void cbfifo_reset(int buff_no);



#endif 
// -----------------------------------------------------------END-----------------------------------------------------------------------
