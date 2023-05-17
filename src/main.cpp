#include <Arduino.h>
#include "acs712.h"
#include "separation.h"

separation sep;

void setup() {

  Serial.begin(115200);
  Serial.println("Hello World");

  sep.init();
}

void loop() 
{

  
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
}
