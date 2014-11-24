#ifndef SquareSynth_h
#define SquareSynth_h

#include <Arduno.h>
#include "SquareSynthNoteDefs.h"

// change these if you know what you're doing
#define MAX_ARPEGGIO 4 // also edit sendArpeggioOn()
// ------------------------------------------

    ////////////////////////////////////////////////
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
    void freqNoise(int mod=4); // generates a random freqency, compared to the current note.
    void noteOff();
    void transposeOn(int transposition);
    void transposeOff();
    void clearFlags();
    void softKill(int steps=2);
    
    /////////////////////////////////////////////  Don't use these unless
    ////    Automation-oriented functions    ////  You're using the
    /////////////////////////////////////////////  SquareSynth governor!
    void transform(int destination, int steps); // transforms current note to specified note, spanning x many steps (16th notes)
    void addDepth(int duty=15, int steps=1); // bends the duty cycle briefly, starting at specified duty and ending at original; spanning x many steps (16th notes)
    void autoKill(int steps=1, bool killArpeggio=false, bool killFreqNoise=false); // kills note after specified number of steps. the boolean arguments are used in some of the drum synth functions.

    ////////////////////////////////////////////////
    // High-level routines (instruments & stuff!) //
    ////////////////////////////////////////////////
    
    // Simple commands:
    void note(int pitch=60, int duty=50, int depth=30, int steps=1);
    
  private:
    bool _noise; // noise mode flag
    bool _freqNoise; // frequency-based noise flag
    bool _high; // speed optimization. no need to write HIGH when it's already on!
    int _transposition; // stores amount of semitones to transpose by.
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
    int _freqMod;
    int _freqBaseNote; // the midi note sample referenced during generate for frequency noise.
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
    unsigned long _transDestinationFreq; // Stores frequency (truncated *0.01) to save time in generate().
    unsigned long _transInterval; // transformation interval in microseconds.
    unsigned long _transStart; // time transform was called. (in micros)
    bool _transform; // transform flag.
    // end of transform vars.
    
    // addDepth vars:
    int _depthArgument; // stores the duty cycle to bend from.
    unsigned long _depthInterval; // how long the bend will take in microseconds.
    unsigned long _depthStart; // when the bend starts (for reference)
    bool _addDepth; // addDepth flag.
    // end of addDepth vars.
    
    // autoKill vars:
    bool _autoKill; // autoKill flag.
    bool _killArpeggio; // kill arpeggio too?
    bool _killFreqNoise; // kill frequencyMod too?
    unsigned long _autoKillDelay; // delay until note is killed
    unsigned long _autoKillStart; // stores the time of triggering.
    // end of autoKill vars.
};

extern Synth_Class Synth; // Note that 'SquareSynth' commands won't work on the basic 'Synth' object.

#endif
