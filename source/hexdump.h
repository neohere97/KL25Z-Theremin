/***************************************************************************
 * Breakfast Serial
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides & White Textbook & Assignment 1 (Hexdump)
 ***************************************************************************/
#ifndef HEXDUMP_
#define HEXDUMP

#include <stdlib.h>  
/***********************************************************************************
 * function : Generates hexdump string with location and number of bytes
 * parameters : *str -> destination buffer to store string
 *              size -> size of the destination buffer
 *              *loc -> Starting location for the dump
 *              nbytes -> Number of bytes to be dumped
 * return : pointer to the string destination
 ***********************************************************************************/
char *hexdump(char *str, size_t size, const void *loc, size_t nbytes);

#endif