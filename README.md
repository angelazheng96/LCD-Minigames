# LCD-Minigames

This repository contains all the code for my TEJ2O (Computer Technology) Summative Project, completed in June 2022.

There are multiple minigames displayed on the LCD using an Arduino Uno Rev3.  

## Links

[Daily Log](daily-log.md)  
[Functional Block Diagram](functional-block-diagram.png)  
[Schematic](hardware-schematic.png)  

## Introduction

Who doesn’t like some simple video games to play? With these LCD Minigames, you can choose between a wide variety of games to play!

I came up with this idea pretty quickly, as I knew that the LCD would allow me to come up with a few different minigames. I also thought that it would be a fun, interactive setting for the player, while incorporating a few other hardware aspects. I decided to make a project from scratch rather than modify an existing one, since I didn’t think I’d be able to make that many contributions to an existing project.

Here are the minigames you can choose from!

### 1. Rock Paper Scissors
Take your pick between rock, paper, or scissors as you compete against the computer with this famous game!

### 2. Match the Pairs
Test your memory by matching up all the cards with their pair. Try to do it in the least amount of turns possible!

### 3. Guess the Number
Ever wondered how good you are at guessing the correct number? Choose from 1 to 9 as you try to be the closest to the correct answer.

### 4. Electric Fan
While this isn’t necessarily a minigame, it’s still a great way to cool off on a hot (and humid) summer day!

## User's Manual

### Welcome Page
Press POWER on the IR Remote to move on to the Mode Selection Menu.

### Mode Selection Menu
Press 1, 2, 3, or 4 on the IR Remote to choose a minigame.

### Mode Confirm
After choosing a minigame on the Mode Selection Menu, this will confirm your choice by displaying the name of the minigame. Press POWER on the IR Remote to proceed to the chosen minigame or 0 to go back to the Mode Selection Menu.

### Mode 1 - Rock Paper Scissors

#### Objective
In this classic game usually played by hand, try and beat the opponent (randomized by the computer). You will both choose between rock, paper, or scissors. Paper beats rock (paper wraps around rock), rock beats scissors (rock smashes scissors), and scissors beat paper (scissors cut paper).

#### Scoring
Winning gives you 1 point.
Losing gives the opponent 1 point.
A tie (choosing the same thing) gives both 0 points.

#### Instructions
Take your pick between rock, paper, or scissors. Use the joystick to move left or right to make your selection; the cursor will appear immediately to the right of your choice. Press the joystick button to confirm your choice.

Your choice and the opponent’s choice will show up on the screen. After a few seconds, the result (whether you have won, lost, or tied) will show up, followed by your score and the opponent’s score. Press the joystick button to confirm and move on.

On the Restart Page, press POWER on the IR Remote to restart the minigame and play again (your score will be saved), or 0 to go back to the Mode Selection Menu to choose a different minigame (your score will not be saved).

### Mode 2 - Match the Pairs

#### Objective
With six pairs of cards shuffled randomly on screen, flip them over until you have matched all of them to their pair.

#### Scoring
Flip over cards to match them with their pair. Try to reduce the number of turns it takes to match all the cards!

#### Instructions
All twelve cards will appear face up for a few seconds. Look them over and try to memorize what they are.

All cards will turn face down. Use the joystick to move left, right, up, or down to choose a card to flip over. The cursor will appear to the right of the card you are currently selecting. Press the joystick button to flip over the card. Repeat again to flip over a second card. The number of turns will go up by one.

If the two flipped-over cards are a match, they will disappear from the screen. If they are different, they will remain for a few seconds before flipping back over.

Continue flipping over pairs of cards until you have matched all six pairs.

On the Restart Page, press POWER on the IR Remote to restart the minigame and play again (your score will be saved), or 0 to go back to the Mode Selection Menu to choose a different minigame.

### Mode 3 - Guess the Number

#### Objective
Try to guess the correct number from 0 to 9. Compete against the opponent to see if you can be the closest to the correct answer!

#### Scoring
If you guess closer to the correct number than the opponent, you receive 1 point.
If the opponent guesses closer to the correct number than you, the opponent receives 1 point.
If you and the opponent’s guesses are the same distance from the correct number, you both receive 0 points.

#### Instructions
Press a button from 0 to 9 on the IR Remote. Your guess will appear on the screen for a few seconds, before revealing the opponent’s guess.

After a few seconds, the correct number will appear. Then, the scores will appear. Press the joystick button to move on.

On the Restart Page, press POWER on the IR Remote to restart the minigame and play again (your score will be saved), or 0 to go back to the Mode Selection Menu to choose a different minigame (your score will not be saved).

### Mode 4 - Electric Fan

#### Objective
Use this customizable remote-controlled electric fan to cool off in hot weather.

#### Instructions
First, choose a speed: either SLOW or FAST. Press 1 on the IR Remote to choose SLOW or 2 to choose FAST.

Then, choose a duration: either 10 seconds or 15 seconds. Press 1 on the IR Remote to choose 10 seconds or 2 to choose 15 seconds.

The fan will automatically turn on for the chosen duration at the specified speed. Make sure to pick it up from the table and point it towards yourself!

On the Restart Page, press POWER on the IR Remote to use the fan again or 0 to go back to the Mode Selection Menu to choose a different minigame.
