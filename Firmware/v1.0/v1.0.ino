#include <Wire.h>               //manejo protocolo I2C
#include <LiquidCrystal_I2C.h>  //manejo display LCD
#include <YetAnotherPcInt.h>
#include "gui.h"
#include "temperatureControl.h"
#include "PETalarms.h"
#include "motorControl.h"
#include "v1.0.h"

//***************************************************************
//*************** VARIABLES Y CONSTANTES GLOBALES ***************
//***************************************************************

//Conexiones del display
LiquidCrystal_I2C lcd(0x3F, 20, 4);

//Conexiones del encoder
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


const char screenPresentacion[4][20] PROGMEM = {
                             "      Proyecto      ",
                             "     PlasticBot     ",
                             "        v2.0        ",
                             "                    ",
                            };

                          
const char screenPagPpal_1[4][20]PROGMEM = {
                          "Ext1:         Col1: ",
                          "Ext2:         Col2: ",
                          "Ext3:         Col3: ",
                          "Alarma sonora:      ",
                          };

const char screenPagPpal_2[4][20]PROGMEM = {
                          "Ext4:         Col4: ",
                          "Ext5:         Col5: ",
                          "Ext6:         Col6: ",
                          "Alarma sonora:      ",
                          };

const char screenPagPpal_3[4][20]PROGMEM = {
                          "Ext7:         Col7: ",
                          "                    ",
                          "                    ",
                          "Alarma sonora:      ",
                          };

const char screenPagConfMenuPpal_1[4][20]PROGMEM = {
                                  "Volver...           ",
                                  "Extrusores fil.     ",
                                  "Colectores fil.     ",
                                  "Alarma              ",
                                  };

const char screenPagConfMenuPpal_2[4][20]PROGMEM = {
                                  "Creditos            ",
                                  "                    ",
                                  "                    ",
                                  "                    ",
                                  };

const char screenPagConfMenuExt_1[4][20]PROGMEM = {
                                  "Volver...           ",
                                  "Extrusor 1          ",
                                  "Extrusor 2          ",
                                  "Extrusor 3          ",
                                  };

const char screenPagConfMenuExt_2[4][20]PROGMEM = {
                                  "Volver...           ",
                                  "Extrusor 4          ",
                                  "Extrusor 5          ",
                                  "Extrusor 6          ",
                                  };

const char screenPagConfMenuExt_3[4][20]PROGMEM = {
                                  "Volver...           ",
                                  "Extrusor 7          ",
                                  "                    ",
                                  "                    ",
                                  };

const char screenPagConfSubMenuExt_1[4][20] = {
                                    "Volver...           ",
                                    "Apagar              ",
                                    "Purgar              ",
                                    "Config. Temp.       ",
                                    };

const char screenPagConfSubMenuExtTemp_1[4][20]PROGMEM = {
                                        "Elija temp. trabajo ",
                                        "                    ",
                                        "            /250 C  ",
                                        "                    ",
                                        };

const char screenPagConfMenuCol_1[4][20]PROGMEM = {
                                  "Volver...           ",
                                  "Colector 1          ",
                                  "Colector 2          ",
                                  "Colector 3          ",
                                  };

const char screenPagConfMenuCol_2[4][20]PROGMEM = {
                                  "Volver...           ",
                                  "Colector 4          ",
                                  "Colector 5          ",
                                  "Colector 6          ",
                                  };

const char screenPagConfMenuCol_3[4][20]PROGMEM = {
                                  "Volver...           ",
                                  "Colector 7          ",
                                  "                    ",
                                  "                    ",
                                  };

const char screenPagCreditos[4][20]PROGMEM = {
                            "  PlasticBot es un  ",
                            "   proyecto libre   ",
                            "Esp.Ing.Vargas Alice",
                            "      Año 2023      ",
                             };
const char iconos[2][1]PROGMEM = {"X" , ">"};

// Define el número de opciones del submenú
const int numSubMenuOptions = 2;

// Define el índice actual en cada pantalla
volatile int currentMainMenuIndex = 0;


//***************************************************************
//******************* PROTOTIPOS DE FUNCIONES *******************
//***************************************************************

void ImprimirDisplay(const char _screen[][20]);
void Modulos_init(void);
void PetConv_Init(PETfilConv *_petFilConv);
void UpdateSensorsFilament();
void UpdateEncoder();

void showMainMenu();
void showSubMenu();
void updateMenu();

void setup() {
  Modulos_init();

  // Muestra pantalla presentacion
  ImprimirDisplay(screenPresentacion);
}

void loop()
{
  // Nada que hacer aquí
}

void Modulos_init(void)
{
  // Inicializa el display
  lcd.init();
  lcd.backlight();

  //inicializa interrupciones
  pinMode(senFil_1, INPUT_PULLUP);
  PcInt::attachInterrupt(
                          senFil_1, 
                          UpdateSensorsFilament,
                          CHANGE,
                          false
                          );
  pinMode(senFil_2, INPUT_PULLUP);
  PcInt::attachInterrupt(senFil_2, UpdateSensorsFilament);
  pinMode(senFil_3, INPUT_PULLUP);
  PcInt::attachInterrupt(senFil_3, UpdateSensorsFilament);
  pinMode(senFil_4, INPUT_PULLUP);
  PcInt::attachInterrupt(senFil_4, UpdateSensorsFilament);
  pinMode(senFil_5, INPUT_PULLUP);
  PcInt::attachInterrupt(senFil_5, UpdateSensorsFilament);
  pinMode(senFil_6, INPUT_PULLUP);
  PcInt::attachInterrupt(senFil_6, UpdateSensorsFilament);
  pinMode(senFil_7, INPUT_PULLUP);
  PcInt::attachInterrupt(senFil_7, UpdateSensorsFilament);
  pinMode(pin_DT, INPUT_PULLUP);
  PcInt::attachInterrupt(pin_DT, UpdateEncoderCCW);
  pinMode(pin_CLK, INPUT_PULLUP);
  PcInt::attachInterrupt(pin_CLK, UpdateEncoderCW);
  pinMode(pin_SW, INPUT_PULLUP);
  PcInt::attachInterrupt(pin_SW, UpdateEncoderSW);
  
  //inicializa resto de los pines
  pinMode(buzzer, OUTPUT);
  pinMode(ext1_pwm, OUTPUT);
  pinMode(ext2_pwm, OUTPUT);
  pinMode(ext3_pwm, OUTPUT);
  pinMode(ext4_pwm, OUTPUT);
  pinMode(ext5_pwm, OUTPUT);
  pinMode(ext6_pwm, OUTPUT);
  pinMode(ext7_pwm, OUTPUT);
}

void UpdateSensorsFilament()
{

    

}

void UpdateEncoderCW()
{
  static int previousTick;
  int currentTick = millis();
  
  if(currentTick - previousTick > 300)
  {
    if(digitalRead(pin_DT) == digitalRead(pin_CLK)) g_btnPressed = Right;
    else g_btnPressed = Left;
  }
  previousTick = millis();
}

void UpdateEncoderCCW()
{
  static int previousTick;
  int currentTick = millis();
  
  if(currentTick - previousTick > 300)
  {
    if(digitalRead(pin_DT) != digitalRead(pin_CLK)) g_btnPressed = Right;
    else g_btnPressed = Left;
  }
  
  previousTick = millis();
}

void UpdateEncoderSW()
{
  g_btnPressed = Ok;
}

void UpdateGUI()
{

}

void ImprimirDisplay(const char _screen[][20])
{
  char sbuff[20];
  
  // Limpia el display
  lcd.clear();
  for( uint8_t i=0; i<4; i++ )
  {
    lcd.setCursor(0, i);
    strcpy_P( sbuff, &_screen[i][0] );
    lcd.print(sbuff);
  }
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
