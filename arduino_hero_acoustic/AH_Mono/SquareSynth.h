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
    void proceed(int modifier=1); // the argument here is the amount of 32nd notes you want the function to run for.
    void sixteenth(); // sixteenth is the same as proceed with no arguments.
    void eighth();
    void quarter();
    void half();
    void whole(); // this is all pretty straightforward.
    // end of timing chunks.
    
    void generate(); // acts the same as Synth/Channel[]'s generate, but calls all instances. Use this over the timing blocks for HID sketches. Or MIDI interfacing.
  private:
    int _synthCount;
    unsigned long _tempo; // tempo in micros per 32nd note
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
    void frequencyNoise(int pitch=60, int negMod=15, int posMod=15); // generates a random freqency, compared to the current note.
    void noteOff();
    void transposeOn(int transposition);
    void transposeOff();
    void clearFlags();
    
    /////////////////////////////////////////////  Don't use these unless
    ////    Automation-oriented functions    ////  You're using the
    /////////////////////////////////////////////  SquareSynth governor!
    void _recievetempo(unsigned long tempoVal); // Don't use this! Ever! Unless you want some crazy prog stuff!
    void transform(int destination, int steps); // transforms current note to specified note, spanning x many steps (16th notes)
    void addDepth(int duty=15, int steps=1); // bends the duty cycle briefly, starting at specified duty and ending at original; spanning x many steps (16th notes)
    void autoKill(int steps=1, bool killArpeggio=false, bool killFreqNoise=false); // kills note after specified number of steps. the boolean arguments are used in some of the drum synth functions.
    //void clip(int percent, int steps=0); // cuts out a percentage of the waveforms to make the sound more jagged.
    //void clipOff(); // stops clipping.
    
    ////////////////////////////////////////////////
    // High-level routines (instruments & stuff!) //
    ////////////////////////////////////////////////
    
    // Simple commands:
    void note(int pitch=60, int duty=50, int depth=30, int steps=1);
    
    // Instrument commands:
    
    
    // Drumkit commands:
    void cymbal(int pitch=80, int decay=6, int steps=1);
    void tom(int pitch=60, int decay=4, int steps=3);
    void kick(int pitch=36, int decay=3, int steps=1);
    void hihat(int pitch=70, int decay=1, int steps=1);
    void hihatOpen(int pitch=70, int decay=4, int steps=4);
    void snare(int pitch=60, int decay=4, int steps=1);
    
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
    int _freqNegMod;
    int _freqPosMod;
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
    
    // clip vars:
    //bool _clip; // clip flag for calculation
    //bool _clipping; // check for whether clipping is in effect.
    //unsigned long _clipInterval; // (optional) delay until clipping ends.
    //unsigned long _clipStart; // time of flagging.
    //int _clipCount; // keeps track of cycle count.
    //int _clipPercent; // percentage of cycles to skip.
    // end of clip vars.
    
};

extern SquareSynth_Class SquareSynth;
extern Synth_Class *Channel; // If you're using multiple channels, and controlling timing and such with the 'SquareSynth' object, this is the one to use.
extern Synth_Class Synth; // Note that 'SquareSynth' commands won't work on the basic 'Synth' object.

#endif
