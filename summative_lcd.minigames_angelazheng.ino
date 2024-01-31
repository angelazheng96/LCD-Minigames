/*
  TEJ2O Summative Project - LCD Minigames & Electric Fan

  Choose between Rock Paper Scissors, Match the Pairs, Guess the Number, or turn on a remote-controlled fan!

  Author: Angela Zheng
  Date created: June 6, 2022
  Last modified: June 23, 2022
*/

// libraries
#include <IRremote.hpp>
#include <LiquidCrystal.h>

// other file in the folder - contains custom character designs
#include "summative_custom_characters.h"

// general variables
int stage = 0;
int gameMode = 0;

// Arduino pins
const int RS = 7, EN = 6, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
const int IR_RECEIVE_PIN = 8;
const int JOYSTICK_BUTTON_PIN = 9;
const int FAN_ENABLE = 10;
const int FAN_DIRA = 11;
const int FAN_DIRB = 12;
const int JOYSTICK_X_PIN = A0;
const int JOYSTICK_Y_PIN = A1;

// IR receiver
IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;
int remoteButtonPressed = 0;

// LCD
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// joystick
int joyX = 512;
int joyY = 512;
int previousJoyX = 512;
int previousJoyY = 512;
int joystickPosition = 0;             // value from 0 to 8 - represents the more general position of the joystick (up, left, right, etc.)
int previousJoystickPosition = 0;
boolean joystickButton = 1;
boolean previousJoystickButton = 1;

// Rock Paper Scissors
int rpsStage = 0;     // current stage of the RPS game
int rpsChoose = 0;    // when choosing what to put out, this stores what is currently selected
int rpsChooseLcd[3] = {4, 8, 12};   // stores the values for where the blinking cursors are shown on LCD
int rpsYouPick = 0;   // after choosing what to put out, the value gets stored here
int rpsOppPick = 0;
int rpsYouScore = 0;  // your score
int rpsOppScore = 0;  // opponent's score

// Match the Pairs
int mtpStage = 0;

int mtpRandomNum = 0;
int mtpCardsNum[6] = {0, 0, 0, 0, 0, 0};            // the number of times that each card has been put in the shuffled array
                                                    // (each index is a different card value, e.g. index 0 is customSmile)

int mtpCardsShuffled[2][6] = {{0, 0, 0, 0, 0, 0},   // the cards' values after being shuffled
                              {0, 0, 0, 0, 0, 0}};

int mtpCursorCoordinates[6] = {1, 3, 5, 7, 9, 11};  // coordinates (column #) for the cursor on the LCD
int mtpCardCoordinates[6] = {0, 2, 4, 6, 8, 10};    // coordinates (column #) for the cards on the LCD

int mtpCursor[2] = {0, 0};                          // where the cursor is currently (row #, column #)

boolean mtpCardsState[2][6] = {{0, 0, 0, 0, 0, 0},  // whether each card is currently unmatched or matched
                               {0, 0, 0, 0, 0, 0}};

int mtpFlippedCounter = 0;                          // stores how many cards are currently flipped over
int mtpFlippedCoordinates[2][2] = {{6, 6},          // stores the coordinates for cards that are flipped over
                                   {6, 6}};         // not initialized at 0 because (0, 0) is the top-left card

int mtpTurns = 0;             // the number of turns the player has used to match the cards
int mtpMatchedCounter = 0;    // the number of matches that have already been made

// Guess the Number
int gtnStage = 0;
int gtnCorrectGuess = 0;
int gtnRemoteValues[10] = {22, 12, 24, 94, 8, 28, 90, 66, 82, 74};
int gtnYouGuess = 10;     // set to 10 since 0 is a valid guess
int gtnOppGuess = 10;
int gtnYouDist = 0;       // difference between the player's guess and the correct number (absolute value)
int gtnOppDist = 0;
int gtnYouScore = 0;
int gtnOppScore = 0;

// Electric Fan
int efStage = 0;
int efSpeed = 0;
int efDuration = 0;

void setup() {

  Serial.begin(9600);

  pinMode(IR_RECEIVE_PIN, INPUT);
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(FAN_ENABLE, OUTPUT);
  pinMode(FAN_DIRA, OUTPUT);
  pinMode(FAN_DIRB, OUTPUT);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  lcd.begin(16, 2);

  randomSeed(analogRead(A5));

}

void loop() {

  irReceive();

  // updates the previous joystick position, reads joystick values, updates new joystick position
  joystickRead();
  joystickPositionRead();

  lcd.noBlink();

  // figures out which stage the program is in & runs the right function based on that
  switch(stage) {

    case 0:
      startPage();
      break;

    case 1:
      modeSelection();
      break;

    case 2:
      modeConfirm();
      break;

    case 3:
      rockPaperScissors();
      break;

    case 4:
      matchThePairs();
      break;

    case 5:
      guessTheNumber();
      break;

    case 6:
      electricFan();
      break;
    
  }

  delay(50);

}

// if a button is pressed on the remote, its code gets put into the remoteButtonPressed variable
void irReceive() {

  remoteButtonPressed = 0;

  if (IrReceiver.decode()) {
    remoteButtonPressed = IrReceiver.decodedIRData.command;
    delay(100);
    IrReceiver.resume();
  }

}

// reads joystick values & updates the variables for the previous values
void joystickRead() {

  previousJoyX = joyX;
  previousJoyY = joyY;

  joyX = analogRead(JOYSTICK_X_PIN);
  joyY = analogRead(JOYSTICK_Y_PIN);

  previousJoystickButton = joystickButton;
  joystickButton = digitalRead(JOYSTICK_BUTTON_PIN);

}

// returns where the joystick is (up, left, bottom, right, etc.)
// see planning document for meaning of variable values
void joystickPositionRead() {

  previousJoystickPosition = joystickPosition;

  // if x is to the left, it can be top left, left, or bottom left
  if (joyX < 256) {

    if (joyY < 256) {
      joystickPosition = 8;
    } else if (joyY > 766) {
      joystickPosition = 6;
    } else {
      joystickPosition = 7;
    }

  // if x is to the right, it can be top right, right, or bottom right
  } else if (joyX > 766) {

    if (joyY < 256) {
      joystickPosition = 2;
    } else if (joyY > 766) {
      joystickPosition = 4;
    } else {
        joystickPosition = 3;
    }

  // if x is in the middle, then it can be top, middle, or bottom
  } else {

    if (joyY < 256) {
      joystickPosition = 1;
    } else if (joyY > 766) {
      joystickPosition = 5;
    } else {
      joystickPosition = 0;
    }
    
  }
  
}

// startPage at the very beginning of the code
void startPage() {

  lcd.setCursor(0, 0);
  lcd.print("Welcome! Press");
  lcd.setCursor(0, 1);
  lcd.print("POWER to play");

  // if the POWER button has been pressed, it proceeds to the next screen
  if (remoteButtonPressed == 69) {
    stage = 1;
    lcd.clear();
    return;
  }

}

// screen to choose a game mode
void modeSelection() {

  lcd.setCursor(0, 0);
  lcd.print("Choose a mode:");
  lcd.setCursor(0, 1);
  lcd.print("1 2 3 4");

  // different buttons pressed go to different minigames
  switch(remoteButtonPressed) {

    case 12:
      lcd.clear();
      stage = 2;
      gameMode = 1;
      break;

    case 24:
      lcd.clear();
      stage = 2;
      gameMode = 2;
      break;

    case 94:
      lcd.clear();
      stage = 2;
      gameMode = 3;
      break;

    case 8:
      lcd.clear();
      stage = 2;
      gameMode = 4;
      break;
    
  }

}

// shows the name of each game mode after choosing it, confirming the player's choice
void modeConfirm() {

  switch(gameMode) {

    // Mode 1 - Rock Paper Scissors
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("MODE 1 - Rock");
      lcd.setCursor(0, 1);
      lcd.print("Paper Scissors");

      modeConfirmButtons();
      
      break;

    // Mode 2 - Match the Pairs
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("MODE 2 - Match");
      lcd.setCursor(0, 1);
      lcd.print("the Pairs");

      modeConfirmButtons();

      break;

    // Mode 3 - Guess the Number
    case 3:
      lcd.setCursor(0, 0);
      lcd.print("MODE 3 - Guess");
      lcd.setCursor(0, 1);
      lcd.print("the Number");

      modeConfirmButtons();

      break;

    case 4:
      lcd.setCursor(0, 0);
      lcd.print("MODE 4 - ");
      lcd.setCursor(0, 1);
      lcd.print("Electric Fan");

      modeConfirmButtons();

      break;
    
  }
  
}

// while on the mode confirm page, this function runs to check the buttons pressed
void modeConfirmButtons() {

  // if the POWER button is pressed, it confirms and starts the game
  if (remoteButtonPressed == 69) {
    lcd.clear();
    // sets the stage for the game
    stage = gameMode + 2;

  // if 0 is pressed, it returns to the mode selection menu
  } else if (remoteButtonPressed == 22) {
    lcd.clear();
    gameMode = 0;
    stage = 1;
  }

}

// the game of Rock Paper Scissors (against a randomizer)
void rockPaperScissors() {

  lcd.createChar(0, customRock);
  lcd.createChar(1, customPaper);
  lcd.createChar(2, customScissors);
  lcd.createChar(3, customCursor);
  
  switch(rpsStage) {

    // player chooses rock, paper, or scissors to put out
    case 0:

      lcd.clear();
    
      // prints the non-moving parts on the screen
      lcd.setCursor(0, 0);
      lcd.print("Take your pick!");
      lcd.setCursor(3, 1);
      lcd.write(byte(0));
      lcd.setCursor(7, 1);
      lcd.write(byte(1));
      lcd.setCursor(11, 1);
      lcd.write(byte(2));

      // prints cursor beside the currently selected choice
      lcd.setCursor(rpsChooseLcd[rpsChoose], 1);
      lcd.write(byte(3));        

      // if the joystick has been moved left or right and then back to the middle
      if (previousJoystickPosition != 0 && joystickPosition == 0) {
        
        // joystick moved to right, currently selected gets incremented (unless it's already at the right)
        if (previousJoystickPosition == 3 && rpsChoose != 2) {
          rpsChoose++;
          
        // same thing happens when joystick is moved to the left
        } else if (previousJoystickPosition == 7 && rpsChoose != 0) {
          rpsChoose--;
        }
        
      }

      // if the button has been pressed, it locks in the selection and moves on to the next stage
      if (previousJoystickButton == 0 && joystickButton == 1) {
        
        rpsYouPick = rpsChoose;   // your pick gets locked in
        rpsOppPick = int(random(0, 3));
        
        rpsChoose = 0;
        rpsStage = 1;
        lcd.clear();
        
      }
              
      break;

    // shows both players' choices on the screen, who won, and the scores
    case 1:

      // prints text
      lcd.setCursor(1, 0);
      lcd.print("YOU        OPP");

      // prints both players' picks
      lcd.setCursor(2, 1);
      lcd.write(byte(rpsYouPick));
      lcd.setCursor(13, 1);
      lcd.write(byte(rpsOppPick));

      // delays a bit before showing the results
      delay(1000);

      // determining who has won (0 = rock, 1 = paper, 2 = scissors)

      // first determining if it's a tie
      if (rpsYouPick == rpsOppPick) {
        lcd.setCursor(6, 0);
        lcd.print("TIE!");
        
      // if YOU is greater than OPP, then YOU has won, unless YOU is 2 and OPP is 0
      } else if (rpsYouPick > rpsOppPick) {

        if (rpsYouPick == 2 && rpsOppPick == 0) {
          lcd.setCursor(6, 0);
          lcd.print("LOSE");
          rpsOppScore++;
          
        } else {
          lcd.setCursor(6, 0);
          lcd.print("WIN!");
          rpsYouScore++;
        }
      
      // if YOU is less than OPP, then YOU has lost, unless YOU is 0 and OPP is 2
      } else {

        if (rpsYouPick == 0 && rpsOppPick == 2) {
          lcd.setCursor(6, 0);
          lcd.print("WIN!");
          rpsYouScore++;
          
        } else {
          lcd.setCursor(6, 0);
          lcd.print("LOSE");
          rpsOppScore++;
        }
        
      }

      // sets unneeded variables back to 0 for next time it runs
      rpsYouPick = 0;

      delay(1000);

      lcd.setCursor(6, 1);
      lcd.print(String(rpsYouScore) + "  " + String(rpsOppScore));

      // moves to next stage without clearing
      rpsStage = 2;
      
      break;

    // detects if the button has been pressed to move on to the restart page
    case 2:

      if (previousJoystickButton == 0 && joystickButton == 1) {
        lcd.clear();
        rpsStage = 3;
      }

      break;

    // restart screen
    case 3:
    
      lcd.setCursor(0, 0);
      lcd.print("Restart - POWER");
      lcd.setCursor(0, 1);
      lcd.print("Home - 0");

      // if POWER is pressed, restarts the game
      if (remoteButtonPressed == 69) {
        lcd.clear();
        gameMode = 1;
        stage = 3;
        rpsStage = 0;

      // if 0 is pressed, returns to mode selection screen
      } else if (remoteButtonPressed == 22) {
        lcd.clear();
        gameMode = 0;
        stage = 1;
        rpsStage = 0;
        rpsYouScore = 0;
        rpsOppScore = 0;
      }

      break;
    
  }
  
}

void matchThePairs() {

  lcd.createChar(0, customSmile);
  lcd.createChar(1, customHeart);
  lcd.createChar(2, customCircle);
  lcd.createChar(3, customSquare);
  lcd.createChar(4, customHouse);
  lcd.createChar(5, customPerson);
  lcd.createChar(6, customCardBack);
  lcd.createChar(7, customCursor);

  switch(mtpStage) {

    // shuffles cards & stores them in int arrays
    case 0:

      // runs through all the 12 cards that are on the screen
      for (int i = 0; i < 2; i++) {
        
        for (int j = 0; j < 6; j++) {

          // for each one, it continually generates a random card between 0 and 5
          // each number represents a different kind of card (e.g. smiley face or square)
          while (true) {
            
            mtpRandomNum = random(0, 6);

            // if that card has not already been used twice,
            // then this card space becomes that card & breaks out of the forever loop
            if (mtpCardsNum[mtpRandomNum] != 2) {
              mtpCardsShuffled[i][j] = mtpRandomNum;
              mtpCardsNum[mtpRandomNum]++;
              break;
            }
            
          }
          
        }
        
      }

      // moves on to the next stage
      mtpStage = 1;
      break;

    // right before the main game - all cards are flipped over for a few seconds so that the player can memorize them
    case 1:

      lcd.clear();

      // goes through all 12 cards and prints the front of them
      for (int i = 0; i < 2; i++) {
        
        for (int j = 0; j < 6; j++) {
          
          lcd.setCursor(mtpCardCoordinates[j], i);
          lcd.write(byte(mtpCardsShuffled[i][j]));
          
        }
        
      }

      // prints the turns on the side of the screen
      lcd.setCursor(13, 0);
      lcd.print("Trn");
      lcd.setCursor(14, 1);
      lcd.print(mtpTurns);

      delay(4000);

      // moves on to the main stage
      mtpStage = 2;
      break;

    // main game stage - cards are printed on screen & player can flip over cards
    case 2:

      lcd.clear();

      // if there are less than two cards flipped, then the cursor can move around and cards can be flipped
      if (mtpFlippedCounter < 2) {

        // runs through all 12 cards and prints the back of them
        for (int i = 0; i < 2; i++) {
          
          for (int j = 0; j < 6; j++) {

            // sets the cursor to the right spot
            lcd.setCursor(mtpCardCoordinates[j], i);

            // if the card is the first card flipped, print the front
            if (i == mtpFlippedCoordinates[0][0] && j == mtpFlippedCoordinates[0][1]) {
              
              lcd.write(byte(mtpCardsShuffled[i][j]));

            // if the card is the second card flipped, print the front
            } else if (i == mtpFlippedCoordinates[1][0] && j == mtpFlippedCoordinates[1][1]) {
              
              lcd.write(byte(mtpCardsShuffled[i][j]));

            // if the card is not flipped but not yet matched, print the back
            } else if (mtpCardsState[i][j] == 0) {

              lcd.write(byte(6));
              
            }

            // doesn't print anything for cards that are already matched
            
          }
          
        }

        // prints the cursor in the right spot
        lcd.setCursor(mtpCursorCoordinates[mtpCursor[1]], mtpCursor[0]);
        lcd.write(byte(7));

        // prints the number of turns taken
        lcd.setCursor(13, 0);
        lcd.print("Trn");
        lcd.setCursor(14, 1);
        lcd.print(mtpTurns);

        // after reprinting the cards, but before allowing the user to move
        // checks if all the cards have already been matched
        if (mtpMatchedCounter == 6) {

          delay(2000);

          // resets unneeded variables
          
          mtpMatchedCounter = 0;
          mtpTurns = 0;

          for (int i = 0; i < 2; i++) {
            mtpCursor[i] = 0;
          }

          for (int i = 0; i < 6; i++) {
            mtpCardsNum[i] = 0;
          }

          for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
              mtpFlippedCoordinates[i][j] = 6;
            }
          }

          for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 6; j++) {
              mtpCardsState[i][j] = 0;
            }
          }
          
          // moves on to the next stage & clears the screen
          mtpStage = 3;
          lcd.clear();
          
          break;
          
        }
  
        // joystick handling changing the cursor position
        
        // if the joystick was moved to the right
        if (previousJoystickPosition != 0 && joystickPosition == 0) {
  
          // if the joystick was moved to the top & it's not already at the top, the cursor moves up
          if (previousJoystickPosition == 1 && mtpCursor[0] != 0) {
            mtpCursor[0]--;
  
          // joystick moved to the right
          } else if (previousJoystickPosition == 3 && mtpCursor[1] != 5) {
            mtpCursor[1]++;
            
          // joystick moved to the bottom
          } else if (previousJoystickPosition == 5 && mtpCursor[0] != 1) {
            mtpCursor[0]++;
            
          // joystick moved to the left
          } else if (previousJoystickPosition == 7 && mtpCursor[1] != 0) {
            mtpCursor[1]--;
          }
       
        }
  
        // if the joystick button has been pressed & the card is still on the screen
        if (previousJoystickButton == 0 && joystickButton == 1 && mtpCardsState[mtpCursor[0]][mtpCursor[1]] == 0) {
  
          // if the card is not already flipped
          if (mtpCursor[0] != mtpFlippedCoordinates[0][0] || mtpCursor[1] != mtpFlippedCoordinates[0][1]) {

            // this card's coordinates get entered into the array
            mtpFlippedCoordinates[mtpFlippedCounter][0] = mtpCursor[0];
            mtpFlippedCoordinates[mtpFlippedCounter][1] = mtpCursor[1];

            // the counter for the number of flipped cards goes up by 1
            mtpFlippedCounter++;
            
          }
          
        }

      // if two cards have already been flipped
      } else {

        mtpTurns++;

        // runs through all 12 cards & prints them again
        for (int i = 0; i < 2; i++) {
          
          for (int j = 0; j < 6; j++) {

            // sets the cursor to the right spot to print the card
            lcd.setCursor(mtpCardCoordinates[j], i);

            // if the card is the first card flipped, print the front
            if (i == mtpFlippedCoordinates[0][0] && j == mtpFlippedCoordinates[0][1]) {
              
              lcd.write(byte(mtpCardsShuffled[i][j]));

            // if the card is the second card flipped, print the front
            } else if (i == mtpFlippedCoordinates[1][0] && j == mtpFlippedCoordinates[1][1]) {
              
              lcd.write(byte(mtpCardsShuffled[i][j]));

            // if the card is not flipped and not yet matched, print the back
            } else if (mtpCardsState[i][j] == 0) {

              lcd.write(byte(6));
              
            }

            // doesn't print anything for cards that are already matched

          }
          
        }

        // prints the cursor in the right spot (where it was before)
        lcd.setCursor(mtpCursorCoordinates[mtpCursor[1]], mtpCursor[0]);
        lcd.write(byte(7));

        // prints the current number of turns taken
        lcd.setCursor(13, 0);
        lcd.print("Trn");
        lcd.setCursor(14, 1);
        lcd.print(mtpTurns);

        delay(2000);

        // if the two flipped cards are the same (have the same icon on the front)
        if (mtpCardsShuffled[mtpFlippedCoordinates[0][0]][mtpFlippedCoordinates[0][1]] == mtpCardsShuffled[mtpFlippedCoordinates[1][0]][mtpFlippedCoordinates[1][1]]) {

          // runs through the two cards that are currently unflipped
          for (int i = 0; i < 2; i++) {

            // both cards are set as "removed" in the card state array
            mtpCardsState[mtpFlippedCoordinates[i][0]][mtpFlippedCoordinates[i][1]] = 1;

            // runs through the two coordinates for each of the two cards
            for (int j = 0; j < 2; j++) {

              // the flipped cards array gets set back to its original value
              mtpFlippedCoordinates[i][j] = 6;
              
            }
            
          }

          // the counter gets set back to 0
          mtpFlippedCounter = 0;

          // adds 1 to the counter for cards that have been matched
          mtpMatchedCounter++;
          
        } else {

          // all the flipped coordinates get set back to the original values
          for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
              mtpFlippedCoordinates[i][j] = 6;
            }
          }

          mtpFlippedCounter = 0;
        }
        
      }

      break;

    // restart page
    case 3:

      lcd.setCursor(0, 0);
      lcd.print("Restart - POWER");
      lcd.setCursor(0, 1);
      lcd.print("Home - 0");

      // if POWER is pressed, restarts the game
      if (remoteButtonPressed == 69) {
        lcd.clear();
        gameMode = 2;
        stage = 4;
        mtpStage = 0;

      // if 0 is pressed, returns to mode selection screen
      } else if (remoteButtonPressed == 22) {
        lcd.clear();
        gameMode = 0;
        stage = 1;
        mtpStage = 0;
      }

      break;
    
  }

}

void guessTheNumber() {

  lcd.createChar(0, customCardBack);

  switch(gtnStage) {

    // correct number is randomized & the player picks their number
    case 0:

      // the correct number is randomized
      gtnCorrectGuess = random(0, 10);

      // the "opponent" (also the computer)'s guess is randomized
      gtnOppGuess = random(0, 10);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("YOU");
      lcd.setCursor(9, 0);
      lcd.print("Guess a");
      lcd.setCursor(9, 1);
      lcd.print("number!");

      // if the player hasn't guessed yet, reads input from the IR receiver
      // runs through all the codes sent for 0 to 9 on the IR remote
      // the player's pick gets set to the array index, since the code for the button 0 is at index 0 in the array (same for the other numbers)
      if (gtnYouGuess == 10 && remoteButtonPressed != 0) {
        
        for (int i = 0; i < 10; i++) {
          
          if (gtnRemoteValues[i] == remoteButtonPressed) {
            gtnYouGuess = i;
            break;
          }
          
        }
        
      }

      // if the player has already guessed, then it gets printed on the screen and then moves on to the next stage
      if (gtnYouGuess != 10) {
        
        lcd.setCursor(1, 1);
        lcd.print(gtnYouGuess);
        
        delay(2000);
        gtnStage = 1;
        
      }

      break;

    // the guesses are revealed, but the correct number is still unknown
    case 1:

      lcd.clear();

      // prints text & guesses
      lcd.setCursor(0, 0);
      lcd.print("YOU CORRECT  OPP");
      lcd.setCursor(1, 1);
      lcd.print(gtnYouGuess);
      lcd.setCursor(14, 1);
      lcd.print(gtnOppGuess);
      lcd.setCursor(7, 1);
      lcd.write(byte(0));

      // after waiting a bit, moves on to the next stage
      delay(2000);
      gtnStage = 2;

      break;

    // correct guess is revealed
    case 2:

      lcd.clear();

      // prints text & guesses
      lcd.setCursor(0, 0);
      lcd.print("YOU CORRECT  OPP");
      lcd.setCursor(1, 1);
      lcd.print(gtnYouGuess);
      lcd.setCursor(14, 1);
      lcd.print(gtnOppGuess);

      // correct guess is printed in the middle
      lcd.setCursor(7, 1);
      lcd.print(gtnCorrectGuess);

      // waits a bit, then moves on to the next stage
      delay(2000);
      gtnStage = 3;
      
      break;

    // the winner is revealed
    case 3:

      lcd.clear();

      // prints the same text from before
      lcd.setCursor(0, 0);
      lcd.print("YOU          OPP");
      lcd.setCursor(1, 1);
      lcd.print(gtnYouGuess);
      lcd.setCursor(14, 1);
      lcd.print(gtnOppGuess);

      // determines who won (whoever's guess is closer to the correct one)

      gtnYouDist = abs(gtnYouGuess - gtnCorrectGuess);
      gtnOppDist = abs(gtnOppGuess - gtnCorrectGuess);

      // if the guesses' distance from the correct one are the same
      if (gtnYouDist == gtnOppDist) {
        
        lcd.setCursor(6, 0);
        lcd.print("TIE!");
        lcd.setCursor(6, 1);
        lcd.print(String(gtnYouScore) + "  " + String(gtnOppScore));

      // if the player's guess is closer
      } else if (gtnYouDist < gtnOppDist) {

        gtnYouScore++;

        lcd.setCursor(6, 0);
        lcd.print("WIN!");
        lcd.setCursor(6, 1);
        lcd.print(String(gtnYouScore) + "  " + String(gtnOppScore));

      // if the opponent's guess is closer
      } else {

        gtnOppScore++;

        lcd.setCursor(6, 0);
        lcd.print("LOSE");
        lcd.setCursor(6, 1);
        lcd.print(String(gtnYouScore) + "  " + String(gtnOppScore));
        
      }

      gtnStage = 4;

      break;

    // continues displaying the scores, detects if the joystick button has been pressed
    case 4:

      // moves on to the next stage & resets variables when the joystick button is pressed
      if (previousJoystickButton == 0 && joystickButton == 1) {
        
        gtnStage = 5;
        
        gtnYouGuess = 10;
        gtnOppGuess = 10;
        
      }

      break;

    // restart page
    case 5:

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Restart - POWER");
      lcd.setCursor(0, 1);
      lcd.print("Home - 0");

      // if POWER is pressed, restarts the game
      if (remoteButtonPressed == 69) {
        lcd.clear();
        gameMode = 3;
        stage = 5;
        gtnStage = 0;

      // if 0 is pressed, returns to mode selection screen
      } else if (remoteButtonPressed == 22) {
        lcd.clear();
        gameMode = 0;
        stage = 1;
        gtnStage = 0;
        gtnYouScore = 0;
        gtnOppScore = 0;
      }

      break;
    
  }

}

void electricFan() {

  switch(efStage) {

    // choose the fan speed
    case 0:

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Choose a speed:");
      lcd.setCursor(0, 1);
      lcd.print("1: SLOW  2: FAST");

      // if 1 on the remote is pressed, the speed is set to SLOW
      if (remoteButtonPressed == 12) {
        efSpeed = 150;
        efStage = 1;

      // if 2 on the remote is pressed, the speed is set to FAST
      } else if (remoteButtonPressed == 24) {
        efSpeed = 255;
        efStage = 1;
      }

      break;

    // choose the duration of the fan turning on
    case 1:

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Choose a length:");
      lcd.setCursor(0, 1);
      lcd.print("1: 10s    2: 15s");

      // if 1 on the remote is pressed, the duration is set to 10 seconds
      if (remoteButtonPressed == 12) {
        efDuration = 10000;
        efStage = 2;

      // if 2 on the remote is pressed, the duration is set to 15 seconds
      } else if (remoteButtonPressed == 24) {
        efDuration = 15000;
        efStage = 2;
      }

      break;

    // the fan turns on for the chosen duration
    case 2:

      lcd.clear();

      // turns fan on at the chosen speed
      analogWrite(FAN_ENABLE, efSpeed);

      // starts running
      digitalWrite(FAN_DIRA, HIGH);
      digitalWrite(FAN_DIRB, LOW);

      // delays for the chosen duration
      delay(efDuration);

      // turns off
      digitalWrite(FAN_ENABLE, LOW);
      digitalWrite(FAN_DIRA, LOW);
      digitalWrite(FAN_DIRB, LOW);

      delay(1000);
      efStage = 3;

      break;

    case 3:

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Restart - POWER");
      lcd.setCursor(0, 1);
      lcd.print("Home - 0");

      // if POWER is pressed, restarts the game
      if (remoteButtonPressed == 69) {
        lcd.clear();
        gameMode = 4;
        stage = 6;
        efStage = 0;

      // if 0 is pressed, returns to mode selection screen
      } else if (remoteButtonPressed == 22) {
        lcd.clear();
        gameMode = 0;
        stage = 1;
        efStage = 0;
      }

      break;
    
  }
  
}
