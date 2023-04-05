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

  error_prev = 0;
  integral_pos = 0;
  kp = 1.25;//0.9;
  ki = 0.0;
  kd = 0;//0.02;

  max_pos = 676;
  min_pos = 357;

  pos = readPos();
  stroke = pos2stroke(pos);
  Serial.print("Initial Position: ");
  Serial.println(stroke);
}

void linAct::extend(float pwr)
{
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);

  analogWrite(ENA_PIN, pwr);

  //Serial.println("Extend");
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

  return pos;
}

void linAct::calibrate()
{
  move(max_pwr);
  Serial.print("Extending - ");
  int counter = 0;
  int prev = 9999;
  int curr = readPos();

  while (curr != prev || counter < 20)
  {
    //delay(1000);
    prev = curr;
    curr = readPos();
    Serial.print("Counter: ");
    Serial.print(counter);
    Serial.print(" Pos: ");
    Serial.println(curr);
    if (curr == prev)
    {
      counter++;
    }
    else
      counter = 0;
  }
  max_pos = readPos();
  Serial.print("Max Pos: ");
  Serial.println(max_pos);
  Serial.print("Stroke: ");
  Serial.println(pos2stroke(max_pos));

  stop();
  delay(1000);

  move(-max_pwr);
  delay(50);
  Serial.print("Retracting- ");
  counter = 0;
  prev = 9999;
  while (curr != prev || counter < 20)
  {
    //delay(1000);
    prev = curr;
    curr = readPos();
    Serial.print("Counter: ");
    Serial.print(counter);
    Serial.print(" Pos: ");
    Serial.println(curr);
    if (curr == prev)
    {
      counter++;
    }
    else 
      counter = 0;
  }
  min_pos = readPos();
  Serial.print("Min Pos: ");
  Serial.println(min_pos);
  Serial.print("Stroke: ");
  Serial.println(pos2stroke(min_pos));

  stop();

}

double linAct::pos2stroke(int position)
{

  //stroke = map(position, min_pos, max_pos, 0, max_stroke);
  	
  double ratio = (position - min_pos)/(max_pos - min_pos);
  
  stroke = max_stroke*(position - min_pos)/(max_pos - min_pos);

  return stroke;
}

int linAct::stroke2pos(double dist)
{
  pos = map(dist, 0, max_stroke, min_pos, max_pos);

  return pos;
}

void linAct::go2pos(float target)
{
  if(target > 200)
    target = 200;
  else if(target < 0)
    target = 0;
    
  int target_pos = stroke2pos(target);

  pos = readPos();

  int error = target_pos - pos;

  int tolerance = 1;

  while(abs(error) >= tolerance)
  {
    Serial.print("error:");
    Serial.print(error);
    Serial.print(",");
    if(error > 0)
    {
      move(max_pwr);
      Serial.print("-FWD-");
    }
    else if(error < 0)
      move(-max_pwr);

    pos = readPos();
    error = target_pos - pos;

    Serial.print("Target:");
    Serial.print(target_pos);
    Serial.print(",");
    Serial.print("Pos:");
    Serial.println(pos);
  }
  stop();

  Serial.print("Target:");
  Serial.print(target_pos);
  Serial.print(",");
  Serial.print(target);
  Serial.print(" -- Actual:");
  Serial.print(pos);
  Serial.print(",");
  Serial.println(pos2stroke(pos));
}

/*
void linAct::go2pos(float target)
{ 
  float tolerance = 0.32;
  float output_signal = 0;

  int x = readPos();
  double curr = pos2stroke(x);

  while( (target <= curr - tolerance || target >= curr + tolerance) || abs(output_signal) > 80)
  {
  /*
    float error = target - curr;
    float dedt = error - error_prev;
    integral_pos += error; 
    output_signal = kp*error + ki*integral_pos + kd*dedt;

    if(abs(output_signal) < 0.3*max_pwr)
    {
      if(output_signal > 0)
        output_signal = 0.3*max_pwr;
      else
        output_signal = -0.3*max_pwr;
    }

    move(output_signal);

    error_prev = error;
  

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

/*
void linAct::calcVel() 
{

  if(!t0)
  {
    t0 = millis();
    x0 = stroke;
  }
  else
  {
    t1 = millis();
    x1 = stroke;

    int dt = t1 - t0; //ms

    Serial.print("x0: ");
    Serial.print(x0);
    Serial.print(" x1: ");
    Serial.print(x1);
    Serial.print(" dt: ");
    Serial.print(dt);

    if(x1 != x0)
    {
      float dx = x1 - x0; //mm
      vel = 1000*dx/dt; // mm/s
     
      Serial.print(" dx: ");
      Serial.print(dx);
      Serial.print(" vel: ");
      Serial.println(vel);

      t0 = t1;
      x0 = x1;
    }
    else
      Serial.println();
  }
}
*/


void linAct::calcVel() 
{
  for(int i = 9; i > 0; i--)
  {
    timeArray[i] = timeArray[i-1];
    positionArray[i] = positionArray[i-1];
  }

  timeArray[0] = millis();
  positionArray[0] = stroke;

  double dx = positionArray[0] - positionArray[9];
  unsigned int dt = timeArray[0] - timeArray[9];

  vel = 1000*dx/dt;

  Serial.print(" dt: ");
  Serial.print(dt);
  Serial.print(" dx: ");
  Serial.print(dx);
  Serial.print(" vel: ");
  Serial.println(vel);

}

double linAct::average(double array[], int size)
{
  double sum = 0;
  for(int i = 0; i < size; i++)
  {
    sum += array[i];
  }

  return sum/size;
}

