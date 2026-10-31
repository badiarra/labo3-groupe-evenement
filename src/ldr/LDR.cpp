
#include "LDR.h"

// Constructeur
LDR::LDR(int broche)
{
    pin = broche;
}

// Initialisation
void LDR::init()
{
    pinMode(pin, INPUT);
    analogReadResolution(12);
}

// Lecture analogique
int LDR::getValue()
{
    return analogRead(pin);
}
