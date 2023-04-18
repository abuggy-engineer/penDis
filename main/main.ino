#include <Arduino.h>
#include "linAct.h"

linAct pinPress;
linAct pinPress_left;
linAct pinPress_right;
linAct capRem;

int counter = 0;

double left_target, right_target;

void setup() {

  Serial.begin(9600);
  Serial.println("Hello World");
  
  pinPress_left.set_pins(6, 7, 8, A0);
  
  pinPress_right.set_pins(13, 12, 11, A1);

  capRem.set_pins(A3, 6, 5, A2);

  pinPress_left.max_pos = 675;
  pinPress_left.min_pos = 355;
  pinPress_right.max_pos = 676;
  pinPress_right.min_pos = 352;
  pinPress_right.max_pos = 675;
  pinPress_right.min_pos = 348;
  capRem.max_pos = 674;
  capRem.min_pos = 349;

  //pinPress_left.init();
  //pinPress_right.init();
  capRem.init();

  //capRem.calibrate();
  capRem.move(-255);
}

void loop() 
{
  capRem.pot_smoothing();
  
  Serial.print("Target Left Pos: ");
  while(!Serial.available()){}
  double response_left = Serial.parseInt();
  Serial.println(response_left);

  if(response_left == 555)
  {
    capRem.pot_smoothing_init();
  }
  else
  {
    capRem.go2pos(response_left);    
  } 
}
