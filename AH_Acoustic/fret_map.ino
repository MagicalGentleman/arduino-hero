byte frets(boolean g, boolean r, boolean y, boolean b, boolean o, byte oct) {
  byte octave=oct;
  byte octave2=(octave+1);
  byte octave3=(octave2+1);
  // ----- // C
  if((!g)&&(!r)&&(!y)&&(!b)&&(!o)) return pitch(0, octave);
  
  // G---- // CS
  else if(g&&(!r)&&(!y)&&(!b)&&(!o)) return pitch(1, octave);
  
  // -R--- // D
  else if((!g)&&r&&(!y)&&(!b)&&(!o)) return pitch(2, octave);
  
  // --Y-- // DS
  else if((!g)&&(!r)&&y&&(!b)&&(!o)) return pitch(3, octave);
  
  // ---B- // E
  else if((!g)&&(!r)&&(!y)&&b&&(!o)) return pitch(4, octave);
  
  // ----O // F
  else if((!g)&&(!r)&&(!y)&&(!b)&&o) return pitch(5, octave);
  
  // GR--- // FS
  else if(g&&r&&(!y)&&(!b)&&(!o)) return pitch(6, octave);
  
  // -RY-- // G
  else if((!g)&&r&&y&&(!b)&&(!o)) return pitch(7, octave);
  
  // --YB- // GS
  else if((!g)&&(!r)&&y&&b&&(!o)) return pitch(8, octave);
  
  // ---BO // A
  else if((!g)&&(!r)&&(!y)&&b&&o) return pitch(9, octave);
  
  // G-Y-- // AS
  else if(g&&(!r)&&y&&(!b)&&(!o)) return pitch(10, octave);
  
  // GRY-- // B
  else if(g&&r&&y&&(!b)&&(!o)) return pitch(11, octave);
  
  // -R-B- // C
  else if((!g)&&r&&(!y)&&b&&(!o)) return pitch(0, octave2);
  
  // -RYB- // CS
  else if((!g)&&r&&y&&b&&(!o)) return pitch(1, octave2);
  
  // --Y-O // D
  else if((!g)&&(!r)&&y&&(!b)&&o) return pitch(2, octave2);
  
  // --YBO // DS
  else if((!g)&&(!r)&&y&&b&&o) return pitch(3, octave2);
  
  // G--B- // E
  else if(g&&(!r)&&(!y)&&b&&(!o)) return pitch(4, octave2);
  
  // GR-B- // F
  else if(g&&r&&(!y)&&b&&(!o)) return pitch(5, octave2);
  
  // G-YB- // FS
  else if(g&&(!r)&&y&&b&&(!o)) return pitch(6, octave2);
  
  // GRYB- // G
  else if(g&&r&&y&&b&&(!o)) return pitch(7, octave2);
  
  // -R--O // GS
  else if((!g)&&r&&(!y)&&(!b)&&o) return pitch(8, octave2);
  
  // -RY-O // A
  else if((!g)&&r&&y&&(!b)&&o) return pitch(9, octave2);
  
  // -R-BO // AS
  else if((!g)&&r&&(!y)&&b&&o) return pitch(10, octave2);
  
  // -RYBO // B
  else if((!g)&&r&&y&&b&&o) return pitch(11, octave2);
  
  // G---O // C
  else if(g&&(!r)&&(!y)&&(!b)&&o) return pitch(0, octave3);
  
  // GR--O // CS
  else if(g&&r&&(!y)&&(!b)&&o) return pitch(1, octave3);
  
  // G-Y-O // D
  else if(g&&(!r)&&y&&(!b)&&o) return pitch(2, octave3);
  
  // G--BO // DS
  else if(g&&(!r)&&(!y)&&b&&o) return pitch(3, octave3);
  
  // GRY-O // E
  else if(g&&r&&y&&(!b)&&o) return pitch(4, octave3);
  
  // G-YBO // F
  else if(g&&(!r)&&y&&b&&o) return pitch(5, octave3);
  
  // GR-BO // FS
  else if(g&&r&&(!y)&&b&&o) return pitch(6, octave3);
  
  // GRBYO // G
  else return pitch(7, octave3);
  
}
