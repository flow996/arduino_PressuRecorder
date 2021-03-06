/*

Code for ISD1820 Voice Recording Module
by eLab Peers (C) 2014.

Visit us at:
http://www.elabpeers.com

All rights reserved.

Wiring:
1.  VCC to 5V on Arduino board
2.  GND to GND on Arduino board
3.  REC to Pin 11 on Arduino board
4.  P-E to Pin 13 on Arduino board

*/

int Rec = 11;
int Play = 13;

void setup()
{ 
  pinMode(Rec, OUTPUT);
  pinMode(Play, OUTPUT);
}

void loop()
{
  digitalWrite(Rec, HIGH);
  delay(10000);
  digitalWrite(Rec, LOW);
  delay(5000);
  digitalWrite(Play, HIGH);
  delay(100);
  digitalWrite(Play, LOW);
  delay(10000);
}  
