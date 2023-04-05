#include <Arduino.h>
#include "linAct.h"

linAct pinPress;
linAct pinPress_left;
linAct pinPress_right;

int counter = 0;

double left_target, right_target;

void setup() {

  Serial.begin(9600);
  Serial.println("Hello World");
  
  pinPress_left.set_pins(6, 7, 8, A0);
  pinPress_left.init();
  
  pinPress_right.set_pins(13, 12, 11, A1);
  pinPress_right.init();

  pinPress_left.max_pos = 675;
  pinPress_left.min_pos = 355;
  pinPress_right.max_pos = 676;
  pinPress_right.min_pos = 352;
  //pinPress_right.calibrate();

}

void loop() 
{

  Serial.println("Target Left Pos: ");
  while(!Serial.available()){}
  double response_left = Serial.parseInt();
  Serial.println(response_left);

  Serial.println("Target Right Pos: ");
  while(!Serial.available()){}
  double response_righ = Serial.parseInt();
  Serial.println(response_righ);

  pinPress.dualControl(pinPress_left, pinPress_right, response_left, response_righ);
  
}
