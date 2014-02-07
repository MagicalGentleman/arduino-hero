void checkInput() {
  int x;
  if(tick>0) x=1;
  else x=2;
  if(digitalRead(green)==LOW) g[0]=g[x]=true;
  else g[0]=g[x]=false;
  if(digitalRead(red)==LOW) r[0]=r[x]=true;
  else r[0]=r[x]=false;
  if(digitalRead(yellow)==LOW) y[0]=y[x]=true;
  else y[0]=y[x]=false;
  if(digitalRead(blue)==LOW) b[0]=b[x]=true;
  else b[0]=b[x]=false;
  if(digitalRead(orange)==LOW) o[0]=o[x]=true;
  else o[0]=o[x]=false;
  
  if(digitalRead(select)==LOW) u[0]=u[x]=true;
  else u[0]=u[x]=false;
  if(digitalRead(start)==LOW) d[0]=d[x]=true;
  else d[0]=d[x]=false;
  
  if(digitalRead(strumD)==LOW) {
    s[0]=s[x]=true;
    sD=false;
  }
  else if(digitalRead(strumU)==LOW) sD=s[0]=s[x]=true;
  else s[0]=s[x]=false;
  return;
}
