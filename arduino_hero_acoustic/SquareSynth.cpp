#include <Arduino.h>
#include <stdio.h>
#include "SquareSynth.h"

// Note: all the high-level (stuff you'll actually use)
//       functions are at the bottom!
//       e.g. instrument blueprints, realistic notes, etc.

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
  //Synth.~Synth_Class(); // remove single version of object, as not being used.
  _tempo=125; // initial tempo is 120bpm. (this is in millis per 16th note)
  _synthCount=synths;
  Channel = new Synth_Class [synths]; // allocate array at specified size.
  
  va_list pins; // the va_ macro functions are the handlers for unknown arguments. look them up.
  va_start(pins, synths); // pins= the unknown arguments, synths= the number of arguments (specified by user)
  
  int pinValue;
  for(int i=0; i<synths; i++) {
    pinValue = va_arg(pins, int); // case of the value to be returned must be specified!
    Channel[i].begin(pinValue); // it all goes straight to global, so doesn't destroy at end of function!
    Channel[i]._recievetempo(_tempo);
  }
  
  va_end(pins);
  return;
}
///////////////////////////////////////////////////////////////
////                    Timing code here                   ////
///////////////////////////////////////////////////////////////

void SquareSynth_Class::tempo(int bpm){
  _tempo=60000/(bpm*4); // value in millis for a sixteenth note.
  for(int i=0; i<_synthCount; i++) Channel[i]._recievetempo(_tempo);
  return;
}

void SquareSynth_Class::proceed(int modifier){
  long unsigned tempoTimer=millis();
  int period=_tempo*modifier;
  do{
    for(int i=0; i<_synthCount; i++) Channel[i].generate();
  }while(millis()-tempoTimer<=period);
  return;
}
//----some shortcut routines----//
void SquareSynth_Class::sixteenth(){
  proceed();
  return;
}
void SquareSynth_Class::eighth(){
  proceed(2);
  return;
}
void SquareSynth_Class::quarter(){
  proceed(4);
  return;
}
void SquareSynth_Class::half(){
  proceed(8);
  return;
}
void SquareSynth_Class::whole(){
  proceed(16);
  return;
}
//------------------------------//

void SquareSynth_Class::generate(){
  for(int i=0; i<_synthCount; i++) Channel[i].generate();
  return;
}

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
  pinMode(pin,OUTPUT);
  _pin=_microWavelength=0;
  _microTimerWave=micros();
  _dutyCycle=_volatileDuty=50;
  for(int j=0; j<=MAX_ARPEGGIO; j++) _arpeggio[j]=0;
  _arpeggioCount=0;
  _arpeggioTrack=0;
  _pin=pin;
  _note=60; // default note.
  _tempo=125; // default tempo (120bpm)
  _noise=false;
  _transform=false;
  _addDepth=false;
}

void Synth_Class::generate(){
  if(!_microWavelength) return; // setting the wavelength to 0 will stop the wave.
  else {
    unsigned long timeNow=micros();
    _microTimerDuty=timeNow-_microTimerWave; // keeps track of exact point of duty cycle
    if(_microTimerDuty<=_microWavelength*(1-_volatileDuty)) {
      // the duty cycle is actually reversed here.(e.g. --_ = _-- and -__ = __-)
      // The reason for this is that I want to leave the flag
      // calculation to run during the dead (LOW) space of the wave.
      // Since most users use the % range of 0 to 50, Calculating
      // the flagged functions may eat into the time specified by
      // that duty cycle, extending it and returning false results.
      // calculating inbetween each waveform would just lower the
      // frequency depending on how long it takes to resolve the
      // flagged items.
      // So, calculating while the wave is LOW would be most efficient
      // since statistically, the time spent on LOW will be longer
      // than HIGH anyways.
      // If you're too purist, just remove the 1- in (1-_volatileDuty)
      // to change it back to a non-reversed duty cycle!
      if(!_high) { // this is actually second
        digitalWrite(_pin, HIGH);
        _high=true;
      }
    }
    else{ // this is actally first
      if(_high) {
        digitalWrite(_pin, LOW);
        _high=false;
      }
      if(_microTimerDuty>=_microWavelength) { // this runs at the end of each cycle.
        _microTimerWave=timeNow; // when duty completes, reset wave timer
        digitalWrite(_pin, LOW); // this fixes problems with signal stretching while flags are being resolved!
        _high=false;
        
        // are any flags set?
        
        // Duty cycle flags:
        
        // Deals with _volatileDuty, to preserve the last user-defined duty cycle (_dutyCycle)
        // noise generation shouldn't affect the other flagged functions, so it's up here.
        if(_noise) _volatileDuty=random(_minDuty,_maxDuty)*0.01; // alter next duty cycle for pseudo-noise
        
        else if(_addDepth){ // condensing with else, as these two flags shouldn't run at the same time anyways.
          unsigned long depthMap=(timeNow-_depthStart)*0.1; // this is truncated so it works with map.
          if(depthMap<=_depthInterval) _volatileDuty=0.01*map(depthMap,0,_depthInterval,_depthArgument,(_dutyCycle*100));
          else {
            _volatileDuty=_dutyCycle;
            _addDepth=false;
          }
        }
        
        // Frequency altering flags:
        // the following deal with _volatileNote and altering the wavelength:
        
        // as arpeggio calls a new note from the start, it should resolve first.
        if(_arpeggioCount){
          _volatileNote=_note+_arpeggio[_arpeggioTrack]; // _volatileNote is assigned, so transform works correctly. (if flagged)
          _microWavelength=_midiMap[_volatileNote];
          if(_arpeggioTrack>=_arpeggioCount) _arpeggioTrack=0;
          else _arpeggioTrack++;
        }
        
        // transform should resolve last, as it needs a final note value to alter.
        if(_transform) {
          unsigned long transMap=(timeNow-_transStart)*0.1;
          if(transMap<=_transInterval) _microWavelength=10*map(transMap,0,_transInterval,(_midiMap[_volatileNote])*0.1,_transDestinationFreq);
          else {// transform complete, assign new note and make sure it's the right one!
            _note=_volatileNote=_transDestinationNote;
            _microWavelength=_midiMap[_note];
            _transform=false;
          }
        }
      }
    }
  }
  return;
}

void Synth_Class::noteOn(int pitch, int duty){
  _note=_volatileNote=pitch;
  if(duty) dutyCycle(duty);
  _microWavelength=_midiMap[pitch];
  return;
}

void Synth_Class::pitchBend(int bend){
  if(!bend) _microWavelength=_midiMap[_volatileNote];
  else _microWavelength=10*map(bend,-1000,1000,_midiMap[_volatileNote-2]*0.1,_midiMap[_volatileNote+2]*0.1);// again, some truncation for the map() function.
  return;
}

void Synth_Class::transform(int destination, int steps){
  _transInterval=steps*_tempo*100; // everything has to be truncated a bit for map(). will all be expanded afterwards.
  _transStart=micros();
  _transDestinationFreq=(_midiMap[destination])*0.1; // same as above
  _transDestinationNote=destination;
  _transform=true;
  return;
}

void Synth_Class::addDepth(int duty, int steps){
  _depthInterval=steps*_tempo*100;
  _depthStart=micros();
  _depthArgument=duty;
  _addDepth=true;
  return;
}

void Synth_Class::arpeggioOn(int offset1, int offset2, int offset3, int offset4){
  // Messy, but efficient.
  // _arpeggio[0] is void, so the base note is called at this array index.
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
  _microWavelength=_midiMap[_note];
  return;
}

void Synth_Class::dutyCycle(int percent){
  _volatileDuty=_dutyCycle=percent*0.01;
  _noise=false;
  return;
}

void Synth_Class::noise(int pitch, int minDuty, int maxDuty){
  _note=_volatileNote=pitch;
  _minDuty=minDuty;
  _maxDuty=maxDuty;
  _microWavelength=_midiMap[pitch];
  _noise=true;
  _addDepth=false;
  return;
}

void Synth_Class::noteOff(){
  _microWavelength=0;
  _transform=false; // automation will continue unexpectedly otherwise.
  _addDepth=false; // same here!
  return;
}

void Synth_Class::clearFlags(){
  _transform=false;
  _addDepth=false;
  _noise=false;
  arpeggioOff();
  return;
}

void Synth_Class::_recievetempo(int tempoVal){
  _tempo=tempoVal;
  return;
}

///////////////////////////////////////////////////////////////
////               High-level routines here                ////
///////////////////////////////////////////////////////////////

// Simple commands:

void Synth_Class::note(int pitch, int duty, int depth, int steps){
  noteOn(pitch,duty);
  addDepth(depth,steps);
  return;
}


// Instrument commands:




// Drumkit commands:

void Synth_Class::cymbal(int pitch, int decay, int steps){
  noise(pitch);
  transform(pitch-decay, steps);
  
  return;
}

void Synth_Class::tom(int pitch, int decay, int steps){
  note(pitch);
  transform(pitch-decay, steps);
  
  return;
}

void Synth_Class::kick(int pitch, int decay, int steps){
  note(pitch);
  transform(pitch-decay, steps);
  
  return;
}

void Synth_Class::hihat(int pitch, int decay, int steps){
  note(pitch);
  transform(pitch-decay, steps);
  
  return;
}

void Synth_Class::hihatOpen(int pitch, int decay, int steps){
  noise(pitch);
  transform(pitch-decay, steps);
  
  return;
}

void Synth_Class::snare(int pitch, int decay, int steps){
  noise(pitch);
  transform(pitch-decay, steps);
  
  return;
}
