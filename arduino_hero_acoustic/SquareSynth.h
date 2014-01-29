#ifndef SquareSynth_h
#define SquareSynth_h

#include <Arduno.h>

// change these if you know what you're doing
#define MAX_ARPEGGIO 4 // also edit sendArpeggioOn()
// ------------------------------------------

class Synth_Class {
  public:
    Synth_Class();
    ~Synth_Class();
    void begin(int pin);
    void generate(); // place in loop for wave generation.
    void noteOn(int note=60, int duty=50);
    void pitchBend(int bend=0);
    void arpeggioOn(int offset1, int offset2=0, int offset3=0, int offset4=0);
    void arpeggioOff();
    void dutyCycle(int percent);
    void noise(int note=60, int minDuty=1, int maxDuty=51);
    void noteOff();
  private:
    bool _noise; // checker for noise mode
    bool _high; // speed optimization. no need to write HIGH when it's already on!
    int _arpeggio[MAX_ARPEGGIO+1]; // checker for arpeggio values
    int _arpeggioCount; // amount of notes to arpeggiate
    int _arpeggioTrack; // keeps track of arpeg. cycling in generate()
    int _note; // keep track of MIDI note value for reference
    int _pin; // stores the pin number
    float _dutyCycle; //stores duty cycle percentage, in decimal form.
    int _minDuty; // optional paramaters for the noise generator
    int _maxDuty; //-^
    unsigned long _microWavelength; // stores wavelength in microseconds
    unsigned long _microTimerWave; // wavelength timer
    unsigned long _microTimerDuty; // duty cycle timer
    static const unsigned long _midiMap[128]; // mappings of midi to wavelength in microseconds
};

class SquareSynth_Class {
  public:
    SquareSynth_Class();
    ~SquareSynth_Class();
    void begin(int synths, ...);
    //void tempo(int bpm);
    //void generate(); // acts the same as Synth's generate, but calls all instances.
    //void 
  private:
    int _synthCount;
    
};

extern SquareSynth_Class SquareSynth;
extern Synth_Class *Channel;
extern Synth_Class Synth;

#endif
