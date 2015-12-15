// TmDisplayCount.ino
//#include <Serial.h>

#define COUNT

// ---------------- TM
#include <TM1640.h>
#include "TmDisplayNbr.h"

TM1640 left(6,7);
TM1640 right(10,11);

TmPair tmPair(left,right);

// ---------------- BT

#include <Streaming.h>

#define S_BAUDRATE    9600 // serial baudrate
#define nl '\n'

//AltSoftSerial altSerial; //On Arduino Uno and Lilypad : TX = pin 9 | RX = pin 8

// ---------------- PS

#include "PressureSensor.h"
#define DURATION_THRESHOLD 1000 // in ms

int pt_inputs[] = {A0, A1}; 
PressureSensor sensor(pt_inputs, 2);

bool displayCount = true;
int hugCount = 0;
long duration, totalDuration = 0;

// ==========================

void setup() {
    Serial.begin(S_BAUDRATE);
    //altSerial.begin(AS_BAUDRATE);
    sensor.calibrate();
    tmPair.set(0); // light the led board
    Serial << "start" << nl;
}


void loop() {

    // check hug
    if(sensor.isPressed()){

        Serial << "Start ";
        long start = millis();

        while(sensor.isPressed()){ 
            delay(40);
            Serial << ".";
            Serial.flush(); 
        }

        duration = millis() - start;

        if(duration < DURATION_THRESHOLD){
            Serial << "FALSE POSITIVE (" << duration << ")" << nl;
            
        }else{
            Serial << "HUG duration: " << duration << nl;
            hugCount++;
            totalDuration += duration;

            updateDisplay();
            // send infos via bluetooth
            sendInfos();
        }

        delay(500);
    }

    // check commands from android
    if(Serial.available()){
        char c = Serial.read();
        if(c == 'I'){ // I for infos
            sendInfos();
            Serial << "infos sent" << nl;
            
        }else if(c == 'R'){ // r for Reset
            duration = 0;
            hugCount = 0;
            updateDisplay();
            Serial << "reset ok." << nl;
        
        }else if(c == 'C'){ // c for calibrate
            sensor.calibrate();
            Serial << "calibration done." << nl;

        }else if(c == 'D'){ // d for duration: display duration
            displayCount = false;
            updateDisplay();
            Serial << "switched to duration mode." << nl;

        }else if(c == 'H'){ // h for hugs: display count
            displayCount = true;
            updateDisplay();
            Serial << "switched to count mode." << nl;

        }else{
            Serial << "BT ? " << c << " ?" << nl;
        }
    }
}

void updateDisplay(){
    tmPair.setColor(displayCount);
    if(displayCount){
        tmPair.set(hugCount);
    }else{
        int sec = (int)(duration / 100);
        tmPair.set(sec);
    }
}


void sendInfos(){
    Serial << "# " << hugCount << " " << duration << " " << totalDuration << nl;
}
