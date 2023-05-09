#include <Arduino.h>
#include "linAct.h"

void linAct::init()
{
  
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(POT, INPUT);

  max_pwr = 255; // 8-bit
  max_stroke = 200; // mm

  pot_smoothing_init();
  //pos = readPos();

  Serial.print("Initial Position: ");
  Serial.println(stroke);
}

void linAct::extend(float pwr)
{
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);

  analogWrite(ENA_PIN, pwr);
}

void linAct::retract(float pwr)
{
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);

  analogWrite(ENA_PIN, pwr);

  //Serial.println("Retract");
}

void linAct::stop()
{
  //analogWrite(ENA_PIN, 0);

  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);


  //Serial.println("Stop");
}

void linAct::move(float pwr)
{

  if(pwr > 0)
  {
    if(pwr > abs(max_pwr))
      pwr = max_pwr;

    extend(pwr);
  }

  else if(pwr < 0)
  {
    if(pwr > abs(max_pwr))
      pwr = -max_pwr;

    retract(abs(pwr));
  }

  else
    stop();

}

unsigned int linAct::readPos()
{
  pos = analogRead(POT);
  stroke = pos2stroke(pos);

  return pos;
}

void linAct::calibrate()
{
  max_pos = 4095;

  move(-255);
  delay(1000*100 / 8);
  pot_smoothing_init();
  Serial.print("Min Position: ");
  Serial.println(min_pos);

}

double linAct::pos2stroke(int position)
{ 
  stroke = max_stroke*(position - min_pos)/(max_pos - min_pos);
  //Serial.println(stroke);  
  return stroke;
}

int linAct::stroke2pos(double dist)
{
  pos = ( (dist - 0)*(max_pos - min_pos)/(max_stroke - 0) ) + min_pos;
  //Serial.println(pos);  
  return pos;
}

void linAct::go2pos(float target)
{
  if(target > 200)
    target = 200;
  else if(target < 0)
    target = 0;
    
  int target_pos = stroke2pos(target);

  //pos = readPos();
  pot_smoothing();

  int error = target_pos - pos_av;//stroke;//pos;

  int tolerance = 10;

  while(abs(error) >= tolerance)
  {
    //Serial.print("error:");
    //Serial.print(error);
    //Serial.print(",");//////////////////////
    if(error > 0)
      move(max_pwr);
    else if(error < 0)
      move(-max_pwr);

    pos = readPos();//pot_smoothing();//pos = readPos();
    error = target_pos - pos;//pos_av;//stroke;//pos;

    /*
    Serial.print("Target:");///////////
    Serial.print(target_pos);
    Serial.print(",");
    Serial.print("Pos:");
    Serial.println(pos);
    */
  
  }
  stop();

  /////////////////////////////////////////////////////////////////
  //pot_smoothing_init();
  //Serial.print("************************************Pos av: ");
  //Serial.println(pos_av);
  pos = pos_av;
  /*
  while(pos_av != target_pos)
  {
    error = target_pos - pos;

    while(abs(error) >= tolerance)
    {
      Serial.print("error:");
      Serial.print(error);
      Serial.print(",");//////////////////////
      if(error > 0)
        move(0.5*max_pwr);
      else if(error < 0)
        move(-0.5*max_pwr);

      pos = readPos();//pot_smoothing();//pos = readPos();
      error = target_pos - pos;//pos_av;//stroke;//pos;

      Serial.print("Target:");///////////
      Serial.print(target_pos);
      Serial.print(",");
      Serial.print("Pos:");
      Serial.println(pos);    
    }
    stop();
    pot_smoothing_init();
    Serial.print("************************************Pos av: ");
    Serial.println(pos_av);
    pos = pos_av;
  }

    */
  /////////////////////////////////////////////////////////////////
  Serial.print("Target:");
  Serial.print(target_pos);
  Serial.print(",");
  Serial.print(target);
  Serial.print(" -- Actual:");
  Serial.print(pos_av);
  Serial.print(",");
  Serial.println(stroke);
}

/*
void linAct::go2pos(float target)
{ 
  float tolerance = 0.32;
  
  int x = readPos();
  double curr = pos2stroke(x);

  while( (target <= curr - tolerance || target >= curr + tolerance) )
  {

    //
  	float error = target - curr;
    if(error > 5)
      move(max_pwr);
    else if (error > 0)
      move(max_pwr*0.5);
    else if (error < -5)
      move(-max_pwr);
    else if (error < 0)
      move(-max_pwr*0.5);

    //    
    x = readPos(); 
    curr = pos2stroke(x);    
    
    Serial.print("Target:");
    Serial.print(stroke2pos(target));
    Serial.print(",");
    Serial.print("Actual:");
    Serial.println(stroke2pos(curr));

    //calcVel();

  }

  stop();

  Serial.print("Target:");
  Serial.print(target);
  Serial.print(",");
  Serial.print("Actual:");
  Serial.println(curr);
      
}
*/

void linAct::set_pins(int ena, int in1, int in2, int pot)
{
  ENA_PIN = ena; // Speed/Power to Motor
  IN1_PIN = in1; // Polarity of Voltage ==> Direction (Fwd)
  IN2_PIN = in2; // Polarity of Voltage ==> Direction (Rev)
  POT = pot; // the Arduino pin connected to the potentiometer of the actuator
}

void linAct::dualControl(linAct l1, linAct l2, double left, double right)
{

  int tolerance = 1;

  if(left == 222 || right == 222)
  {
    l1.move(-l1.max_pwr);
    l2.move(-l2.max_pwr);
    return;
  }
  else if(left == 333 || right == 333)
  {
    l1.move(l1.max_pwr);
    l2.move(l2.max_pwr);
    return;
  }
  else if(left == 555 || right == 555)
  {
    Serial.print("L ");
    l1.printPos();
    Serial.print("R ");
    l2.printPos();
    return;
  }

  //Limit Target Values
  if(left > 200)
    left = 200;
  else if(left < 0)
    left = 0;

  if(right > 200)
    right = 200;
  else if(right < 0)
    right = 0;
  //Convert target to position
  int left_pos = l1.stroke2pos(left);
  int righ_pos = l1.stroke2pos(right);

  Serial.print(left_pos);
  Serial.print(" Target ");
  Serial.println(righ_pos);

  //Read Position
  l1.pos = l1.readPos();
  l2.pos = l2.readPos();

  //Error
  int e_left = left_pos - l1.pos;
  int e_righ = righ_pos - l2.pos;

  Serial.println("Left \tRight");

  while(abs(e_left) > tolerance || abs(e_righ) > tolerance)
  {
    if(abs(e_left) < tolerance)
      l1.stop();
    else if(e_left > 0)
    {
      l1.move(l1.max_pwr);
      l1.pos = l1.readPos();
    }
    else if(e_left < 0)
    {
      l1.move(-l1.max_pwr);
      l1.pos = l1.readPos();
    }

    if(abs(e_righ) < tolerance)
      l2.stop();
    else if(e_righ > 0)
    {
      l2.move(l2.max_pwr);
      l2.pos = l2.readPos();
    }
    else if(e_righ < 0)
    {
      l2.move(-l2.max_pwr);
      l2.pos = l2.readPos();
    }

    e_left = left_pos - l1.pos;
    e_righ = righ_pos - l2.pos;

    Serial.print(l1.pos);
    Serial.print("\t");
    Serial.println(l2.pos);
  }

  l1.stop();
  l2.stop();

  Serial.print("L ");
  l1.printPos();
  Serial.print("R ");
  l2.printPos();

}

void linAct::printPos()
{
  pos = readPos();
  stroke = pos2stroke(pos);

  Serial.print("Position: ");
  Serial.print(pos);
  Serial.print(" Stroke: ");
  Serial.println(stroke);
}

void linAct::pot_smoothing()
{
  int sum = 0;

  //moving the values on by 1, making space for new value, getting all but new val summed
  for(int i = 8; i >= 0; i--)
  {
    pot_vals[i+1] = pot_vals[i];

    if(i <= 8)
      sum += pot_vals[i+1];
  }

  //record new value, add to sum
  pot_vals[0] = readPos();
  sum += pot_vals[0];

  pos_av = sum/10;

  //print data
  /*
  for(int i = 0; i < 10; i++)
  {
    Serial.print(pot_vals[i]);
    Serial.print(" ");
  }
  Serial.print("--> ");
  Serial.println(pos_av);
  */
}

void linAct::pot_smoothing_init()
{
  int sum = 0;
  for(int i = 99; i >= 0; i--)
  {
    pot_vals[i] = readPos();
    sum += pot_vals[i];
    //Serial.println(pot_vals[i]);
  }
  pos_av = sum/100;
  //Serial.println(pos_av);

  min_pos = pos_av;
}

