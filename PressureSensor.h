#ifndef PRESSURE_SENSOR
#define PRESSURE_SENSOR

#include <Arduino.h>
#include "Streaming.h"

// #define HUGGI_PRESSURE_SENSOR_DEBUG

#define MAX_SAMPLES          4      // samples for calibation
#define DEFAULT_SENSITIVITY  0.75

typedef struct {
    int pin;
    int refValue;
} SInput;


class PressureSensor 
{
    public:
        PressureSensor(int *, int);
        bool isPressed();
        void setSensitivity(double);
        void calibrate();

    private:
        int nbrOfInputs;
        double sensitivity;
        SInput * inputs;
};

#endif