

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

The DAC output has been fed into an Audio Interface/Recorder (Zoom H1). This recorder connects to the desktop via USB and live audio can be monitored from the DAC. The audio is played back on the speakers of the display. 
Alternatively, the audio can also be fed into any audio amplified and speaker setup for sound output. 

## Implementation Details 
Music Notes Supported. Frequency data used from https://pages.mtu.edu/~suits/notefreqs.html.
 
	   A,B,C,D,E,F,G (The octaves have been chosen such that there is a linear increase in frequency)

 - Command Processor from Assignment 6 (Breakfast Serial), has been modified to add 2 new commands. **'play'** & **'motionplay'**  . Below are the example usage of both commands and their description.
 

	    play A300,B500,C400,F900
	    
The above command plays the note A for 300msec, B for 500msec, C for 400msec and lastly F for 900msec.
A list of notes are made by splitting them using the comma. 
Time in msec needs to be a minimum 10msec and there is no check for upper bound. 
The notes can be upper case or lower case.
Max Limit for number of characters for a command is 100.

	  motionplay
This is the second command which has been implemented, this does not need any parameters. This commands initiates an infinite loop where the Accelerometer data is periodically sampled and depending on whether **pitch or roll** is detected,  the tone played is changed.
After entering the infinte loop, the default tone is set to A, depending on whether a pitch is detected or a roll is detected next or the previous tone is played.
In motionplay mode, the supported notes are the same as play mode.
There is no way to exit this mode with user input. The microcontroller has to be reset to enter the command processor again.
There is diagnostic output about which tone is being played on the console. 

With a combination of pitch and rolls, random melodies can be played. As shown in the video.

Initially when the DAC signal was played as a sound, there were many hitches between the same frequency output. 
Code from Assignment 7 had to be tweaked to make the audio output seamless and smooth. This involved changing the behaviour of DMA end of transfer interrupt and how frequently the samples are generated.

Accelerometer was configured to detect pitch and roll using example code from Dean text book and the github repository.

**UART Connection parameters**

 - Baud 38400
 - Stop Bits - 2
 - No Parity

**Peripherals & Technologies Used**

 - Systick
 - TPM0
 - Accelerometer
 - DAC
 - UART
 - I2C
 - Command Processor
 - DMA
 - Interrupts(NVIC)
 - Integer Math

 
 
## Results

 Both the project goals set during the project proposal have been met. The demo video is available on YouTube.
![Alt text](../master/play_screenshot.png)

## Testing

 - Command processor testing was done manually and supports backspace and handles invalid commands. 
 - Assignment 7 test_sin function provided by Howdy was used to test the accuracy of the frequencies generated by the samples generator
 - Testing of final objectives was done manually since an audio signal is played. The audio signal was also tested by a phone app to measure the frequency.
 
 ## Ideas for Improvements

 - Error handling for multiple music notes can be expanded to check  for duplicate notes.
 - Accelerometer can be used to detect different actions and produce accurate tone changes based on actions.
 - User Input can be taken to define new Music Notes and integrate it into available notes.


    
