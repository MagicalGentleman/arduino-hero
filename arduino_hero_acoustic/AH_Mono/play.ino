void chord(boolean mode) {
  if(!mode) Synth.arpeggioOn(4,7); // major chord
  else Synth.arpeggioOn(2,5); // minor chord
  play();
  return;
}

void play() {
  if(cLift) {
    Synth.arpeggioOff();
    cLift=false;
  }
  int pitch=frets(GREEN,RED,YELLOW,BLUE,ORANGE,octave);
  int swapSpace;
  if(slide&&hammerOn){
    Synth.transform(pitch,2);
    swapSpace=pitch;
    pitch=pitchMem;
    pitchMem=swapSpace;
  }
  else pitchMem=pitch;
  if(noisy)Synth.noise(pitch);
  else Synth.note(pitch,DEFAULT_DUTY_CYCLE,20,2);
  silent=false;
  return;
}

void killNote() { // cuts off all notes and sets 'silent' to 'true'
  Synth.noteOff();
  silent=true;
  return;
}

byte pitch(byte pit, byte oct){
  return (pit+(oct*12));
}

