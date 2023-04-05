#include <Arduino.h>
#include "linAct.h"

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

}

void loop() {

  //Appreciate Left pin press initial stroke 28.5mm, right is 22.5 ~6mm diff

  Serial.println("1. Dual Control \n2. Individual Control (6mm diff)");
  while(!Serial.available()){}
  int response = Serial.parseInt();
  Serial.println(response);

  switch(response)
  {
    case 1:

      Serial.println("Enter Target Position Pin Press Left: ");
      Serial.println("222 = back \n333 = fwd \n444 = stop \n555 = print pos");
      while(!Serial.available()){}
  	  left_target = Serial.parseFloat();
      Serial.println(left_target);

      if(left_target == 222)
      {
        pinPress_left.move(-255);
        pinPress_right.move(-255);
      }
      else if(left_target == 333)
      {
        pinPress_left.move(255);
        pinPress_right.move(255);
      }
      else if(left_target == 444)
      {
        pinPress_left.stop();
        pinPress_right.stop();
      }
      else if(left_target == 555)
      {
        int l = pinPress_left.readPos();
        l = pinPress_left.pos2stroke(l);
        Serial.print("Left: ");
        Serial.print(l);
        
        int r = pinPress_left.readPos();
        r = pinPress_left.pos2stroke(r);
        Serial.print(" Right: ");
        Serial.println(r);
      }
      else
      {
        pinPress_left.go2pos(left_target);
        pinPress_right.go2pos(left_target + 6);
      }

    break;

    case 2:
      
      Serial.println("Enter Target Position Pin Press Left: ");
      while(!Serial.available()){}
  	  left_target = Serial.parseFloat();
      Serial.println(left_target);

      Serial.println("Enter Target Position Pin Press Right: ");
      while(!Serial.available()){}
  	  right_target = Serial.parseFloat();
      Serial.println(right_target);

      pinPress_left.go2pos(left_target);
      pinPress_right.go2pos(right_target);

    break;
    
    default:
      Serial.println("*******Incorrect Input*******");
  }
}
