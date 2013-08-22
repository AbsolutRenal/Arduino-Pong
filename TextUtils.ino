/**
Text Size :
2 => 14px
3 => 21px
**/

void drawTitle(char title[], int posX){
  drawText(title, posX, 4, 255, 255, 255, 3, true, 30, 30, 30, width, 29, 0, 0, false);
}

void drawTimer(char time[], int posX){
  drawText(time, posX, 54, 255, 255, 255, 3, true, 180, 180, 180, width, 29, 0, 50, false);
}

void drawResult(char result[], int posX){
  drawText(result, posX, 54, 255, 255, 255, 2, true, 180, 180, 180, width, 22, 0, 50, false);
}

void drawText(char text[], int x, int y, int strokeR, int strokeG, int strokeB, int textSize, boolean background, int fillR, int fillG, int fillB, int backWidth, int backHeight, int backX, int backY, boolean backgroundStroke){
  
  
  if(background){
    if(backgroundStroke){
      screen.stroke(fillR, fillG, fillB);
      screen.fill(255 - fillR, 255 - fillG, 255 - fillB);
    } else {
      screen.fill(fillR, fillG, fillB);
    }
    screen.rect(backX, backY, backWidth, backHeight);
  }
    
  screen.stroke(strokeR, strokeG, strokeB);
  screen.setTextSize(textSize);
  screen.text(text, x, y);
  screen.noStroke();
}

void drawScore(char score1[], char score2[]){
  drawText(score1, 61, 4, 145, 145, 145, 2, false, 30, 30, 30, width, 29, 0, 0, false);
  drawText(score2, 90, 4, 145, 145, 145, 2, false, 30, 30, 30, width, 29, 0, 0, false);
}
