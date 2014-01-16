arduino-hero
============

A midi adapter for any guitar hero controller!

This works for pretty much any GH or RB guitar controller out there, as it just 

intercepts the button presses and the potentiometer. You can then send the signal 

through the arduino's usb (via serial to midi program. I recommend Hairless and 

loopMIDI), or through an installed midi port. (WARNING I have only tested on my 

computer. If it isn't working, the baud rate may need to be changed to work with 

a midi port.)

Can play in a range of 2 2/3 octaves (32 notes) through increasingly intense fret 

progressions. In general, the more fret buttons held down, the higher the note. 

There's no option toggling or anything fancy, so you play it just like you would 

in the video game.

The code is commented well enough for you to change things if you like. reading 

the comments in fret_map.ino will give you an idea of how to use the frets.

that said:
pressing start+select will toggle between chords and single notes.
in chord mode, strum down for a major chord and up for a minor chord.

you can hammer-on when you keep the strum bar pressed down.

whammy bar sends a pitchbend signal

to increase/decrease the base octave you play at, press select to go up, and 

start to go down.

http://projectgus.github.io/hairless-midiserial/
http://www.tobias-erichsen.de/software/loopmidi.html
