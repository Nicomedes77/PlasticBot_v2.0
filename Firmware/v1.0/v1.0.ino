#include <Wire.h>               //manejo protocolo I2C
#include <LiquidCrystal_I2C.h>  //manejo display LCD
#include <Encoder.h>            //manejo de encoder rotativo KY-040
#include "gui.h"
#include "temperatureControl.h"
#include "PETalarms.h"
#include "motorControl.h"
#include "v1.0.h"

//Conexiones del display
LiquidCrystal_I2C lcd(0x3F, 20, 4);

#define primer_fila   0
#define segunda_fila  1
#define tercera_fila  2
#define cuarta_fila   3

//Conexiones del encoder
Encoder myEncoder(2, 3);
int g_btnPressed;   //guarda estado del encoder

//Rotative Encoder constants
#define NoPressed   0
#define Ok          1
#define Left        2
#define Right       3

//cantidad de paginas por pantalla
#define nPagPpal                  3
#define nPagConfMenuPpal          2
#define nPagConfMenuExt           3
#define nPagConfSubMenuExt        1
#define nPagConfSubMenuExtTemp    1
#define nPagConfMenuCol           3
#define nPagCreditos              1

//numero de paginas por pantalla
const int nPagDisplay[7] = {nPagPpal,                 //menú principal
                            nPagConfMenuPpal,         //menu de configuración principal
                            nPagConfMenuExt,          //menu de configuración extrusores
                            nPagConfSubMenuExt,       //menu de configuración colectores
                            nPagConfSubMenuExtTemp,   //menu de configuración temperatura en extrusores
                            nPagConfMenuCol,          //menu de configuración colectores
                            nPagCreditos              //créditos
                            };

//cantidad de items por pantalla
#define itemPagPpal                 3                                
#define itemPagConfMenuPpal         5
#define itemPagConfMenuExt          12 
#define itemPagConfSubMenuExt       4 
//#define itemPagConfSubMenuExtTemp    
#define itemPagConfMenuCol          12 
//#define itemPagCreditos              

//numero de item por pantalla
//const int nItemsPag[7] = 

// Define el número de opciones del menú principal
const int numMainMenuOptions = 3;

// Define los nombres de las opciones del menú principal
//String mainMenuOptionNames[numMainMenuOptions] = {"Opción 1", "Opción 2", "Opción 3"};


const char* screenPresentacion[] = {
                             "      Proyecto      ",
                             "     PlasticBot     ",
                             "        v2.0        ",
                             "                    ",
                            };

                          
char* screenPagPpal_1[] = {
                          "Ext1:     Col1:     ",
                          "Ext2:     Col2:     ",
                          "Ext3:     Col3:     ",
                          "Alarma sonora:      ",
                          };

char* screenPagPpal_2[] = {
                          "Ext4:     Col4:     ",
                          "Ext5:     Col5:     ",
                          "Ext6:     Col6:     ",
                          "Alarma sonora:      ",
                          };

char* screenPagPpal_3[] = {
                          "Ext7:     Col7:     ",
                          "Ext8:     Col8:     ",
                          "Ext9:     Col9:     ",
                          "Alarma sonora:      ",
                          };

char* screenPagConfMenuPpal_1[] = {
                                  "Volver...           ",
                                  "Extrusores fil.     ",
                                  "Colectores fil.     ",
                                  "Alarma              ",
                                  };

char* screenPagConfMenuPpal_2[] = {
                                  "Creditos            ",
                                  "                    ",
                                  "                    ",
                                  "                    ",
                                  };

char* screenPagConfMenuExt_1[] = {
                                  "Volver...           ",
                                  "Extrusor 1          ",
                                  "Extrusor 2          ",
                                  "Extrusor 3          ",
                                  };

char* screenPagConfMenuExt_2[] = {
                                  "Volver...           ",
                                  "Extrusor 4          ",
                                  "Extrusor 5          ",
                                  "Extrusor 6          ",
                                  };

char* screenPagConfMenuExt_3[] = {
                                  "Volver...           ",
                                  "Extrusor 7          ",
                                  "Extrusor 8          ",
                                  "Extrusor 9          ",
                                  };

char* screenPagConfSubMenuExt_1[] = {
                                    "Volver...           ",
                                    "Apagar              ",
                                    "Purgar              ",
                                    "Config. Temp.       ",
                                    };

char* screenPagConfSubMenuExtTemp_1[] = {
                                        "Elija temp. trabajo ",
                                        "                    ",
                                        "            /250 C  ",
                                        "                    ",
                                        };

char* screenPagConfMenuCol_1[] = {
                                  "Volver...           ",
                                  "Colector 1          ",
                                  "Colector 2          ",
                                  "Colector 3          ",
                                  };

char* screenPagConfMenuCol_2[] = {
                                  "Volver...           ",
                                  "Colector 4          ",
                                  "Colector 5          ",
                                  "Colector 6          ",
                                  };

char* screenPagConfMenuCol_3[] = {
                                  "Volver...           ",
                                  "Colector 7          ",
                                  "Colector 8          ",
                                  "Colector 9          ",
                                  };

char* screenPagCreditos[] = {
                            "  PlasticBot es un  ",
                            "   proyecto libre   ",
                            "Esp.Ing.Vargas Alice",
                            "      Año 2023      ",
                             };
char* iconos[] = {"X" , ">"};

// Define el número de opciones del submenú
const int numSubMenuOptions = 2;

// Define los nombres de las opciones del submenú
//String subMenuOptionNames[numSubMenuOptions] = {"Subopción 1", "Subopción 2"};

// Define el índice actual en cada pantalla
volatile int currentMainMenuIndex = 0;


// Define el valor anterior del encoder
volatile long lastValue = -999;

// Prototipos de funciones
void imprimirPresentacion(const char *_valor);
void PetConv_Init(PETfilConv *_petFilConv);

void showMainMenu();
void showSubMenu();
void updateMenu();

void setup() {
  // Inicializa el display
  lcd.init();
  lcd.backlight();

  // Muestra pantalla presentacion
  imprimirPresentacion(screenPresentacion[]);

  // Define la interrupción externa para el encoder
  attachInterrupt(digitalPinToInterrupt(2), updateMenu, CHANGE);
}

void loop()
{
  // Nada que hacer aquí
}

void imprimirPresentacion(const char *_valor)
{
  // Limpia el display
  lcd.clear();

  // Muestra la opción actual del menú principal en el display
  lcd.setCursor(primer_fila, 0);
  lcd.print(_valor[0]);
/*  
//  lcd.print("> ");
  lcd.print(mainMenuOptionNames[currentMainMenuIndex]);

  // Imprime el número de opciones del menú principal en la pantalla
  lcd.setCursor(18, 0);
  lcd.print(numMainMenuOptions);

  // Define la posición del cursor en la segunda fila del display
  lcd.setCursor(0, 1);
*/
}

/*
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
*/

/*
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
*/
