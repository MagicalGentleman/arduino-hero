void chord(boolean mode) {
  if(!mode){      // Play major chord on mode 0 (strum down)
    play(0,0,velocity);
    play(4,1,velocity);
    play(7,2,velocity);
  }
  else{           // Play minor chord
    play(0,0,velocity);
    play(2,1,velocity);
    play(5,2,velocity);
  }
  /*
  
  */
  return;
}

void play(byte offset, int count, int vel) {
  byte j;
  if(!silent){
    if((drop&&c)||cLift) {
      for(int i=0; i<=3; i++) MIDI.sendNoteOn(noteMem[i],0,MIDIOUT);
      cLift=false;
      drop=false;
    }
    else MIDI.sendNoteOn(noteMem[count],0,MIDIOUT);
  }
  j=(offset+(frets(g,r,y,b,o,octave)));
  MIDI.sendNoteOn(j,vel,MIDIOUT);
  noteMem[count]=j;
  if(!vel) silent=true;
  else silent=false;
  return;
}

void killNote() { // cuts off all notes and sets 'silent' to 'true'
  if(c||cLift) {
    for(int i=0; i<=3; i++) MIDI.sendNoteOff(noteMem[i],64,MIDIOUT);
    cLift=false;
  }
  else MIDI.sendNoteOff(noteMem[0],64,MIDIOUT);
  silent=true;
  velocity=0;
  return;
}
