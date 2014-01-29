#include <MIDI.h>

// the 'd' (DOWN) and 'u' (UP) variables refer to the start and select buttons.

// edit these defines to customize your guitar's capabilities!
#define MIDIOUT 1
// where the octave MUST be between 0 and 10:
#define INIT_OCTAVE 4
#define MIN_OCTAVE 3
#define MAX_OCTAVE 8
// dmg gameboy has normal range of 3 to 8

// change your pot values here:
#define POT_MAX 400
#define POT_MIN 0
#define WHAM_MAX -4000
// -------------------------------

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

int velocity=127;
byte noteMem[3]={0,0,0};

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
  for(int i=0; i<3; i++) g[i]=r[i]=y[i]=b[i]=o[i]=u[i]=d[i]=s[i]=false;
  timer=millis();
}

void loop() {
  unsigned long mils=millis();
  int w=0;
  int wham=analogRead(whammy);
  if((mils-timer)>0) {
    tick*=(-1);
    checkInput(); // input tracker
    checkKeyLocks(); // Most of the work is done here.
    timer=mils;
  }
  if(wham&&(wham<=400)){
    w=map(analogRead(whammy), POT_MIN, POT_MAX, WHAM_MAX, 0);
    MIDI.sendPitchBend(w,MIDIOUT);
  }
}

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
    velocity=127;
    if(!c) play(0,0,velocity); //new note
    else if(c) chord(sD);
  }
  else if((g[2]!=g[1])||(r[2]!=r[1])||(y[2]!=y[1])||(b[2]!=b[1])||(o[2]!=o[1])) if(!silent){ // have the frets changed?
    if(!STRUM) killNote(); //if frets change and strum isn't depressed, kill the note.
    else if(!((!GREEN)&&(!RED)&&(!YELLOW)&&(!BLUE)&&(!ORANGE))){
      velocity-=1; // comment this line out to remove velocity reduction during hammer-ons
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
