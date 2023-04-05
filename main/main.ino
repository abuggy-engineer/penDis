#include <Arduino.h>
#include "linAct.h"

linAct linAct;

int counter = 0;
int t0;
double x0;

void setup() {

  Serial.begin(9600);
  Serial.println("Hello World");
  linAct.init();

  //linAct.calibrate();

}

void loop() {

  while(Serial.available() == false){}

  float target = Serial.parseFloat();

  if(target == 999)
  {
    linAct.stop();
    Serial.println("STOP");
    return;
  }
  else if(target == 222)
    linAct.move(-255);
  else if(target == 333)
    linAct.move(255);
  else if(target == 444)
    linAct.go2pos(0);
  else if(target != 0)
    linAct.go2pos(target);
}
