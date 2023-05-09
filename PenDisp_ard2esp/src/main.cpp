#include <Arduino.h>
#include "acs712.h"
#include "separation.h"

//linAct pinPress;
//linAct pinPress_left;
//linAct pinPress_right;
//linAct capRem;
separation sep;
acs712 cap;

int counter = 0;

double left_target, right_target;

void setup() {

  Serial.begin(115200);
  Serial.println("Hello World");
  
  //pinPress_left.set_pins(6, 7, 8, A0);
  
  //pinPress_right.set_pins(13, 12, 11, A1);

  //capRem.set_pins(0, 16, 17, 34);
  cap.set_pin(35);

  /*pinPress_left.max_pos = 675;
  pinPress_left.min_pos = 355;
  pinPress_right.max_pos = 676;
  pinPress_right.min_pos = 352;
  pinPress_right.max_pos = 675;
  pinPress_right.min_pos = 348;*/
  //capRem.max_pos = 4095;
  //capRem.min_pos = 2075;

  //pinPress_left.init();
  //pinPress_right.init();
  //capRem.init();
  //capRemm.init();
  sep.init();
}

void loop() 
{

  
  Serial.print("Target Left Pos: ");

  while(!Serial.available()){}
  double response_left = Serial.parseInt();
  Serial.println(response_left);

  if(response_left == 777)
  {
    sep.capRemoval();
  }
  else if(response_left == 888)
  {
    //sep.pinPress();
  }
  
}
