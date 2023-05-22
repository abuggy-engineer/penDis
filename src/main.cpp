#include <Arduino.h>
#include "acs712.h"
#include "separation.h"

#define TRIGGER_PP 12
#define TRIGGER_CAP 13

separation sep;

void setup() {

  Serial.begin(115200);
  Serial.println("Hello World");

  sep.init();

  pinMode(TRIGGER_PP, INPUT);
  pinMode(TRIGGER_CAP, INPUT);
  //attachInterrupt(digitalPinToInterrupt(TRIGGER_PP), sep.pinPress_limit, RISING);
  //attachInterrupt(digitalPinToInterrupt(TRIGGER_CAP), sep.capRemoval, RISING);
}

void loop() 
{

  bool pp = digitalRead(TRIGGER_PP);
  bool wedge = digitalRead(TRIGGER_CAP);

  Serial.print(pp);
  Serial.print("\t");
  Serial.print(wedge);
  Serial.print("\t");

  delay(50);

  
  if(pp)
  {
    sep.pinPress_limit();
    Serial.println("PP");
  }
  else if (wedge)
  {
    sep.capRemoval();
    Serial.println("cap");
  }
  Serial.print("\n");

  /*
  Serial.print("Target Left Pos: ");

  while(!Serial.available()){}
  double response_left = Serial.parseInt();
  //Serial.println(response_left);

  if(response_left == 777)
  {
    sep.capRemoval();
  }
  else if(response_left == 888)
  {
    //sep.pinPress();
    //sep.test_act(sep.ppL);
    sep.pinPress_limit();
  }  
  */
}
