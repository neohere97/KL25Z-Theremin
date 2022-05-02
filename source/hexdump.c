/***************************************************************************
 * Breakfast Serial
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides & White Textbook & Assignment 1 (Hexdump)
 ***************************************************************************/

#include "hexdump.h"
#include "fsl_debug_console.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>


static int uint_to_hexstr(char *str, size_t size, uint32_t num, uint8_t nbits);
// ------------------------------------------------hexdump-from-Assignment1----------------------------------------------------------
/***********************************************************************************
 * function : Generates hexdump string with location and number of bytes
 * parameters : *str -> destination buffer to store string
 *              size -> size of the destination buffer
 *              *loc -> Starting location for the dump
 *              nbytes -> Number of bytes to be dumped
 * return : pointer to the string destination
 ***********************************************************************************/

char *hexdump(char *str, size_t size, const void *loc, size_t nbytes)
{

  // Dereferencing the void pointer, declared as const becasue the parameter is also const
  const uint8_t *dump_loc = loc;

  // Initializing variables
  int total_memory_needed = 0;
  int total_rows = 0;

  // copy of nbytes
  int nbytes_copy = nbytes;
  int i;
  // calculating total rows using nbytes
  for (i = 1; i <= nbytes_copy; i++)
  {
    if (i == 1)
    {
      total_rows++;
    }
    if (i == 16)
    {
      i = 0;
      nbytes_copy = nbytes_copy - 16;
    }
  }

  // Calculating the total bytes required
  total_memory_needed = (total_rows * 7) + (nbytes * 3);
  // counter to write to the final string
  int finalStringCount = 0;

  if (size >= total_memory_needed && nbytes > 0)
  {
    // first column address counter
    int address_offset_counter = (int)loc;
    // initializing more counters and format variables
    int total_bytes_to_read_write = nbytes;
    int bytes_this_row = 0;

    // Temporary storage for Hex converted characters
    char off[100];
    char data[100];

    // Main For-Loop to go through all the rows
    for (int j = 0; j < total_rows; j++)
    {
      // converting address offset value to hex by calling the uint_to_hexstr function previously written
      // nbits is 16 to get 4  digit hex representation of address
      uint_to_hexstr(off, 100, address_offset_counter, 32);
      // ir is a general use counter variable to read through hex characters and adding to final string
      int ir = 0;
      while (off[ir] != '\0')
      {
        str[finalStringCount++] = off[ir++];
      }
      // Adding a space after writing the address column
      str[finalStringCount++] = ' ';

      // Assigning number of bytes to read_Write per row
      if (total_bytes_to_read_write > 16)
      {
        bytes_this_row = 16;
        total_bytes_to_read_write -= 16;
      }
      else
      {
        // If remaining bytes are less than 16, program will finish in this row
        bytes_this_row = total_bytes_to_read_write;
      }
      // Reading throguh bytes of memory location
      for (int i = 0; i < bytes_this_row; i++)
      {
        // conversion of raw binary data to hex strings
        uint_to_hexstr(data, 100, *dump_loc++, 8);
        // ir is 2 to not include the '0x' which is by default included by the uint to hexstr function
        ir = 2;
        while (data[ir] != '\0')
        {
          str[finalStringCount++] = data[ir++];
        }
        // Adding a space after the hex string if it's not the 16th byte in the row
        if (i + 1 != bytes_this_row)
          str[finalStringCount++] = ' ';
      }
      // Adding a new line character after 16 bytes in the row
      if (bytes_this_row == 16 && j + 1 != total_rows)
      {
        str[finalStringCount++] = '\n';
        str[finalStringCount++] = '\r';
      }
      // Incrementing the address offset counter by 16bytes
      address_offset_counter += 16;
    }
    // Adding null character to the end of the final string which needs to be returned
    str[finalStringCount++] = '\0';
    return str;
  }
  else
  {
    // Setting string to  null in case of error and returning the same
    str[0] = '\0';
    return str;
  }
}
// ------------------------------------------------uint-to-hexstr-Assignment-1--------------------------------------------------------
/***********************************************************************************
 * function : converts uint to hex string
 * parameters : *str -> string buffer to store the converted hex string
 *              size -> size of the buffer
 *               num -> number to be converted
 *             nbits -> number of bits in the hex string
 * return : none
 ***********************************************************************************/
static int uint_to_hexstr(char *str, size_t size, uint32_t num, uint8_t nbits)
{

  char hex_reverse_array[nbits];
  int count = 0;
  // Checking if the size is enough to represent Hex characters
  if ((nbits >> 2) + 3 > size)
  {
    goto error;
  }
  // Checking if string pointer is null
  if (str == NULL)
    return -1;
  // Checking if nbits is one of the specific value, 4,8,16,32
  else if (nbits == 4 || nbits == 8 || nbits == 16 || nbits == 32)
  {
    while (num > 0)
    {
      // Extracting last 4 bits from the least significant byte by
      // using the Bitwise AND operation with 0xF
      uint32_t res = num & 0xF;

      if (res <= 9)
      {
        // Adding characters using ascii for numbers
        hex_reverse_array[count++] = (char)(res + 48);
      }
      else if (res >= 10 && res < 16)
      {
        // Adding characters using ascii for Letters
        hex_reverse_array[count++] = (char)(res + 55);
      }
      // Rigt shifting by 4 to access next 4 bits of the number
      num = num >> 4;
    }
    // Range check
    if ((count * 4) > nbits)
    {
      goto error;
    }
    else if (count < nbits)
    {
      // calculating extra zeros for hex representation
      int extra_zeros = (nbits - (count * 4)) >> 2;
      // Adding Extra zeros
      for (int i = 0; i < extra_zeros; i++)
      {
        hex_reverse_array[count++] = '0';
      }
    }
  }
  else
  {
  // Error Block
  error:
    str[0] = '\0';
    return -1;
  }
  // Adding hex identifiers
  str[0] = '0';
  str[1] = 'x';
  int count2 = 2;
  // Reversing the reversed array and adding to the string
  for (int i = count - 1; i >= 0; i--)
  {
    if (i == 3)
      str[count2++] = '_';

    str[count2++] = hex_reverse_array[i];
  }
  str[count2++] = '\0';
  // Returning the total characters written without \0
  return count + 2;
}
// ------------------------------------------------End------------------------------------------------------------
