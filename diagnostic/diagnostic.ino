const int green=2;
const int red=3;
const int yelLOW=4;
const int blue=5;
const int orange=6;
const int start=7;
const int select=8;
const int strumD=9;
const int strumU=10;
const int whammy=A0;

int whammyValue=0;

boolean g=false,r=false,y=false,b=false,o=false,sta=false,sel=false,strU=false,strD=false;

void setup() {
  Serial.begin(9600);
  pinMode(green, INPUT);
  pinMode(red, INPUT);
  pinMode(yelLOW, INPUT);
  pinMode(blue, INPUT);
  pinMode(orange, INPUT);
  pinMode(start, INPUT);
  pinMode(select, INPUT);
  pinMode(strumU, INPUT);
  pinMode(strumD, INPUT);
}

void loop() {
  if(digitalRead(green)==LOW && !g) {
    Serial.print("GREEN OK\n");
    //g=true;
  }
  if(digitalRead(red)==LOW && !r) {
    Serial.print("RED OK\n");
    //r=true;
  }
  if(digitalRead(yelLOW)==LOW && !y) {
    Serial.print("YELLOW OK\n");
    //y=true;
  }
  if(digitalRead(blue)==LOW && !b) {
    Serial.print("BLUE OK\n");
    //b=true;
  }
  if(digitalRead(orange)==LOW && !o) {
    Serial.print("ORANGE OK\n");
    //o=true;
  }
  if(digitalRead(start)==LOW && !sta) {
    Serial.print("START OK\n");
    //sta=true;
  }
  if(digitalRead(select)==LOW && !sel) {
    Serial.print("SELECT OK\n");
    //sel=true;
  }
  if(digitalRead(strumU)==LOW && !strU) {
    Serial.print("STRUM UP OK\n");
    //strU=true;
  }
  if(digitalRead(strumD)==LOW && !strD) {
    Serial.print("STRUM DOWN OK\n");
    //strD=true;
  }
}
