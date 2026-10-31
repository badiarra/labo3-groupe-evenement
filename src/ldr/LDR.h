
#ifndef LDR_H
#define LDR_H

#include <Arduino.h>

class LDR
{
private:
    int pin;

public:
    // Constructeur
    LDR(int broche);

    // Initialisation du capteur
    void init();

    // Lecture de la valeur analogique
    int getValue();

};

#endif
