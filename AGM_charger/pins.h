#include "includes.h"

#define ANALOG_PIN A0 // voltage read pin
#define REL_MAX_PIN = D8;
#define REL_MID_PIN = D7;
#define REL_LOW_PIN = D6;
#define ACC_IN_PIN = D5;
#define CHARGE_PUMPIN_PIN = D4;

uint8_t CHARGE;
uint8_t CCV_CHARGE;

float SOC_voltage;
float CCV_voltage;

void pinsSetup()
{
    analogReference(DEFAULT);

    pinMode(REL_MAX_PIN, OUTPUT);
    pinMode(REL_MID_PIN, OUTPUT);
    pinMode(REL_LOW_PIN, OUTPUT);
    
    digitalWrite(REL_MAX_PIN, LOW);
    digitalWrite(REL_MID_PIN, LOW);
    digitalWrite(REL_LOW_PIN, LOW);

    pinMode(ACC_IN_PIN, INPUT); // pin input to indicate when AC mains are availible
}