const int RACKET_HEIGHT = 20;
const int RACKET_WIDTH = 4;
const int RACKET_OFFSET = 6;
const int RACKET_R = 0;
const int RACKET_G = 0;
const int RACKET_B = 0;
const int BALL_R = 0;
const int BALL_G = 0;
const int BALL_B = 0;
const int BALL_RADIUS = 2;
const int UPDATE_DELAY = 50;
const int REDRAW_OFFSET = 0;
const int MAX_SCORE = 10;

int tick = 0;
int racket1Y, prevRacket1Y;
float racket2Y, prevRacket2Y;
float ballX, ballY;
float prevBallX, prevBallY;
float speedX, speedY;
boolean gameStarted = false;
int middle;
int movingHeight;
boolean redrawRacket = false;
int score = 0;
int adversaryScore = 0;
boolean gameEnded = false;
boolean scoreRedraw = false;

float iaDestY;
int iaDirection;
boolean calculateDestIA = true;
const float IA_INCREMENT = 2.00;
float iaIncrement;
int center;
int level = 1;


void initGame(){
  center = prevRacket1Y = prevRacket2Y = racket1Y = racket2Y = (height - RACKET_HEIGHT) * .5;
  middle = width * .5;
  movingHeight = height - RACKET_HEIGHT;

  drawRacket(RACKET_OFFSET, racket1Y);
  drawRacket(width - RACKET_OFFSET - RACKET_WIDTH, racket2Y);

  drawMiddleLine();

  ballX = width * .5;
  ballY = height * .5;
  drawBall();

  startTimer();
}

void drawMiddleLine(){
  screen.stroke(220, 220, 220);

  int posY = 0;
  int dash = 3;

  while(posY < height){
    screen.line(middle, posY, middle, posY + dash);
    posY += (dash * 2);
  }

  screen.noStroke();
}

void startTimer(){
  drawLevel(level);
  
  iaIncrement = IA_INCREMENT + ((float)level) * .1;
  Serial.println(iaIncrement);
  
  drawTimer("3", 73);
  delay(1000);
  drawTimer("2", 73);
  delay(1000);
  drawTimer("1", 73);
  delay(1000);
  eraseRegion(0, 39, width, 40);

  startGame();
}

void startGame(){
  score = 0;
  adversaryScore = 0;
  gameEnded = false;
  
  drawCurrentScore(true);
  newRound(false);
}

void drawCurrentScore(boolean erase){
  if(erase)
    eraseRegion(60, 4, 50, 15);
  
  drawMiddleLine();

  char scoreStr[3];
  char adversaryScoreStr[3];
  sprintf(scoreStr, "%d", score);
  sprintf(adversaryScoreStr, "%d", adversaryScore);
  drawScore(scoreStr, adversaryScoreStr);
  
  if(score == MAX_SCORE || adversaryScore == MAX_SCORE){
    endGame();
  } 
}

void endGame(){
  gameStarted = false;
  
  int posX;
  char* result;
  
  if(score == MAX_SCORE){
    result = "YOU WIN !";
    posX = 27;
    level ++;
  } else {
    result = "YOU LOOSE";
    posX = 25;
  }
  
  drawResult(result, posX);
  gameEnded = true;
}

void updateScore(boolean loose){
  if(loose)
    adversaryScore ++;
  else
    score ++;
  
  drawCurrentScore(true);
}

void newRound(boolean erase){
  tick = 0;
  gameStarted = true;
  calculateDestIA = true;
  iaDestY = middle;

  int sign = random(-100, 100);
  speedX = random(10, 20) * getSign(sign) * .1;
  sign = random(-100, 100);
  speedY = random(10, 20) * getSign(sign) * .1;

  /*Serial.print(speedX);
  Serial.print("\t");
  Serial.println(speedY);*/

  drawRacket(RACKET_OFFSET, racket1Y);
  drawRacket(width - RACKET_OFFSET - RACKET_WIDTH, racket2Y);
  
  if(erase){
    eraseRegion(0, 0, RACKET_OFFSET, height);
    eraseRegion(width - RACKET_OFFSET, 0, RACKET_OFFSET, height);
  }

  prevBallX = ballX = width * .5;
  prevBallY = ballY = height * .5;
  drawBall();

  drawMiddleLine();
}

void drawBall(){
  screen.fill(BALL_R, BALL_G, BALL_B);
  screen.circle(ballX, ballY, BALL_RADIUS);
}

void updateBall(){
  eraseRegion(prevBallX - BALL_RADIUS - 3 , prevBallY - BALL_RADIUS - 3, BALL_RADIUS * 2 + 6, BALL_RADIUS * 2 + 6);

  prevBallX = ballX;
  prevBallY = ballY;

  ballX += speedX;
  ballY += speedY;

  if((ballY - BALL_RADIUS) <= 0 || (ballY + BALL_RADIUS) >= height){
    speedY *= -1;
  }
  
  if(((ballX - BALL_RADIUS) <= (RACKET_OFFSET + RACKET_WIDTH) && (ballX - BALL_RADIUS) >= (RACKET_OFFSET + RACKET_WIDTH - 2) && ballY >= racket1Y && ballY <= (racket1Y + RACKET_HEIGHT))
      || ((ballX + BALL_RADIUS) >= (width - RACKET_OFFSET - RACKET_WIDTH) && (ballX + BALL_RADIUS) <= (width - RACKET_OFFSET - RACKET_WIDTH + 2) && ballY >= racket2Y && ballY <= (racket2Y + RACKET_HEIGHT))
      
      || ((prevBallX - BALL_RADIUS) > (RACKET_OFFSET + RACKET_WIDTH) && (ballX - BALL_RADIUS) < (RACKET_OFFSET + RACKET_WIDTH) && ballY >= racket1Y && ballY <= (racket1Y + RACKET_HEIGHT))
      || ((prevBallX + BALL_RADIUS) < (width - RACKET_OFFSET - RACKET_WIDTH) && (ballX + BALL_RADIUS) > (width - RACKET_OFFSET - RACKET_WIDTH) && ballY >= racket2Y && ballY <= (racket2Y + RACKET_HEIGHT))
      ){
    redrawRacket = true;
    speedX *= -1.1;
    speedY *= 1.1;
    
    rebound();
  }

  ballY = constrain(BALL_RADIUS, ballY, height - BALL_RADIUS);
  drawMiddleLine();
  drawBall();
  drawCurrentScore(false);

  if(ballX < 0 || ballX > width){
    addPoint(ballX < 0);
  }
}

void addPoint(boolean loose){  
  updateScore(loose);
  
  if(loose)
    loosePoint();
  else
    winPoint();
  
  if(gameStarted)
    newRound(true);
}

void updateRackets(){
  float percentPos = map(Esplora.readJoystickY(), -512, 512, 0, 100) * .01;
  racket1Y = percentPos * movingHeight;

  //if(racket1Y != prevRacket1Y || redrawRacket){
  if(tick ==0 ){
    redrawRacket = false;
    eraseRegion(RACKET_OFFSET, prevRacket1Y, RACKET_WIDTH, RACKET_HEIGHT);
    prevRacket1Y = racket1Y;
    drawRacket(RACKET_OFFSET, racket1Y);
  }
  
  // CHEAT
  if(ballX < middle){
    if(iaDestY != center){
      calculateDestIA = true;
      iaDestY = center;
      //iaDirection = getSign(iaDestY - racket2Y);
    }
  } else if(speedX > 0 && calculateDestIA == true){
    calculateDestIA = false;
    iaDestY = calculateReboundY();
    /*iaDirection = getSign(iaDestY - racket2Y);
    Serial.print("iaDirection: ");
    Serial.print(iaDirection);
    Serial.print("\t dest: ");
    Serial.println(iaDestY);*/
  }
  iaDirection = getSign(iaDestY - racket2Y);
  
  if(abs(racket2Y - iaDestY) > iaIncrement && tick == 0){
    racket2Y += (iaIncrement * iaDirection);
  }
    
  //racket2Y = ballY - RACKET_HEIGHT * .5;
  racket2Y = constrain(racket2Y, 0, movingHeight);
  // CHEAT

  //if(racket2Y != prevRacket2Y || redrawRacket){
  if(tick ==0 ){
    redrawRacket = false;
    eraseRegion(width - RACKET_OFFSET - RACKET_WIDTH, prevRacket2Y, RACKET_WIDTH, RACKET_HEIGHT);
    prevRacket2Y = racket2Y;
    drawRacket(width - RACKET_OFFSET - RACKET_WIDTH, racket2Y);
  }
}

float calculateReboundY(){
  float dest = ballY + ((width - RACKET_OFFSET - RACKET_WIDTH - ballX - BALL_RADIUS) / speedX) * speedY;
  if(dest < 0)
    dest = abs(dest);
  if(dest > height)
    dest = height + (height - dest);
  
  int errorSign = getSign(random(-10, 10));
  float error = (float)RACKET_HEIGHT * (1.00 - ((float)level) * .1) * errorSign * (float)random(0, 6) * .1;
  dest += error;
  Serial.println(error);
  
  return dest - RACKET_HEIGHT * .5;
}

void drawRacket(int x, float y){
  screen.fill(RACKET_R, RACKET_G, RACKET_B);
  screen.rect(x, y, RACKET_WIDTH, RACKET_HEIGHT);
}

void update(){
  if(gameStarted){
    tick ++;
    if(tick == UPDATE_DELAY){
      //drawCurrentScore();
      tick = 0;
      updateBall();
      
      if(refreshScore())
        drawCurrentScore(false);
    }
    updateRackets();
  } else if(Esplora.readButton(SWITCH_LEFT) == LOW){
    eraseScreen();
    startTimer();
  }
}

boolean refreshScore(){
  boolean ret = false;
  
  if(ballY < (14 + REDRAW_OFFSET) && ballX > (61 - REDRAW_OFFSET) && ballX < (100 + REDRAW_OFFSET)){
    scoreRedraw = true;
  } else if(scoreRedraw){
    scoreRedraw = false;
    ret = true;
  }
  
  return ret;
}

