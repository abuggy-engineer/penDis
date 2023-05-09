#include <Arduino.h>
#include "linAct.h"
#include "acs712.h"

linAct pinPress;
linAct pinPress_left;
linAct pinPress_right;
linAct capRem;
acs712 cap;

int counter = 0;

double left_target, right_target;

void setup() {

  Serial.begin(9600);
  Serial.println("Hello World");
  
  //pinPress_left.set_pins(6, 7, 8, A0);
  
  //pinPress_right.set_pins(13, 12, 11, A1);

  capRem.set_pins(0, 16, 17, 34);
  cap.set_pin(35);

  pinPress_left.max_pos = 675;
  pinPress_left.min_pos = 355;
  pinPress_right.max_pos = 676;
  pinPress_right.min_pos = 352;
  pinPress_right.max_pos = 675;
  pinPress_right.min_pos = 348;
  capRem.max_pos = 4095;
  capRem.min_pos = 2075;

  //pinPress_left.init();
  //pinPress_right.init();
  capRem.init();

  //capRem.calibrate();
  capRem.move(-255);
  delay(1000*100 / 8);
  capRem.pot_smoothing_init();
  Serial.print("Min Position: ");
  Serial.println(capRem.min_pos);
}

void loop() 
{
  
  capRem.pot_smoothing();

  capRem.go2pos(30);
  
  Serial.print("Target Left Pos: ");

  while(!Serial.available()){}
  double response_left = Serial.parseInt();
  Serial.println(response_left);

  if(response_left == 777)
  {
    capRem.move(255);
    int x = capRem.readPos();
    while (capRem.stroke < 72)
    {
      x = capRem.readPos();
      Serial.println(capRem.stroke);
      //cap.readCurr();
    }
    Serial.println("**********************************************");
    capRem.move(-255);
    while (capRem.stroke > 30)
    {
      x = capRem.readPos();
      Serial.println(capRem.stroke);
      //cap.readCurr();
    }
    capRem.stop();
  }

}
