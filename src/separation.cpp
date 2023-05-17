#include <Arduino.h>
#include "separation.h"

void separation::init()
{
    ppL.set_pins(32, 33, 25, 34);
    ppR.set_pins(23, 22, 21, 15);
    vertWedge.set_pins(0, 16, 17, 26);
    pinMode(PPL_LIMIT, INPUT);
    pinMode(PPR_LIMIT, INPUT);

    Serial.println("Initialising... \nMin Pos");
    Serial.println("Cap \t Left \t Right");
    vertWedge.init();
    ppL.init();
    ppR.init();
    Serial.println();
    
    //calibrate
    Serial.print("Calibrating...\t");
    vertWedge.max_pos = 4095;
    ppL.max_pos = 4095;
    ppR.max_pos = 4095;
    
    vertWedge.move(-vertWedge.max_pwr);
    ppL.move(-ppL.max_pwr);
    ppR.move(-ppR.max_pwr);
    
    delay(1000*50 / 8);
    
    vertWedge.pot_smoothing_init();
    ppL.pot_smoothing_init();
    ppR.pot_smoothing_init();

    Serial.println("Complete");

    Serial.print("Min Position | Cap: ");
    Serial.print(vertWedge.min_pos);
    Serial.print("\t ppL: ");
    Serial.print(ppL.min_pos);
    Serial.print("\t ppR: ");
    Serial.println(ppR.min_pos);  
    //end - calibrate

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
            ppR.stop();
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

void separation::pinPress_limit()
{

    bool l = digitalRead(PPL_LIMIT);
    bool r = digitalRead(PPR_LIMIT);

    ppR.move(ppR.max_pwr);
    ppL.move(ppL.max_pwr);

    while(!ppStatus_left || !ppStatus_right)
    {
        l = digitalRead(PPL_LIMIT);
        r = digitalRead(PPR_LIMIT);

        if(l)
        {
            ppL.stop();
            ppStatus_left = HIGH;
        }
        if(r)
        {
            ppR.stop();
            ppStatus_right = HIGH;
        }
        /*
        Serial.print(l);
        Serial.print("\t");
        Serial.print(r);
        Serial.print("\n");
        */

        delay(50);
    }

    Serial.println("AFTER");
    delay(500);
    ppR.move(-ppR.max_pwr);
    ppL.move(-ppL.max_pwr);

    ppStatus_left = LOW;
    ppStatus_right = LOW;

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
    
    //simple pp - needs tuning
    ppR.move(255);
    delay(450);
    l1.move(255);
    delay(2200);
    l1.move(-255);
    ppR.move(-255);
    delay(4000);
    
   /*
   //init
    l1.move(l1.max_pwr);
    delay(1000);
    l1.move(-l1.max_pwr);
    delay(2000);
    */

    /*
    //print both pot vals
    double minL = 999.0, maxL = -999.0, minR = 999.0, maxR = -999.0;
    for (int i = 0; i < 40; i++)
    {
        int x = ppL.readPos();
        if(ppL.stroke<minL)
            minL = ppL.stroke;
        else if(ppL.stroke>maxL)
            maxL = ppL.stroke;

        x = ppR.readPos();
        if(ppR.stroke<minR)
            minR = ppR.stroke;
        else if(x>maxR)
            maxR = ppR.stroke;
        print_stroke();
        delay(250);
    }
    Serial.print(minL);
    Serial.print("\t");
    Serial.print(maxL);
    Serial.print("\t");
    Serial.print(maxL-minL);
    Serial.print("\t");
    Serial.print(minR);
    Serial.print("\t");
    Serial.print(maxR);
    Serial.print("\t");
    Serial.println(maxR-minR);
    */

    /*
    //Move extra steps ahead
    int x = l1.readPos();
    double target = l1.stroke + 16.0;
    Serial.print("Target: ");
    Serial.println(target);
    l1.move(l1.max_pwr);
    Serial.println(l1.stroke);
    while ( l1.stroke  < target)
    {
      x = l1.readPos();
      Serial.println(l1.stroke);
      delay(250);
    }
    l1.stop();
    x = l1.readPos();
    print_stroke();
    delay(5000);
    l1.move(-l1.max_pwr);
    */

}