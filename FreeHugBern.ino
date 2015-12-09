// TmDisplayCount.ino

#include <Serial>

#define COUNT

// ---------------- TM
#include <TM1640.h>
#include <TmDisplayCount.h>

TM1640 left(2,3);
TM1640 right(4,5);

TmCounter cnter(left,right);

// ---------------- BT

#include <AltSoftSerial.h>
#include <Streaming.h>

#define S_BAUDRATE    9600 // serial baudrate
#define AS_BAUDRATE   9600 // altserial baudrate
#define nl '\n'

AltSoftSerial altSerial; //On Arduino Uno and Lilypad : TX = pin 9 | RX = pin 8

// ---------------- PS

#include "HuggiPressureSensor.h"
#define DURATION_THRESHOLD 1000 // in ms

int pt_inputs[] = {A0, A5}; 
HuggiPressureSensor sensor(pt_inputs, 2);

// ==========================

void setup() {
    Serial.begin(S_BAUDRATE);
    altSerial.begin(AS_BAUDRATE);
    sensor.calibrate();
    Serial << "start" << nl;
}


void loop() {

    if(sensor.isPressed()){

        Serial << "Start ";
        long start = millis();

        while(sensor.isPressed()){ 
            delay(40);
            Serial << ".";
            Serial.flush(); 
        }

        long duration = millis() - start;

        if(duration < DURATION_THRESHOLD){
            Serial << "FALSE POSITIVE (" << duration << ")" << nl;
            
        }else{
            Serial << "HUG duration: " << duration << nl;
            cnter.increment();

            // send infos via bluetooth
            altSerial << cnter.get() << duration << nl;
        }

        delay(500);
    }
}
