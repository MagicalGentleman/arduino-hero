#include "Squaresynth.h"

// the 'd' (DOWN) and 'u' (UP) variables refer to the start and select buttons.

// WHAMMY BAR COMING SOON
// I have to finish SquareSynth first.

// edit these defines to customize your guitar's capabilities!
// where the octave MUST be between 0 and 10:
#define INIT_OCTAVE 4
#define MIN_OCTAVE 2
#define MAX_OCTAVE 6
// so far, stable range of 2 to 6 (and 2/3)

// DON'T CHANGE THESE DEFINES!-----------------
#define GREEN g[0]
#define RED r[0]
#define YELLOW y[0]
#define BLUE b[0]
#define ORANGE o[0]
#define STRUM s[0]
#define DOWN d[0]
#define UP u[0]
// --------------------------------------------

// pins
const int green=2;
const int red=3;
const int yellow=4;
const int blue=5;
const int orange=6;
const int start=7;
const int select=8;
const int strumD=9;
const int strumU=10;

int duty;
byte noteMem[3]={0,0,0};
unsigned long midi[127];

//GREEN,RED,YELLOW,BLUE,ORANGE,STRUM,(UP/SELECT),(DOWN/START),CHORD//
boolean g[3],r[3],y[3],b[3],o[3],s[3],u[3],d[3];
boolean c;

boolean sD; // Strum direction. up=true, down=false.
boolean cLift=false;
boolean drop=false;
boolean silent=true;

boolean skip=false;

byte octave=4;
int tick=1;
unsigned long timer;
unsigned long mils;
unsigned long trigger=0;
unsigned long triggerReference;

void setup() {
  Synth.begin(11);
  pinMode(11, OUTPUT);
  pinMode(green, INPUT);
  pinMode(red, INPUT);
  pinMode(yellow, INPUT);
  pinMode(blue, INPUT);
  pinMode(orange, INPUT);
  pinMode(strumD, INPUT);
  pinMode(strumU, INPUT);
  pinMode(start, INPUT);
  pinMode(select, INPUT);
  for(int i=0; i<3; i++) g[i]=r[i]=y[i]=b[i]=o[i]=u[i]=d[i]=s[i]=false;
  timer=millis();
  triggerReference=millis();
}

void loop() {
  mils=millis();
  if((mils-timer)>5) {
    tick*=(-1);
    checkInput(); // input tracker
    checkKeyLocks(); // Most of the work is done here.
    addDepth(); // changes duty cycle over time for some depth
    timer=mils;
  }
  Synth.generate();
}

//---------------------------------------------------------------

void checkKeyLocks() {
  if((u[2]!=u[1])&&(!UP)){       //the instant select is lifted
    if(DOWN){                  //if start is held down
      c=(!c);               // toggle chord mode
      if(!c) cLift=true;
      skip=true;
    }
    else if(skip) skip=false;
    else if(octave<(MAX_OCTAVE-1)) octave++;
  }
  else if((d[2]!=d[1])&&(!DOWN)){  //the instant start is lifted
    if(UP){                  //if select is held down
      c=(!c);               // toggle chord mode
      if(!c) cLift=true;
      skip=true;
    }
    else if(skip) skip=false;
    else if(octave>MIN_OCTAVE) octave--;
  }
  else if((s[1]!=s[2])&&STRUM){ // check for strum
    if(!c) play(); //new note
    else if(c) chord(sD);
  }
  else if((g[2]!=g[1])||(r[2]!=r[1])||(y[2]!=y[1])||(b[2]!=b[1])||(o[2]!=o[1])) if(!silent){ // have the frets changed?
    if(!STRUM) killNote(); //if frets change and strum isn't depressed, kill the note.
    else if(!((!GREEN)&&(!RED)&&(!YELLOW)&&(!BLUE)&&(!ORANGE))){
      if(!c) play(); // hammer-on!
      else if(c) chord(sD);
    }
    else{
      play(); // keep this here if you want pullups to go with your hammer-ons
    }
  }
  return;
}
