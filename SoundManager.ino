void rebound(){
  playNote(880, 30, 0);
}

void winPoint(){
  playNote(510, 100, 100);
  playNote(660, 100, 100);
  playNote(770, 100, 100);
}

void loosePoint(){
  playNote(255, 500, 0);
}

void playNote(int pitch, int duration, int delayed){
  Esplora.tone(pitch, duration);
  delay(delayed);
}
