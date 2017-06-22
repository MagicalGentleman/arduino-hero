byte frets(boolean g, boolean r, boolean y, boolean b, boolean o, byte oct) {
  
  byte noteLUT[32] = {
    0x00, // ----- // C 
    0x05, // ----O // F 
    0x04, // ---B- // E 
    0x09, // ---BO // A 
    0x03, // --Y-- // DS
    0x12, // --Y-O // D 
    0x08, // --YB- // GS
    0x13, // --YBO // DS
    0x02, // -R--- // D 
    0x18, // -R--O // GS
    0x10, // -R-B- // C 
    0x1A, // -R-BO // AS
    0x07, // -RY-- // G 
    0x19, // -RY-O // A 
    0x11, // -RYB- // CS
    0x1B, // -RYBO // B 
    0x01, // G---- // CS
    0x20, // G---O // C 
    0x14, // G--B- // E 
    0x23, // G--BO // DS
    0x0A, // G-Y-- // AS
    0x22, // G-Y-O // D 
    0x16, // G-YB- // FS
    0x25, // G-YBO // F 
    0x06, // GR--- // FS
    0x21, // GR--O // CS
    0x15, // GR-B- // F 
    0x26, // GR-BO // FS
    0x0B, // GRY-- // B
    0x24, // GRY-O // E
    0x17, // GRYB- // G
    0x27  // GRYBO // G
  };
  
  int index = ((int(g)&1)<<4) | ((int(r)&1)<<3) | ((int(y)&1)<<2) | ((int(b)&1)<<1) | (int(o)&1);
  byte note = noteLUT[index];
  
  return  pitch(note&0x0F, oct + ((note&0x30)>>4));
}
