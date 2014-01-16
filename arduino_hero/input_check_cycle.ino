void checkInput1() {
  if(digitalRead(green)==LOW) g=g1=true;
  else g=g1=false;
  if(digitalRead(red)==LOW) r=r1=true;
  else r=r1=false;
  if(digitalRead(yellow)==LOW) y=y1=true;
  else y=y1=false;
  if(digitalRead(blue)==LOW) b=b1=true;
  else b=b1=false;
  if(digitalRead(orange)==LOW) o=o1=true;
  else o=o1=false;
  
  if(digitalRead(select)==LOW) u=u1=true;
  else u=u1=false;
  if(digitalRead(start)==LOW) d=d1=true;
  else d=d1=false;
  
  if(digitalRead(strumD)==LOW) {
    s=s1=true;
    sD=false;
  }
  //else if(digitalRead(strumU)==LOW) sD=s=s1=true;
  else s=s1=false;
  return;
}

void checkInput2() {
  if(digitalRead(green)==LOW) g=g2=true;
  else g=g2=false;
  if(digitalRead(red)==LOW) r=r2=true;
  else r=r2=false;
  if(digitalRead(yellow)==LOW) y=y2=true;
  else y=y2=false;
  if(digitalRead(blue)==LOW) b=b2=true;
  else b=b2=false;
  if(digitalRead(orange)==LOW) o=o2=true;
  else o=o2=false;
  
  if(digitalRead(select)==LOW) u=u2=true;
  else u=u2=false;
  if(digitalRead(start)==LOW) d=d2=true;
  else d=d2=false;
  
  if(digitalRead(strumD)==LOW) {
    s=s2=true;
    sD=false;
  }
  //else if(digitalRead(strumU)==LOW) sD=s=s2=true;
  else s=s2=false;
  return;
}
