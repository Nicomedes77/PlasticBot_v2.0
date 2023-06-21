#include <Wire.h>               //manejo protocolo I2C
#include <LiquidCrystal_I2C.h>  //manejo display LCD
#include <Encoder.h>            //manejo de encoder rotativo KY-040
#include "gui.h"
#include "temperatureControl.h"
#include "PETalarms.h"
#include "motorControl.h"
#include "v1.0.h"

// Define las conexiones del display
LiquidCrystal_I2C lcd(0x3F, 20, 4);

// Define las conexiones del encoder
Encoder myEncoder(2, 3);
int g_btnPressed;
// Define el número de opciones del menú principal
const int numMainMenuOptions = 3;

// Define los nombres de las opciones del menú principal
String mainMenuOptionNames[numMainMenuOptions] = {"Opción 1", "Opción 2", "Opción 3"};

// Define el número de opciones del submenú
const int numSubMenuOptions = 2;

// Define los nombres de las opciones del submenú
String subMenuOptionNames[numSubMenuOptions] = {"Subopción 1", "Subopción 2"};

// Define el índice actual del menú principal
volatile int currentMainMenuIndex = 0;

// Define el índice actual del submenú
volatile int currentSubMenuIndex = 0;

// Define el valor anterior del encoder
volatile long lastValue = -999;

// Prototipos de funciones

void PetConv_Init(PETfilConv *_petFilConv);

void showMainMenu();
void showSubMenu();
void updateMenu();

void setup() {
  // Inicializa el display
  lcd.init();
  lcd.backlight();

  // Muestra la primera opción del menú principal en el display
  showMainMenu();

  // Define la interrupción externa para el encoder
  attachInterrupt(digitalPinToInterrupt(2), updateMenu, CHANGE);
}

void loop()
{
  // Nada que hacer aquí
}

void showMainMenu()
{
  // Limpia el display
  lcd.clear();

  // Muestra la opción actual del menú principal en el display
  lcd.setCursor(0, 0);
  lcd.print("> ");
  lcd.print(mainMenuOptionNames[currentMainMenuIndex]);

  // Imprime el número de opciones del menú principal en la pantalla
  lcd.setCursor(18, 0);
  lcd.print(numMainMenuOptions);

  // Define la posición del cursor en la segunda fila del display
  lcd.setCursor(0, 1);
}

void showSubMenu()
{
  // Limpia el display
  lcd.clear();

  // Muestra la opción actual del submenú en el display
  lcd.setCursor(0, 0);
  lcd.print("> ");
  lcd.print(subMenuOptionNames[currentSubMenuIndex]);

  // Imprime el número de opciones del submenú en la pantalla
  lcd.setCursor(18, 0);
  lcd.print(numSubMenuOptions);

  // Define la posición del cursor en la segunda fila del display
  lcd.setCursor(0, 1);
}

void updateMenu()
{
  // Obtiene el valor actual del encoder
  long currentValue = myEncoder.read();

  // Si el valor ha cambiado, actualiza el menú
  if (currentValue != lastValue)
  {
    // Si estamos en el menú principal, actualiza el índice del menú principal
    if (currentSubMenuIndex == -1)
    {
      currentMainMenuIndex += (currentValue - lastValue) / 4;

      // Asegura que el índice esté en el rango válido
      if (currentMainMenuIndex < 0)
      {
        currentMainMenuIndex = 0;
      }
      if (currentMainMenuIndex >= numMainMenuOptions)
      {
        currentMainMenuIndex = numMainMenuOptions - 1;
      }
    }

    // Muestra la opción actual del menú principal en el display
    showMainMenu();
  }
}
