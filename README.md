

# PES Final Project & Report
Spring 2022, 
Written By **Chinmay Shalawadi**
University of Colorado Boulder, ECEE
chsh1552@colorado.edu
References -> Lecture Slides ,Dean & White Text Book, Disassembly from MCUXpresso
						I2C & Accelerometer References from Dean Repository
						https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo/src
						
## Demo Video Link

 - https://www.youtube.com/watch?v=B2YyIXl01aE
 
## Project Goals 
 - Play  musical notes via the DAC.  In response to the commands  on the
   user interface play   different  musical notes.  As mentioned  in the
   example project, a command like   “A300,G300,F300”  will play 300msec
   of note A and followed by the others.   
 
 - Use the Accelerometer Data to vary the music notes
## Sound Output Details 
![Alt text](../master/AudioOutSetup.png)
## Implementation Details 
Music Notes Supported. Frequency data used from https://pages.mtu.edu/~suits/notefreqs.html. 
  `A,B,C,D,E,F,G (The octaves have been chosen such that there is a linear increase in frequency)`

 - Command Processor from Assignment 6 (Breakfast Serial), has been modified to add 2 new commands. **'play'** & **'motionplay'**  . Below are the example usage of both commands and their description.
 

	    play A300,B500,C400,F900
	    
The above command plays the note A for 300msec, B for 500msec, C for 400msec and lastly F for 900msec.
Time in msec needs to be a minimum 10msec and there is no check for upper bound. 
The notes can be upper case or lower case.
Max Limit for number of characters for a command is 100.


 
 
## Results

 - ADC Caliberation has been implemented using references from the manual and github.

## Testing

 - ADC Caliberation has been implemented using references from the manual and github.
 
 ## Ideas for Improvements

 - ADC Caliberation has been implemented using references from the manual and github.



    
