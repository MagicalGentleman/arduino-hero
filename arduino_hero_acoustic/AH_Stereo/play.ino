void chord(boolean mode) {
  if(!mode) {
    Channel[0].arpeggioOn(4,7); // major chord
    if(chug)Channel[1].arpeggioOn(4,7); // major chord
  }
  else {
    Channel[0].arpeggioOn(2,5); // minor chord
    if(chug)Channel[1].arpeggioOn(2,5); // minor chord
  }
  play();
  return;
}

void play() {
  if(cLift) {
    Channel[0].arpeggioOff();
    Channel[1].arpeggioOff();
    cLift=false;
  }
  int pitch=frets(GREEN,RED,YELLOW,BLUE,ORANGE,octave);
  int swapSpace;
  if(slide&&hammerOn){
    Channel[0].transform(pitch,2);
    Channel[1].transform(pitch,3);
    swapSpace=pitch;
    pitch=pitchMem;
    pitchMem=swapSpace;
  }
  else pitchMem=pitch;
  /*if(noisy){
    Channel[LRAlternate].noise(pitch);
    Channel[!LRAlternate].noise(pitch);
  }*/
  //else {
    Channel[0].note(pitch,L_DUTY_CYCLE,30,2);
    Channel[1].note(pitch,R_DUTY_CYCLE,30,2);
  //}
  hammerOn=false;
  silent=false;
  //LRAlternate=!LRAlternate;
  return;
}

void killNote() { // cuts off all notes and sets 'silent' to 'true'
  Channel[0].noteOff();
  Channel[1].noteOff();
  silent=true;
  return;
}

byte pitch(byte pit, byte oct){
  return (pit+(oct*12));
}

