#include <Arduino.h>
#include "acs712.h"

void acs712::set_pin(int x)
{
    pin = x;
    pinMode(pin, INPUT);
    Serial.print("pin: ");
    Serial.println(pin);

    return;
}

void acs712::readCurr()
{
    int adc = analogRead(pin);
    float voltage = adc * 5 / 4095; 
    curr = (voltage - acs_offset) / sensitivity;
    Serial.print("ADC: ");
    Serial.print(adc);
    Serial.print("\t");
    Serial.print("Voltage(V): ");
    Serial.print(voltage);
    Serial.print("\t");
    Serial.print("Current(A): ");
    Serial.println(curr);
    //printCurr();
}

void acs712::printCurr()
{
    Serial.print("Current(A): ");
    Serial.println(curr);
}