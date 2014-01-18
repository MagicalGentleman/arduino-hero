#include <MIDI.h>

#define MIDIOUT 1
#define INIT_OCTAVE 4
// where the octave MUST be between 0 and 10:
#define MAX_OCTAVE 8
#define MIN_OCTAVE 3
// dmg gameboy has normal range of 3 to 8

// pins
const int whammy=A0;
const int green=2;
const int red=3;
const int yellow=4;
const int blue=5;
const int orange=6;
const int start=7;
const int select=8;
const int strumD=9;
const int strumU=10;

int velocity=127;
byte noteMem[3]={0,0,0};

//GREEN,RED,YELLOW,BLUE,ORANGE,STRUM,(UP/SELECT),(DOWN/START),CHORD//
boolean g,r,y,b,o,s,u,d,c;
boolean g1,r1,y1,b1,o1,s1,u1,d1;
boolean g2,r2,y2,b2,o2,s2,u2,d2;

boolean sD; // Strum direction. up=true, down=false.
boolean cLift=false;
boolean drop=false;
boolean silent=true;

boolean skip=false;

byte octave=4;
int tick=1;
long int timer;

void setup() {
  MIDI.begin();
  Serial.begin(115200); // for usb serial to midi on computer. (use program 'hairless')
  pinMode(green, INPUT);
  pinMode(red, INPUT);
  pinMode(yellow, INPUT);
  pinMode(blue, INPUT);
  pinMode(orange, INPUT);
  pinMode(strumD, INPUT);
  pinMode(strumU, INPUT);
  pinMode(start, INPUT);
  pinMode(select, INPUT);
  g=r=y=b=o=false;
  g1=r1=y1=b1=o1=false;
  g2=r2=y2=b2=o2=false;
  s1=u1=d1=false;
  s2=u2=d2=false;
  timer=millis();
}

void loop() {
  unsigned long mils=millis();
  int w=0;
  int wham=analogRead(whammy);
  if((mils-timer)>=1) {
    tick*=(-1);
    if(tick>0) checkInput1(); // input trackers
    else checkInput2();
    checkKeyLocks(); // Most of the work is done here.
    timer=mils;
  }
  if((!silent)&&wham&&(wham<=420)){
    w=map(analogRead(whammy), 0, 420, -4096, 0);
    MIDI.sendPitchBend(w,MIDIOUT);
  }
}

void checkKeyLocks() {
  if((u2!=u1)&&(!u)){       //the instant select is lifted
    if(d){                  //if start is held down
      c=(!c);               // toggle chord mode
      if(!c) cLift=true;
      skip=true;
    }
    else if(skip) skip=false;
    else if(octave<(MAX_OCTAVE-1)) octave++;
  }
  else if((d2!=d1)&&(!d)){  //the instant start is lifted
    if(u){                  //if select is held down
      c=(!c);               // toggle chord mode
      if(!c) cLift=true;
      skip=true;
    }
    else if(skip) skip=false;
    else if(octave>MIN_OCTAVE) octave--;
  }
  else if((s1!=s2)&&s){ // check for strum
    velocity=127;
    if(!c) play(0,0,velocity); //new note
    else if(c) chord(sD);
  }
  else if((g2!=g1)||(r2!=r1)||(y2!=y1)||(b2!=b1)||(o2!=o1)) if(!silent){ // have the frets changed?
    if(!s) killNote(); //if frets change and strum isn't depressed, kill the note.
    else if(!((!g)&&(!r)&&(!y)&&(!b)&&(!o))){
      velocity-=1;
      if(velocity<0) velocity=0;
      if(!c) play(0,0,velocity); //otherwise hammer-on with reduced velocity
      else if(c) chord(sD);
    }
    else{
      if(c) drop=true;
      play(0,0,int(velocity/2));
    }
  }
  return;
}
