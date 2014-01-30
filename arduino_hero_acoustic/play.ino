void chord(boolean mode) {
  if(!mode) Synth.arpeggioOn(4,7); // major chord
  else Synth.arpeggioOn(2,5); // minor chord
  Synth.note(frets(GREEN,RED,YELLOW,BLUE,ORANGE,octave),DEFAULT_DUTY_CYCLE,20);
  silent=false;
  return;
}

void play() {
  if(cLift) {
    Synth.arpeggioOff();
    cLift=false;
  }
  Synth.note(frets(GREEN,RED,YELLOW,BLUE,ORANGE,octave),DEFAULT_DUTY_CYCLE,20);
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

