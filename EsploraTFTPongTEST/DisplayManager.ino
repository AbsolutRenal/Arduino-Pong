void eraseScreen(){
  redraw = false;
  screen.stroke(RED_BACKGROUND, GREEN_BACKGROUND, BLUE_BACKGROUND);
  screen.fill(RED_BACKGROUND, GREEN_BACKGROUND, BLUE_BACKGROUND);
  screen.rect(0, 0, width, height);
  screen.noStroke();
}

void eraseRegion(int x, int y, int w, int h){
  screen.fill(RED_BACKGROUND, GREEN_BACKGROUND, BLUE_BACKGROUND);
  screen.rect(x, y, w, h);
}
