#include <Arduino.h>
#include "acs712.h"
#include "separation.h"

const uint8_t TRIGGER_PP = 18;//12;
const uint8_t TRIGGER_CAP = 4;//5//19;//13;

//interrupts
void pp_trigger();
void vw_trigger();
volatile uint16_t switchCount = 0;

separation sep;
linAct test;

unsigned int pp_count = 0;
unsigned int vw_count = 0;

void setup() 
{

  Serial.begin(115200);
  Serial.println("Hello World");

  sep.init();

  pinMode(TRIGGER_PP, INPUT);
  pinMode(TRIGGER_CAP, INPUT);
  //attachInterrupt(digitalPinToInterrupt(TRIGGER_PP), pp_trigger, RISING);
  //attachInterrupt(digitalPinToInterrupt(TRIGGER_CAP), vw_trigger, RISING);

  //test
  pinMode(35, INPUT);
}

bool pp = false;
bool wedge = false;

void loop() 
{
  //sep.test_act(sep.ppR);

  //interrupt version
  /*
  if(pp)
  {
    Serial.println("PP");
    //sep.pinPress_limit();
    pp = false;
  }
  else if (wedge)
  {
    Serial.println("cap");
    //sep.capRemoval();
    wedge = false;
  }
  */

  bool pp = digitalRead(TRIGGER_PP);
  unsigned long wedge = analogRead(TRIGGER_CAP);

  
  //Serial.print(analogRead(TRIGGER_CAP));
  //Serial.print("\t");
  Serial.print(wedge);//digitalRead(TRIGGER_CAP));
  Serial.print("\n");
  

  delay(50);

  //check button signals
  if(pp)
  {
    Serial.print("PP");
    //five successive signals
    while(pp && pp_count < 5)
    {
      pp_count++;
      pp = digitalRead(TRIGGER_PP);
      Serial.print(pp_count);
      delay(50);
    }
    if(pp_count >= 5)
    {
      sep.pinPress_limit();
      Serial.println("\t PP");
      pp_count = 0;
    }
    else
    {
      pp_count = 0; //resetting if not long enough
      Serial.println("exit");
    }
  }
  
  if(wedge > 4095/2)
  {
    Serial.print("VW");
    //five successive signals
    while(wedge > 4095/2 && vw_count < 5)
    {
      vw_count++;
      wedge = analogRead(TRIGGER_CAP);
      Serial.print(vw_count);
      delay(50);
    }
    if(vw_count >= 5)
    {
      Serial.println("\t VW");
      sep.capRemoval();
      vw_count = 0;
    }
    else
    {
      vw_count = 0; //resetting if not long enough
      Serial.println("exit");
    }
  }


}

//UNUSED ISRs
void pp_trigger()
{
  switchCount++;
  pp = true;
}

void vw_trigger()
{
  switchCount++;
  wedge = true;
}
