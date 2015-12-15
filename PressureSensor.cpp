#include "PressureSensor.h"
#define nl '\n'


PressureSensor::PressureSensor(int pins[], int nbrOfInputs)
{
    this->nbrOfInputs = nbrOfInputs;
    this->inputs      = new SInput[nbrOfInputs];

    for (int i = 0; i < nbrOfInputs; i++)
    {
        inputs[i].pin = pins[i];
        pinMode(inputs[i].pin, INPUT_PULLUP);
    }

    this->sensitivity = DEFAULT_SENSITIVITY;
}

// -----------

#ifdef PRESSURE_SENSOR_DEBUG
bool PressureSensor::isPressed()
{
    double value = 0;
    int count = 0;
    
    Serial << nl;

    for (int i = 0; i < nbrOfInputs; i++)
    {
        value = analogRead(inputs[i].pin);
        double delta = abs(inputs[i].refValue - value) / inputs[i].refValue; 

        if(delta > sensitivity)
        {
            Serial << "[" << i << "] ## " << value << " | " << delta << nl;
            count++;
        }
        else
        {
            Serial << "[" << i << "]    " << value << " | " << delta << nl;
        }
    }

    return count > 1;
}
#else
bool PressureSensor::isPressed()
{
    for (int i = 0; i < nbrOfInputs; i++)
    {
        double val = analogRead(inputs[i].pin);
        double delta = abs(inputs[i].refValue - val) / inputs[i].refValue; 

        if(delta > sensitivity) return true;
    }
    return false;
}
#endif

// --------

void PressureSensor::setSensitivity(double sensitivity)
{
    this->sensitivity = sensitivity;
}

// -------

void PressureSensor::calibrate()
{
    for (int i = 0; i < nbrOfInputs; i++)
    {
          int count = 0, val = 0;

          for(int j = 0; j < MAX_SAMPLES; j++){
              val = analogRead(inputs[i].pin);
              count += val;
              delay(300);
          }

          inputs[i].refValue = floor(count / MAX_SAMPLES);

          #ifdef HUGGI_PRESSURE_SENSOR_DEBUG
            Serial << " [" << i << "] ref value = " << val << nl;
          #endif          
    }
}