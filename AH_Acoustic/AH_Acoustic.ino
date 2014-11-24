#include "SquareSynth.h"

// the 'd' (DOWN) and 'u' (UP) variables refer to the start and select buttons.

// edit these defines to customize your guitar's capabilities!

// where the octave MUST be between 0 and 10:
#define INIT_OCTAVE 4
#define MIN_OCTAVE 2
#define MAX_OCTAVE 6
// usable range of 2 to 6 (plus 2/3)

#define CLEAN_DUTY_CYCLE 50 // Sets the duty cycle you play your clean notes at.
#define DIRTY_DUTY_CYCLE 20 // Sets the duty cycle you play dirty notes at.

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

int duty[2];

//GREEN,RED,YELLOW,BLUE,ORANGE,STRUM,(UP/SELECT),(DOWN/START),CHORD//
boolean g[3],r[3],y[3],b[3],o[3],s[3],u[3],d[3];
boolean c=false;

boolean slide=true;
boolean dutyBool=false;
boolean sD; // Strum direction. up=true, down=false.
boolean cLift=false;
boolean drop=false;
boolean silent=true;
boolean hammerOn=false;
boolean skip=false;
boolean ignore=false;

int pitchMem=60;
int noteGap=0;

byte octave=3;
bool tick=true;
unsigned long timer;
unsigned long mils;
int timerDiff=20;

void setup() {
  Synth.begin(11); // SquareSynth declares the pin output for you.
  pinMode(green, INPUT);
  pinMode(red, INPUT);
  pinMode(yellow, INPUT);
  pinMode(blue, INPUT);
  pinMode(orange, INPUT);
  pinMode(strumD, INPUT);
  pinMode(strumU, INPUT);
  pinMode(start, INPUT);
  pinMode(select, INPUT);
  
  duty[0]=CLEAN_DUTY_CYCLE;
  duty[1]=DIRTY_DUTY_CYCLE;
  for(int i=0; i<3; i++) g[i]=r[i]=y[i]=b[i]=o[i]=u[i]=d[i]=s[i]=false;
  timer=millis();
}

void loop() {
  mils=millis();
  if((mils-timer)>timerDiff) {
    tick=!tick;
    checkInput(); // input tracker
    checkKeyLocks(); // Most of the work is done here.
    if(!silent) whammyCheck(); // Runs a check on the whammy bar.
    timer=mils;
    noteGap+=timerDiff;
  }
  Synth.generate();
}

//---------------------------------------------------------------

void checkKeyLocks() {
  if(UP&&DOWN&&!ignore) {
    if(skip) return;
    else liftRoutine();
  }
  else if(skip) skip=false;
  
  if((u[2]!=u[1])&&(!UP)){       //the instant select is lifted
    if(DOWN){
      ignore=true;
      return;
    }
    else if(ignore){
      ignore=false;
      return;
    }
    if(octave<(MAX_OCTAVE-1)) octave++;
  }
  
  else if((d[2]!=d[1])&&(!DOWN)){  //the instant start is lifted
    if(UP){
      ignore=true;
      return;
    }
    else if(ignore){
      ignore=false;
      return;
    }
    if(octave>MIN_OCTAVE) octave--;
  }
  
  else if((s[1]!=s[2])&&STRUM){ // check for strum
    if(!c) play(); //new note
    else if(c) chord(sD);
  }
  
  else if((g[2]!=g[1])||(r[2]!=r[1])||(y[2]!=y[1])||(b[2]!=b[1])||(o[2]!=o[1])) if(!silent){ // have the frets changed?
    if(!STRUM) killNote(); //if frets change and strum isn't depressed, kill the note.
    else if(!((!GREEN)&&(!RED)&&(!YELLOW)&&(!BLUE)&&(!ORANGE))){
      hammerOn=true;
      if(!c) play(); // hammer-on!
      else if(c) chord(sD);
    }
    else{
      hammerOn=true;
      play(); // keep this here if you want pullups to go with your hammer-ons
    }
  }
  return;
}

void whammyCheck(){
  int wham=constrain(analogRead(whammy),19,401);
  if(wham>400 || wham<20) return;
  else Synth.pitchBend(map(wham,20,400,-1000,0));
  return;
}

void liftRoutine(){
  slide=c;
  c=(!c);               // toggle chord mode
  if(!c) cLift=true;
  skip=true;
  return;
}
