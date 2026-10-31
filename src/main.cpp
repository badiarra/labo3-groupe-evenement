
#include <Arduino.h>
#include "ldr/LDR.h"
#include "led/Led.h"

// Broches des DELs
#define LED_VERTE 16
#define LED_JAUNE 17
#define LED_ROUGE 18
#define LDR_PIN   4

// Seuils
#define SEUIL_FAIBLE 2800
#define SEUIL_FORT 1200

// Bits des evenements
#define LUM_FAIBLE  (1 << 0)
#define LUM_MOYENNE (1 << 1)
#define LUM_FORTE   (1 << 2)

// Creation de l'objet LDR
LDR capteur(LDR_PIN);

Led ledGreen(LED_VERTE, "verte");
Led ledJaune(LED_JAUNE, "jaune");
Led ledRouge(LED_ROUGE, "rouge");

// Groupe d'evenements
EventGroupHandle_t groupeEvenements;

// TACHE 1 : Lecture du capteur
void vTaskSensor(void *parameter)
{
  int niveau = 0;

    // Conversion niveau vers evenement
    EventBits_t evenement;

    while (1)
    {
        // Lecture du niveau
        niveau = capteur.getValue();

        Serial.println(niveau);

        // Effacer les anciennes valeurs
        xEventGroupClearBits(groupeEvenements, LUM_FAIBLE | LUM_MOYENNE | LUM_FORTE);

        // Determination de la luminosite
        if (niveau > SEUIL_FAIBLE)
        {
            xEventGroupSetBits(groupeEvenements, LUM_FAIBLE);

            Serial.println("Luminosite faible");
        }
        else if (niveau >= SEUIL_FORT)
        {
            xEventGroupSetBits(groupeEvenements, LUM_MOYENNE );

            Serial.println("Luminosite moyenne");
        }
        else
        {
            xEventGroupSetBits(groupeEvenements, LUM_FORTE);

            Serial.println("Luminosite forte");
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// TACHE 2 : Controle des DELs
void vTaskControl(void *parameter)
{
    EventBits_t bits;

    while (1)
    {
        bits = xEventGroupWaitBits(
            groupeEvenements,
            LUM_FAIBLE | LUM_MOYENNE | LUM_FORTE,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY
        );

        if (bits & LUM_FAIBLE)
        {
            ledVerte.on();
            ledJaune.on();
            ledRouge.on();
        }
        else if (bits & LUM_MOYENNE)
        {
            ledVerte.on();
            ledJaune.on();
            ledRouge.off();
        }
        else if (bits & LUM_FORTE)
        {
            ledVerte.off();
            ledJaune.off();
            ledRouge.on();
        }
    }
}

void setup()
{
    Serial.begin(115200);

    delay(3000);

    // Initialisation du capteur
    capteur.init();
    
    // Initialisation des DELs
    ledRouge.init();
    ledVerte.init();
    ledJaune.init();

    // Creation du groupe d'evenements
    groupeEvenements = xEventGroupCreate();

    if (groupeEvenements == nullptr)
    {
        Serial.println("Erreur Event Group");
        while (1)
        {
            delay(1000);
        }
    }

    // Creation des taches
    xTaskCreate(
        vTaskSensor,
        "Lecture LDR",
        2048,
        nullptr,
        1,
        nullptr
    );

    xTaskCreate(
        vTaskControl,
        "Controle LEDs",
        2048,
        nullptr,
        1,
        nullptr
    );
}

void loop()
{
    vTaskDelay(portMAX_DELAY);
}
