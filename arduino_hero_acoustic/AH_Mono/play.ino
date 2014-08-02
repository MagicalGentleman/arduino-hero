void chord(boolean mode) {
  if(!mode) Synth.arpeggioOn(4,0,7); // major chord
  else Synth.arpeggioOn(2,0,5); // minor chord
  play();
  return;
}

void play() {
  if(cLift) {
    Synth.arpeggioOff();
    cLift=false;
  }
  int cycle=duty[dutyBool];
  int pitch=frets(GREEN,RED,YELLOW,BLUE,ORANGE,octave);
  int swapSpace;
  if(slide&&hammerOn){
    if(noteGap>150) noteGap=150;
    Synth.transform(pitch,noteGap/50);
    swapSpace=pitch;
    pitch=pitchMem;
    pitchMem=swapSpace;
  }
  else pitchMem=pitch;
  Synth.note(pitch,cycle,(cycle/2),2);
  silent=false;
  hammerOn=false;
  noteGap=50;
  return;
}

void killNote() { // cuts off all notes and sets 'silent' to 'true'
  Synth.softKill(2);
  silent=true;
  return;
}

byte pitch(byte pit, byte oct){
  return (pit+(oct*12));
}

