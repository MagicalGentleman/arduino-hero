void chord(boolean mode) {
  if(!mode) Synth.arpeggioOn(4,7); // major chord
  else Synth.arpeggioOn(2,5); // minor chord
  Synth.noteOn(frets(GREEN,RED,YELLOW,BLUE,ORANGE,octave));
  trigger=mils;
  duty=15;
  silent=false;
  return;
}

void play() {
  if(cLift) {
    Synth.arpeggioOff();
    cLift=false;
  }
  Synth.noteOn(frets(GREEN,RED,YELLOW,BLUE,ORANGE,octave));
  trigger=mils;
  duty=15;
  silent=false;
  return;
}

void killNote() { // cuts off all notes and sets 'silent' to 'true'
  Synth.noteOff();
  silent=true;
  return;
}

void addDepth() { // alters the duty cycle over time
  if((trigger-triggerReference)>0){
    Synth.dutyCycle(duty);
    if(duty<35) duty++;
    else triggerReference=mils;
  }
}

byte pitch(byte pit, byte oct){
  return (pit+(oct*12));
}

