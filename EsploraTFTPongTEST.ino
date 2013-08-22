/*

 Esplora TFT Pong
 
 This example for the Esplora with an Arduino TFT screen reads 
 the value of the joystick to move a rectangular platform
 on the x and y axes. The platform can intersect with a ball
 causing it to bounce. The Esplora's slider adjusts the speed
 of the ball.
 
 This example code is in the public domain.
 
 Created by Tom Igoe December 2012
 Modified 15 April 2013 by Scott Fitzgerald
 
 http://arduino.cc/en/Tutorial/EsploraTFTPong
 
 */

#include <Esplora.h>
#include <TFT.h>            // Arduino LCD library
#include <SPI.h>

#define sclk 15
#define mosi 16
#define cs   7
#define dc   0
#define rst  1

const int MENU_STATE = 0;
const int GAME_STATE = 1;
const int HELP_STATE = 2;
const int CREDITS_STATE = 3;
const int RED_BACKGROUND = 255;
const int GREEN_BACKGROUND = 255;
const int BLUE_BACKGROUND = 255;
const int MENU_OFFSET = 5;
char* MENU[] = {"PLAY", "HELP", "CREDITS"};

#define MENU_LENGTH (sizeof(MENU)/sizeof(char*))

const int BLINK_DELAY = 4000;
int currentBlinkCount = 0;
boolean blinkOver = true;

TFT screen = TFT(cs, dc, rst);
int width, height;

int state = 0;
int menuItemSelected = 1;

boolean redraw = true;

void setup() {

  Serial.begin(9600);

  // initialize the display
  screen.begin();
  width = screen.width();
  height = screen.height();
  // set the background the black
  screen.background(255,255,255);
}

void drawMenu(){
  int textHeight;
  
  int stroke;
  int posY = 24;
  int textSize;
  boolean background;
  int fill;  
  
  for(int i = 1; i < MENU_LENGTH +1; i++){
    if(i == menuItemSelected){
      stroke = 255;
      textSize = 3;
      fill = 30;
      background = true;
      
      textHeight = 30;
    } else {
      textSize = 2;
      stroke = 0;
      fill = 255;
      textHeight = 20;
      background = false;
    }

    drawText(MENU[i -1], 10, posY, stroke, stroke, stroke, textSize, background, fill, fill, fill, width, 29, 0, posY - 4, false);
    posY += (textHeight + MENU_OFFSET);
  }
}

void drawBackBtn(boolean over){
  int stroke;
  int fill;
  if(over){
    stroke = 255;
    fill = 30;
  } else {
    stroke = 0;
    fill = 0;
  }
  drawText("BACK", 130, 115, stroke, stroke, stroke, 1, true, fill, fill, fill, 31, 15, 126, 111, !over);
}

void drawGame(){
  initGame();
}

void drawHelp(){
  drawTitle("HELP", 50);
  drawBackBtn(blinkOver);
  
  drawText("Use the bottom slider", 5, 35, 45, 45, 45, 1, false, 0, 0, 0, 0, 0, 0, 0, false);
  drawText("to navigate in the menus.", 5, 47, 45, 45, 45, 1, false, 0, 0, 0, 0, 0, 0, 0, false);
  drawText("SWITCH2: Enter", 5, 59, 45, 45, 45, 1, false, 0, 0, 0, 0, 0, 0, 0, false);
  drawText("SWITCH3: Back", 5, 71, 45, 45, 45, 1, false, 0, 0, 0, 0, 0, 0, 0, false);
  drawText("Use the left joystick", 5, 83, 45, 45, 45, 1, false, 0, 0, 0, 0, 0, 0, 0, false);
  drawText("to controle the racket", 5, 95, 45, 45, 45, 1, false, 0, 0, 0, 0, 0, 0, 0, false);
  drawText("in game.", 5, 107, 45, 45, 45, 1, false, 0, 0, 0, 0, 0, 0, 0, false);
}

void drawCredits(){
  drawTitle("CREDITS", 18);
  drawBackBtn(blinkOver);
  
  drawText("Made by AbsolutRenal,", 18, 50, 45, 45, 205, 1, false, 0, 0, 0, 0, 0, 0, 0, false);
  drawText("using Arduino Esplora", 16, 68, 45, 45, 45, 1, false, 0, 0, 0, 0, 0, 0, 0, false);
  drawText("at MEDIAGONG.", 35, 86, 45, 45, 45, 1, false, 0, 0, 0, 0, 0, 0, 0, false);
}

void loop() {
  if(state == MENU_STATE){
    int tempSelected = map(Esplora.readSlider(), 0, 1023, MENU_LENGTH, 1);
    
    if(tempSelected != menuItemSelected){
      menuItemSelected = tempSelected;
      redraw = true;
    } else if(Esplora.readButton(SWITCH_LEFT) == LOW){
      state = menuItemSelected;
      redraw = true;
    }
  } else if(Esplora.readButton(SWITCH_UP) == LOW){
    redraw = true;
    state = MENU_STATE; 
  } else if((state == HELP_STATE || state == CREDITS_STATE)){
    currentBlinkCount ++;
    if(currentBlinkCount == BLINK_DELAY){
      blinkOver = !blinkOver;
      currentBlinkCount = 0;
      drawBackBtn(blinkOver);
    }
  }
  
  if(redraw){
    eraseScreen();

    
    if(state == MENU_STATE){
      drawMenu();
    } else if(state == GAME_STATE){
      drawGame();
    } else if(state == HELP_STATE){
      currentBlinkCount = 0;
      drawHelp();
    } else if(state == CREDITS_STATE){
      currentBlinkCount = 0;
      drawCredits();
    }
  } else if(state == GAME_STATE){
    update();
  }
}
