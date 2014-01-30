#ifndef SquareSynth_h
#define SquareSynth_h

#include <Arduno.h>
#include "SquareSynthNoteDefs.h"

// change these if you know what you're doing
#define MAX_ARPEGGIO 4 // also edit sendArpeggioOn()
// ------------------------------------------

    /////////////////////////////////////////////
    ////         SquareSynth governor        ////
    /////////////////////////////////////////////
    
    ///////////////////////////////////////////////////
    // This class (keyword 'SquareSynth') initialises//
    // and governs an infinite number of instances   //
    // of Synth_Class held in the Channel[] array.   //
    ///////////////////////////////////////////////////
    // Alternatively, you can use just one Synth     //
    // using Synth.begin(int pinNumber);             //
    ///////////////////////////////////////////////////
    
class SquareSynth_Class {
  public:
    SquareSynth_Class();
    ~SquareSynth_Class();
    void begin(int synths, ...); // initialise as many synth channels as you want here! (they appear in the Channel[] array)
    void tempo(int bpm); // set a new bpm at any time with this function.
    // timing chunks:
    // use these for timing your notes. apply instructions, then use
    // one of these to specify the amount of beats before your
    // next instruction.
    void proceed(int modifier=1); // the argument here is the amount of sixteenth notes you want the function to run for.
    void sixteenth(); // sixteenth is the same as proceed with no arguments.
    void eighth();
    void quarter();
    void half();
    void whole(); // this is all pretty straightforward.
    // end of timing chunks.
    void generate(); // acts the same as Synth/Channel[]'s generate, but calls all instances. Use this over the timing blocks for HID sketches.
  private:
    int _synthCount;
    int _tempo; // tempo in millis per 16th note
};

    ////////////////////////////////////////////////
    ////   Synth (and Channel[]) object class   ////
    ////////////////////////////////////////////////
    
    ////////////////////////////////////////////////
    // Can be initialised by SquareSynth governor //
    // or you can just use one synth with         //
    // Synth.begin(pinNumber);                    //
    ////////////////////////////////////////////////

class Synth_Class {
  public:
    Synth_Class();
    ~Synth_Class();
    void begin(int pin);
    void generate(); // place in loop for wave generation.
    void noteOn(int pitch=60, int duty=50);
    void pitchBend(int bend=0); // not fully tested yet!
    void arpeggioOn(int offset1, int offset2=0, int offset3=0, int offset4=0);
    void arpeggioOff();
    void dutyCycle(int percent);
    void noise(int pitch=60, int minDuty=1, int maxDuty=51);
    void noteOff();
    void clearFlags();
    
    /////////////////////////////////////////////  Don't use these unless
    ////    Automation-oriented functions    ////  You're using the
    /////////////////////////////////////////////  SquareSynth governor!
    void _recievetempo(int tempoVal); // Don't use this! Ever! Unless you want some crazy prog stuff!
    void transform(int destination, int steps); // transforms current note to specified note, spanning x many steps (16th notes)
    void addDepth(int duty=15, int steps=1); // bends the duty cycle briefly, starting at specified duty and ending at original; spanning x many steps (16th notes)
    
    ////////////////////////////////////////////////
    // High-level routines (instruments & stuff!) //
    ////////////////////////////////////////////////
    
    // Simple commands:
    void note(int pitch=60, int duty=50, int depth=30, int steps=1);
    
    // Instrument commands:
    
    
    // Drumkit commands:
    void cymbal(int pitch, int decay, int steps);
    void tom(int pitch, int decay, int steps);
    void kick(int pitch, int decay, int steps);
    void hihat(int pitch, int decay, int steps);
    void hihatOpen(int pitch, int decay, int steps);
    void snare(int pitch, int decay, int steps);
    
  private:
    bool _noise; // noise mode flag
    bool _high; // speed optimization. no need to write HIGH when it's already on!
    int _arpeggio[MAX_ARPEGGIO+1]; // checker for arpeggio values
    int _arpeggioCount; // amount of notes to arpeggiate, also serves as flag.
    int _arpeggioTrack; // keeps track of arpeg. cycling in generate()
    int _note; // keep track of MIDI note value for reference
    int _volatileNote; // mutate this instead of _note
    int _pin; // stores the pin number
    float _dutyCycle; //stores user-defined duty cycle, in decimal form.
    float _volatileDuty; // duty cycle used in generate(). can be changed by automation flags.
    int _minDuty; // optional paramaters for the noise generator
    int _maxDuty; //-^
    unsigned long _microWavelength; // stores wavelength in microseconds
    unsigned long _microTimerWave; // wavelength timer
    unsigned long _microTimerDuty; // duty cycle timer
    static const unsigned long _midiMap[128]; // mappings of midi to wavelength in microseconds
    /////////////////////////////////////////////
    ////    Automation-oriented variables    ////
    /////////////////////////////////////////////
    unsigned long _tempo; // tempo in millis, passed here from SquareSynth governor.
    // transform function vars:
    int _transDestinationNote; // Stores note to transform to.
    unsigned long _transDestinationFreq; // Stores frequency (truncated *0.01) to save calculation.
    unsigned long _transInterval; // transformation interval in microseconds.
    unsigned long _transStart; // time transform was called. (in micros)
    bool _transform; // transform mode flag
    // end of transform vars.
    // addDepth vars:
    int _depthArgument;
    unsigned long _depthInterval;
    unsigned long _depthStart;
    bool _addDepth;
    // end of addDepth vars.
    
};

extern SquareSynth_Class SquareSynth;
extern Synth_Class *Channel;
extern Synth_Class Synth; // Note that SquareSynth commands wont work on this 'Synth' object.

#endif
