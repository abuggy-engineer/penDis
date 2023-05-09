#include <Arduino.h>
#include "separation.h"

void separation::init()
{
    //ppL.set_pins(32, 33, 25, 34);
    //ppR.set_pins(23, 22, 21, 15);
    vertWedge.set_pins(0, 16, 17, 26);

    vertWedge.init();
    //ppL.init();
    //ppR.init();
    
    /*
    //calibrate
    vertWedge.max_pos = 4095;
    ppL.max_pos = 4095;
    ppR.max_pos = 4095;

    vertWedge.move(-vertWedge.max_pwr);
    ppL.move(-ppL.max_pwr);
    ppR.move(-ppR.max_pwr);
    
    delay(1000*100 / 8);

    vertWedge.pot_smoothing_init();
    ppL.pot_smoothing_init();
    ppR.pot_smoothing_init();


    Serial.print("Min Position | Cap: ");
    Serial.print(vertWedge.min_pos);
    Serial.print("\t ppL: ");
    Serial.print(ppL.min_pos);
    Serial.print("\t ppR: ");
    Serial.println(ppR.min_pos);
    //end - calibrate
    */
   vertWedge.calibrate();

    vertWedge.pot_smoothing();
    ppL.pot_smoothing();
    ppR.pot_smoothing();

    vertWedge.go2pos(30);
}

void separation::capRemoval()
{
    vertWedge.move(255);
    int x = vertWedge.readPos();
    Serial.println(vertWedge.stroke);
    while (vertWedge.stroke < 72)
    {
      x = vertWedge.readPos();
      Serial.println(vertWedge.stroke);
      //cap.readCurr();
    }
    Serial.println("**********************************************");
    vertWedge.move(-255);
    while (vertWedge.stroke > 30)
    {
      x = vertWedge.readPos();
      Serial.println(vertWedge.stroke);
      //cap.readCurr();
    }
    vertWedge.stop();
}
/*
void separation::pinPress()
{
    double target_dist = 5.00; //mm

    ppL.move(ppL.max_pwr);
    ppR.move(ppR.max_pwr);

    int x = ppL.readPos();
    int y = ppR.readPos();

    bool ppL_status = false;
    bool ppR_status = false;

    while(!ppL_status || !ppR_status)
    {
        if(ppL.stroke < target_dist)
        {
            x = ppL.readPos();
            Serial.print(ppL.stroke);
            Serial.print("\t");
        }
        else
        {
            ppL.stop();
            ppL_status = true;
            Serial.print(ppL.stroke);
            Serial.print("***\t");
        }

        if(ppR.stroke < target_dist)
        {
            y = ppR.readPos();
            Serial.print(ppR.stroke);
            Serial.print("\n");
        }
        else 
        {
            ppL.stop();
            ppR_status = true;
            Serial.print("***");
            Serial.print("\n");
        }
    }
    Serial.println("Out of pp loop");

    delay(2000);

    ppL.move(-ppL.max_pwr);
    ppR.move(-ppR.max_pwr);

}

void separation::print_stroke()
{
    Serial.print("Cap: ");
    Serial.print(vertWedge.stroke);
    Serial.print("\t ppL: ");
    Serial.print(ppL.stroke);
    Serial.print("\t ppR: ");
    Serial.println(ppR.stroke);
}

void separation::test_act(linAct l1)
{
    l1.move(l1.max_pwr);
    int x = l1.readPos();
    while (l1.stroke < 15)
    {
      x = l1.readPos();
      Serial.println(l1.stroke);
      //cap.readCurr();
    }
    l1.stop();
    print_stroke();
    delay(5000);
    l1.move(-l1.max_pwr);

}
*/