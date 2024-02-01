# Daily Log

## Monday, June 6, 2022
• Learned about how to use the [IR receiver](https://create.arduino.cc/projecthub/electropeak/use-an-ir-remote-transmitter-and-receiver-with-arduino-1e6bc8)  
• Implemented IR receiver code (not yet tested with actual hardware)  
    • Modified code from website slightly - based on comments, one line had to be changed to work with new versions of the IR library  
• Worked on hardware schematic on TinkerCad - LCD display, potentiometer for screen contrast, IR receiver, joystick  

## Tuesday, June 7, 2022
• Started building the actual circuit  
• Fixed IR receiver code - works properly  
    •  Recorded HEX codes for each button on the remote  
• Added code for LCD  
• Researched methods  
    • Naming conventions & where to define them in the sketch  
    • Didn't have time to add them to the code - intending to add a startPage() method for when the code originally begins  

## Wednesday, June 8, 2022
• Added other methods - startPage(), modeSelection()  
    • startPage was working until the LCD started malfunctioning (probably a hardware problem with the wires, not the software)  
• Problem with startPage() - updates text on LCD each time loop() runs, but parts of the LCD get dark then bright again  
    • Tried to figure out how to print the text only once on the screen, then continue detecting a button being pushed on the remote  
    • Tried using an extra "counter" variable - only prints the text when counter = 0, then counter++ each time - but it didn't work properly  
• Problems with communications  
    • The board wasn't communicating with the computer properly (error messages when uploading code, weird messages in the Serial monitor) - possible problem with USB Type A to USB Type B cable  

## Thursday, June 9, 2022
• Fixed LCD problem - didn't break after each case in switch()  
• Finalized startPage() screen  
• Began work on the modeSelection() screen  
    •	Goes to different game modes when you press 1, 2, or 3 on the remote  
•	Connected the joystick to the circuit  
    •	Created some variables for it (joyX, joyY)  
    •	Started coding a function to read joystick values and determine whether it’s been moved left, right, up, or down  
    •	Researched switch/case with multiple variables (don’t really exist) - would’ve been useful for the joystick readings  

## Friday, June 10, 2022
• Mostly worked on the joystick  
    •	Added joystickPositionRead() - rather than have to check for its actual x and y value, this function automatically tells whether it’s up, left, right, etc.  
    •	Struggled at first with a previous joystick position variable - for games that use the joystick, I want it to have a current joystick position and a previous joystick position  
        •	Before, the function joystickPositionRead() would simply return the current position  
        •	However, I changed it to putting the value into a joystickPosition variable so that there can also be a previousJoystickPosition variable  
•	Slightly worked on the Rock Paper Scissors game  
    •	Added the function for it, as well as the rpsStage variable that would tell which part of the game is currently happening  
    •	I came up with ideas on how it would look when playing the game  

## Sunday, June 12, 2022
•	In some places (currently working on the Rock Paper Scissors game), I need one specific “character” on the LCD to get deleted (and get replaced with another), but if I use lcd.clear() before reprinting everything, it becomes very dark  
    •	This is because I haven’t put any delay in the loop(), since I’m accepting inputs and I don’t want there to be a huge delay after pressing a button - but then the stuff on the LCD becomes darker since it’s constantly being cleared (not enough time to light up properly)  
    •	I ended up just putting a delay of 100 ms - it’s not a huge amount of time to be noticeable - although the screen still does flicker a bit (which is annoying, but it was still there even at a delay of 250 ms, and I didn’t want to go any higher)  
•	Finished the Rock Paper Scissors game  
    •	Fully tested & works properly  
    •	It wasn’t as difficult as I thought it might be to code, just time-consuming  
•	Created a new file - custom_characters.h  
    •	Contains all the custom characters that are created for the LCD  
    •	I made this because I need a lot of custom characters for the second minigame - Match the Pairs - and it quickly became annoying to scroll through a lot of byte arrays in the main file  
•	Started Match the Pairs (memory-based card-flipping matching game)  
    •	Mainly just designed all the custom characters (might change a few in the future)  

## Monday, June 13, 2022
•	Started work on the second minigame - Match the Pairs (a bunch of face-down cards that each have a match, and the player has to use their memory to flip over the matching pairs)  
•	Began with defining a bunch of variables, finished designing custom characters for the cards  
•	Wrote the code for the part that randomizes the cards (not yet tested)  

## Tuesday, June 14, 2022
• Worked more on Match the Pairs  
    •	The code that shuffles the cards does work  
    •	Prints all cards on screen (either flipped or unflipped)  
    •	The joystick can move the cursor around the screen to choose different cards  
    •	Pressing the joystick button flips the card back and forth  
• What I need to do:  
    • Player is only allowed to flip over two cards before it checks if they match  
    • Organize variable declarations before setup()  

## Wednesday, June 15, 2022
•	I sort of had to rewrite the code for flipping over cards when the joystick button is pressed - only two cards can be flipped before it goes to a different “state”  
    •	I wrote the code for this new “state” where the player can’t change anything - it just checks if the two flipped cards are the same or not  
        •	If they are, the score should go up by 1 and they disappear from the screen  
        •	If not, they should unflip and continue  
• Problems  
    • For some reason, flipping one card and then trying to flip another leads to not being able to actually flip the second card  
        •	It’s not a hardware issue (I checked to make sure the button worked properly)  
        •	I’m not sure which part in the software is causing the problem yet  
        •	Because of this, I haven’t been able to test what happens when two of the same cards are flipped over (I don’t know if they disappear from the screen like they should)  
    •	Another problem (that I can only fix once I fix the first problem)  
        •	Once cards disappear, the cursor shouldn’t be able to stop at their coordinates  
        •	I need to rewrite the code for the cursor moving so that it doesn’t stop at an empty spot  
        •	Right now, it moves around, just as long as it’s not already at the side (e.g. if it’s already at the very left, it cannot move to the left more)  

## Thursday, June 16, 2022
•	Fixed the first problem from yesterday  
    •	When the code checks if the second card being flipped is the same as the first card, I had accidentally put && instead of ||  
        •	I.e. it should just be that if either the row # or column # is different, then it’s a different card, but instead, it was checking that both had to be different  
        •	That was why some cards could be flipped as the second one, but not others - it had to be both in a different row and column than the first card  
    •	I was finally able to test the whole game - the cards do disappear like they should when they’re matched  
•	Adding more features  
    •	On the right side of the screen, it prints the number of turns currently taken to match all the cards  
    •	Wrote code for the restart screen  

## Friday, June 17, 2022
•	Fixing the problem from Wednesday - after cards are matched and disappeared, the cursor shouldn’t appear beside them  
