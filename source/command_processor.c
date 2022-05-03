/***************************************************************************
 * Breakfast Serial
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: lecture slides & White Textbook & Assignment 1 (Hexdump)
 ***************************************************************************/

#include "command_processor.h"
#include "fsl_debug_console.h"
#include "tone_to_sample.h"
#include "dac.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "MKL25Z4.h"
#include "systick.h"
#include "mma8451.h"
#include <math.h>

// Print character definitions
#define CR 0xD
#define SPACE 0x20
#define BACKSPACE 0x8
#define COMMA 0x2C


// Frequency tones map
#define A (440)
#define B (496)
#define C (524)
#define D (590)
#define E (660)
#define F (700)
#define G (784)

#define MILLISECONDS_200 (20) // 10 count measures to 100 millisec, since we get timer interrupt at appx every 10ms

#define LINEBUFFER_LENGTH 100

// Function Declarations
static void handle_author(int no_commands, char *command_list[]);
static void handle_help(int no_commands, char *command_list[]);
static void handle_play(int no_commands, char *command_list[]);
static void handle_motionplay(int no_commands, char *command_list[]);
static void process_command(char *input);
static void line_accumulator(char line[]);

// function pointer definitions
typedef void (*command_handler_t)(int, char *command_list[]);

// command struct definition
typedef struct
{
  const char *name;
  command_handler_t handler;
  const char *help_string;
} command_table_t;

// commands available
// New commands can be added very easily by adding an entry to the table
// and adding a command handler function
static const command_table_t commands[] = {
    {"author,", handle_author, "Prints Author's Name"},
    {"help,", handle_help, "Lists all the commands supported"},
    {"play", handle_play, "plays tones in the order they're listed, Ex 'play A400,C500'"},
    {"motionplay,", handle_motionplay, "Changes the tone automatically based on pitch and roll data from accelerometer"}

};

static const int num_commands =
    sizeof(commands) / sizeof(command_table_t);

uint8_t volatile continue_playing_flag = 0;

// ------------------------------------------------start-command-processor----------------------------------------------------------
/***********************************************************************************
 * function : Starts the command processor
 * parameters : none
 * return : none
 ***********************************************************************************/
void start_command_processor()
{
  // Line buffer to hold line contents
  char line[LINEBUFFER_LENGTH] = {""};

  // Command processor loop
  while (1)
  {
    printf("\n\r? ");
    line_accumulator(line);
    process_command(line);
  }
};
// ------------------------------------------------line-accumulator-----------------------------------------------------------
/***********************************************************************************
 * function : Gets one line of data from the terminal and handles white space and backspace
 * parameters : *line -> destination location to store final line data
 * return : none
 ***********************************************************************************/
static void line_accumulator(char *line)
{
  // index variable used to write to the buffer
  int i = 0;

  char *data = line;
  // input data byte definiton
  char byte = 0;
  // counter to handle multiple white spaces
  int space_count = 0;

// Instead of a while loop, a label and goto statements are used for input
getmorechars:
  byte = getchar();
  // if valid byte is available
  if (byte > 0)
  {
    // Check to see if the byte is not equal to Carriage Return
    if (byte != CR)
    {
      // Update space counter
      if (byte == SPACE)
        space_count++;

      // If backspace is hit and i>0
      if (byte == BACKSPACE && i)
      {
        i -= 1;
      }
      // Adding character to line if its not backspace and leading spaces
      if (space_count <= 1 && byte != BACKSPACE && i != 0)
      {
        data[i++] = byte;
        if (byte != SPACE)
          space_count = 0;
      }
      // Adding characters to the line if its not space or backspace
      else if (byte != SPACE && byte != BACKSPACE)
      {
        data[i++] = byte;
        space_count = 0;
      }
      goto getmorechars;
    }
    else
    {
      // Adding an extra space and null terminator
      // Extra space is added for the command parser
      data[i++] = COMMA;
      data[i++] = SPACE;
      data[i++] = '\0';
    }
  }
  else
  {
    // when no valid data byte is available go back to get more characters
    goto getmorechars;
  }
}
// ------------------------------------------------process-command-----------------------------------------------------------
/***********************************************************************************
 * function : Parses the line into commands and calls the relevant command handler
 * parameters : *input -> source line data accumulated by line accumulator
 * return : none
 ***********************************************************************************/
static void process_command(char *input)
{
  char *p = input;
  char *end;
  char *temp;
  // find end of string
  for (end = input; *end != '\0'; end++)
    ;
  // Tokenize input in place
  char *command_list[10];
  int no_commands = 0;

  // initializing all the values to 0
  memset(command_list, 0, sizeof(command_list));

  // temp used to store locations of every word start
  temp = input;
  for (p = input; p < end; p++)
  {
    // Replacing the space character with \0 and storing the pointer to the begining
    // in the command_list array
    if (*p == SPACE)
    {
      *p = 0;
      command_list[no_commands++] = temp;
      temp = (p + 1);
    }
    // If there are too many argument entered by the user
    if (no_commands > 3)
    {
      printf("\n\rError: Too many commands/arguments \n\r");
      return;
    }
  }

  command_list[no_commands] = NULL;
  // If no command is sent
  if (no_commands == 0)
  {
    printf("\n\rError: No Command Sent \n\r");
    return;
  }

  // Flag to check if a valid command is written
  int unknown_command = 1;

  // Find the matching command and dispatch the command handler
  for (int i = 0; i < num_commands; i++)
  {
    if (strcasecmp(command_list[0], commands[i].name) == 0)
    {
      commands[i].handler(no_commands, command_list);
      // updating the unknown command flag
      unknown_command = 0;
      break;
    }
  }

  if (unknown_command)
    printf("\n\n\rUnknown Command: %s\n\n\rUse 'help' for list of commands\n\r", command_list[0]);
}
// ------------------------------------------------handle-author-----------------------------------------------------------
/***********************************************************************************
 * function : prints the author
 * parameters : no_commands -> count variable of command array
 *              command_list -> pointers to list of commands
 * return : none
 ***********************************************************************************/
static void handle_author(int no_commands, char *command_list[])
{
  printf("\n\rChinmay Shalawadi\n\r");
};
// ------------------------------------------------handle-help-----------------------------------------------------------
/***********************************************************************************
 * function : Prints all the commands with their help text
 * parameters : no_commands -> count variable of command array
 *              command_list -> pointers to list of commands
 * return : none
 ***********************************************************************************/
static void handle_help(int no_commands, char *command_list[])
{
  printf("\n\n\r  Commands Supported\n\n\r");
  for (int i = 0; i < num_commands; i++)
  {
    printf("  '%s' -> ", commands[i].name);
    printf("%s\n\n\r", commands[i].help_string);
  }
}
// ------------------------------------------------handle-play-----------------------------------------------------------
/***********************************************************************************
 * function : Plays all the tones in the order user specifies and time period
 *            specified by the user
 * parameters : no_commands -> count variable of command array
 *              command_list -> pointers to list of commands
 * return : none
 ***********************************************************************************/
static void handle_play(int no_commands, char *command_list[])
{
  uint16_t total_samples;
  uint16_t dac_buff[BUFF_SIZE];

  char *p = command_list[1];
  char *end;
  char *temp;

  // find end of string
  for (end = command_list[1]; *end != '\0'; end++)
    ;
  // Tokenize input in place
  char *music_notes[10];
  int no_notes = 0;

  // initializing all the values to 0
  memset(music_notes, 0, sizeof(music_notes));

  // temp used to store locations of every music note start
  temp = command_list[1];
  for (p = command_list[1]; p < end; p++)
  {
    // Replacing the comma in notes list with \0 and storing the pointer to the begining
    // in the command_list array
    if (*p == COMMA)
    {
      *p = 0;
      music_notes[no_notes++] = temp;
      temp = (p + 1);
    }
  }

  
  continue_playing_flag = 1;
  for (int i = 0; i < no_notes; i++)
  {
    char *note = music_notes[i];
    int delay;

    if (*note == 'A')
      total_samples = tone_to_samples(A, dac_buff, BUFF_SIZE);
    else if (*note == 'B')
      total_samples = tone_to_samples(B, dac_buff, BUFF_SIZE);
    else if (*note == 'C')
      total_samples = tone_to_samples(C, dac_buff, BUFF_SIZE);
    else if (*note == 'D')
      total_samples = tone_to_samples(D, dac_buff, BUFF_SIZE);
    else if (*note == 'E')
      total_samples = tone_to_samples(E, dac_buff, BUFF_SIZE);
    else if (*note == 'F')
      total_samples = tone_to_samples(F, dac_buff, BUFF_SIZE);
    else if (*note == 'G')
      total_samples = tone_to_samples(G, dac_buff, BUFF_SIZE);

    sscanf(++note, "%d", &delay);

    delay /= 10;
    // Get DMA and DAC working
    buffer_data_copy(dac_buff, total_samples);
    start_tone();
    reset_timer();
    while (get_timer() < delay)
      ;
  }
  continue_playing_flag = 0;
  reset_dma();
}
// ------------------------------------------------handle-motionplay-----------------------------------------------------------
/***********************************************************************************
 * function : uses the accelerometer data to change the tones
 * parameters : no_commands -> count variable of command array
 *              command_list -> pointers to list of commands
 * return : none
 ***********************************************************************************/
static void handle_motionplay(int no_commands, char *command_list[])
{
  uint8_t roll_f, pitch_f;
  uint16_t all_notes[7] = {A, B, C, D, E, F, G};
  uint8_t current_note = 0;
  uint16_t total_samples;
  uint16_t dac_buff[BUFF_SIZE];

  total_samples = tone_to_samples(all_notes[current_note], dac_buff, BUFF_SIZE);

  continue_playing_flag = 1;

  buffer_data_copy(dac_buff, total_samples);
  start_tone();

  while (1)
  {
    reset_timer();
    while (get_timer() < MILLISECONDS_200)
      ;

    read_full_xyz();
    convert_xyz_to_roll_pitch();
    roll_f = (fabs(roll) > 10) ? 1 : 0;
    pitch_f = (fabs(pitch) > 10) ? 1 : 0;

    if (roll_f && !pitch_f)
    {
      if (current_note < 6)
      {
        current_note++;
        total_samples = tone_to_samples(all_notes[current_note], dac_buff, BUFF_SIZE);
        buffer_data_copy(dac_buff, total_samples);
        start_tone();
      }
    }

    if (pitch_f && !roll_f)
    {
      if (current_note > 0)
      {
        current_note--;
        total_samples = tone_to_samples(all_notes[current_note], dac_buff, BUFF_SIZE);
        buffer_data_copy(dac_buff, total_samples);
        start_tone();
      }
    }
  }
}
// ------------------------------------------------End------------------------------------------------------------
