#include <Arduino.h>
#include <stdio.h>
#include "SquareSynth.h"

///////////////////////////////////////////////////////////////
////             Instance government code here             ////
///////////////////////////////////////////////////////////////
SquareSynth_Class SquareSynth;
Synth_Class *Channel;
Synth_Class Synth;

SquareSynth_Class::SquareSynth_Class(){
  // nada.
}

SquareSynth_Class::~SquareSynth_Class(){
  // zilch.
}

void SquareSynth_Class::begin(int synths, ...){
  Synth.~Synth_Class(); // remove single version of object, as not being used.
  _synthCount=synths;
  Channel = new Synth_Class [synths]; // allocate array at specified size.
  
  va_list pins; // the va_ macro functions are the handlers for unknown arguments. look them up.
  va_start(pins, synths); // pins= the unknown arguments, synths= the number of arguments (specified by user)
  
  int pinValue;
  for(int i=0; i<synths; i++) {
    pinValue = va_arg(pins, int); // case of the value to be returned must be specified!
    pinMode(pinValue,OUTPUT);
    Channel[i].begin(pinValue); // it all goes straight to global, so doesn't destroy at end of function!
  }
  
  va_end(pins);
  return;
}
///////////////////////////////////////////////////////////////
////                  Automation code here                 ////
///////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////
////                  Base synth code here                 ////
///////////////////////////////////////////////////////////////

Synth_Class::Synth_Class(){
  // nada.
}

Synth_Class::~Synth_Class(){
  // zilch.
}

const unsigned long Synth_Class::_midiMap[128]={122312,115447,108967,102851,97079,91630,86487,81633,77051,72727,68645,64792,61156,57723,54483,51425,48539,45815,43243,40816,38525,36363,34322,32396,30578,28861,27241,25712,24269,22907,21621,20408,19262,18181,17161,16198,15289,14430,13620,12856,12134,11453,10810,10204,9631,9090,8580,8099,7644,7215,6810,6428,6067,5726,5405,5102,4815,4545,4290,4049,3822,3607,3405,3214,3033,2863,2702,2551,2407,2272,2145,2024,1911,1803,1702,1607,1516,1431,1351,1275,1203,1136,1072,1012,955,901,851,803,758,715,675,637,601,568,536,506,477,450,425,401,379,357,337,318,300,284,268,253,238,225,212,200,189,178,168,159,150,142,134,126,119,112,106,100,94,89,84,79};

void Synth_Class::begin(int pin){
  _pin=_microWavelength=0;
  _microTimerWave=micros();
  _noise=false;
  _dutyCycle=50;
  for(int j=0; j<=MAX_ARPEGGIO; j++) _arpeggio[j]=0;
  _arpeggioCount=0;
  _arpeggioTrack=0;
  _pin=pin;
  _note=60;
}

void Synth_Class::generate(){
  if(!_microWavelength) return; // setting the wavelength to 0 will stop the wave.
  else {
    _microTimerDuty=micros()-_microTimerWave; // keeps track of exact point of duty cycle
    if(_microTimerDuty<=_microWavelength*_dutyCycle) {
      if(!_high) {
        digitalWrite(_pin, HIGH);
        _high=true;
      }
    }
    else{
      if(_high) {
        digitalWrite(_pin, LOW);
        _high=false;
      }
      if(_microTimerDuty>=_microWavelength) { // this runs at the end of each cycle.
        _microTimerWave=micros(); // when duty completes, reset wave timer
        if(_noise) _dutyCycle=random(_minDuty,_maxDuty)*0.01; // alter next duty cycle for pseudo-noise
        if(_arpeggioCount){
          _microWavelength=_midiMap[_arpeggio[_arpeggioTrack]+_note];
          if(_arpeggioTrack>=_arpeggioCount) _arpeggioTrack=0;
          else _arpeggioTrack++;
        }
      }
    }
  }
  return;
}

void Synth_Class::noteOn(int note, int duty){
  _note=note;
  if(duty) dutyCycle(duty);
  _microWavelength=_midiMap[note];
  _noise=false;
  return;
}

void Synth_Class::pitchBend(int bend){
  if(!bend) _microWavelength=_note;
  else _microWavelength=map(bend,-1000,1000,_midiMap[_note-2],_midiMap[_note+2]);
  return;
}

void Synth_Class::arpeggioOn(int offset1, int offset2, int offset3, int offset4){
  // Messy, but efficient.
  _arpeggio[1]=offset1;
  if(offset2) {
    _arpeggio[2]=offset2;
    if(offset3) {
      _arpeggio[3]=offset3;
      if(offset4) {
        _arpeggio[4]=offset4;
        _arpeggioCount=4;
      }
      else _arpeggioCount=3;
    }
    else _arpeggioCount=2;
  }
  else _arpeggioCount=1;
  return;
}

void Synth_Class::arpeggioOff(){
  for(int j=0; j<=MAX_ARPEGGIO; j++) _arpeggio[j]=0;
  _arpeggioCount=0;
  _arpeggioTrack=0;
  return;
}

void Synth_Class::dutyCycle(int percent){
  _dutyCycle=percent*0.01;
  _noise=false;
  return;
}

void Synth_Class::noise(int note, int minDuty, int maxDuty){
  _note=note;
  _minDuty=minDuty;
  _maxDuty=maxDuty;
  _microWavelength=_midiMap[note];
  _noise=true;
  return;
}

void Synth_Class::noteOff(){
  _microWavelength=0;
  _noise=false;
  return;
}
