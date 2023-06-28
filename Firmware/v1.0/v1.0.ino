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

//numeracion de pantallas
#define pagPpal_part1                     1
#define pagPpal_part2                     2
#define pagPpal_part3                     3
#define pagConfMenuPpal_part1             4
#define pagConfMenuPpal_part2             5
#define pagConfMenuExt_part1              6
#define pagConfMenuExt_part2              7
#define pagConfMenuExt_part3              8
#define pagConfSubMenuExt                 9
#define pagConfSubMenuExtTemp            10
#define pagConfMenuCol_part1             11
#define pagConfMenuCol_part2             12
#define pagConfMenuCol_part3             13
#define pagCreditos_part1                14
#define pagCreditos_part2                15

//cantidad de paginas por pantalla
#define nPagPpal                  3
#define nPagConfMenuPpal          2
#define nPagConfMenuExt           3
#define nPagConfSubMenuExt        1
#define nPagConfSubMenuExtTemp    1
#define nPagConfMenuCol           3
#define nPagCreditos              1

int pantallaActual = pagConfMenuPpal_part1; //Al iniciar el equipo, luego de la presentación, muestra en pantalla la 1er parte del menu de configuración

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

// Define los nombres de las opciones del menú principal
//String mainMenuOptionNames[numMainMenuOptions] = {"Opción 1", "Opción 2", "Opción 3"};


const char screenPresentacion[4][21] PROGMEM = {
                             "      Proyecto      ",
                             "     PlasticBot     ",
                             "        v2.0        ",
                             "                    ",
                            };

                          
const char screenPagPpal_1[4][21]PROGMEM = {
                          "Ext1:         Col1: ",
                          "Ext2:         Col2: ",
                          "Ext3:         Col3: ",
                          "Alarma sonora:      ",
                          };

const char screenPagPpal_2[4][21]PROGMEM = {
                          "Ext4:         Col4: ",
                          "Ext5:         Col5: ",
                          "Ext6:         Col6: ",
                          "Alarma sonora:      ",
                          };

const char screenPagPpal_3[4][21]PROGMEM = {
                          "Ext7:         Col7: ",
                          "                    ",
                          "                    ",
                          "Alarma sonora:      ",
                          };

const char screenPagConfMenuPpal_1[4][21]PROGMEM = {
                                  "Volver...           ",
                                  "Extrusores fil.     ",
                                  "Colectores fil.     ",
                                  "Alarma              ",
                                  };

const char screenPagConfMenuPpal_2[4][21]PROGMEM = {
                                  "Creditos            ",
                                  "                    ",
                                  "                    ",
                                  "                    ",
                                  };

const char screenPagConfMenuExt_1[4][21]PROGMEM = {
                                  "Volver...           ",
                                  "Extrusor 1          ",
                                  "Extrusor 2          ",
                                  "Extrusor 3          ",
                                  };

const char screenPagConfMenuExt_2[4][21]PROGMEM = {
                                  "Volver...           ",
                                  "Extrusor 4          ",
                                  "Extrusor 5          ",
                                  "Extrusor 6          ",
                                  };

const char screenPagConfMenuExt_3[4][21]PROGMEM = {
                                  "Volver...           ",
                                  "Extrusor 7          ",
                                  "                    ",
                                  "                    ",
                                  };

const char screenPagConfSubMenuExt_1[4][21] = {
                                    "Volver...           ",
                                    "Apagar              ",
                                    "Purgar              ",
                                    "Config. Temp.       ",
                                    };

const char screenPagConfSubMenuExtTemp_1[4][21]PROGMEM = {
                                        "Elija temp. trabajo ",
                                        "                    ",
                                        "            /250 C  ",
                                        "                    ",
                                        };

const char screenPagConfMenuCol_1[4][21]PROGMEM = {
                                  "Volver...           ",
                                  "Colector 1          ",
                                  "Colector 2          ",
                                  "Colector 3          ",
                                  };

const char screenPagConfMenuCol_2[4][21]PROGMEM = {
                                  "Volver...           ",
                                  "Colector 4          ",
                                  "Colector 5          ",
                                  "Colector 6          ",
                                  };

const char screenPagConfMenuCol_3[4][21]PROGMEM = {
                                  "Volver...           ",
                                  "Colector 7          ",
                                  "                    ",
                                  "                    ",
                                  };

const char screenPagCreditos[4][21]PROGMEM = {
                            "  PlasticBot es un  ",
                            "   proyecto libre   ",
                            "Esp.Ing.Vargas Alice",
                            "      Año 2023      ",
                             };
const char iconos[2][2]PROGMEM = {"X" , ">"};

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
/*  
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
*/
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
/*
#define pagPpal_part1                     1
#define pagPpal_part2                     2
#define pagPpal_part3                     3
#define pagConfMenuPpal_part1             4
#define pagConfMenuPpal_part2             5
#define pagConfMenuExt_part1              6
#define pagConfMenuExt_part2              7
#define pagConfMenuExt_part3              8
#define pagConfSubMenuExt                 9
#define pagConfSubMenuExtTemp            10
#define pagConfMenuCol_part1             11
#define pagConfMenuCol_part2             12
#define pagConfMenuCol_part3             13
#define pagCreditos_part1                14
#define pagCreditos_part2                15
*/
void updateDataGUI(uint32_t *_indexes , uint32_t *_btn , PETfilConv *_petFilConv , uint32_t *_currentScreen , uint32_t *_vel_col)
//void updateDataGUI()
{
  if(*_btn != NoPressed)  //si movieron el encoder
  {
      if(*_btn == Ok)   //presionaron encoder
      {
        switch(*_currentScreen)
        {
          case pagPpal_part1: //pantalla de trabajo
            *_currentScreen = pantallaMainMenu_part1;
            _indexes[cont_item_PrinMenu] = 0;
          break;
          /****************/
          case pagPpal_part2: //pantalla de trabajo
            *_currentScreen = pantallaMainMenu_part1;
            _indexes[cont_item_PrinMenu] = 0;
          break;
          /****************/
          case pagPpal_part3: //pantalla de trabajo
            *_currentScreen = pantallaMainMenu_part1;
            _indexes[cont_item_PrinMenu] = 0;
          break;
          /****************/
          case pagConfMenuPpal_part1: //menu principal (parte 1)
            if(_indexes[cont_item_PrinMenu] == 0) *_currentScreen = pantallaWorkingScreen;
            else if(_indexes[cont_item_PrinMenu] == 1) *_currentScreen = pantallaSubMenuExtrusores;
            else if(_indexes[cont_item_PrinMenu] == 2) *_currentScreen = pantallaSubMenuColectoresFil;
            else if(_indexes[cont_item_PrinMenu] == 3) *_currentScreen = pantallaSubMenuAlarmas;
          break;
          /****************/
          case pagConfMenuPpal_part2: //menu principal (parte 2)
            if(_indexes[cont_item_PrinMenu] == 4) *_currentScreen = pantallaSubMenuHistorial;
            else if(_indexes[cont_item_PrinMenu] == 5) *_currentScreen = pantallaSubMenuAcercaDe_part1;
          break;
          /****************/          
          case pagConfMenuExt_part1: //menu EXTRUSORES (parte 1)
            if(_indexes[cont_item_SubMenuExtrusores] == 0) *_currentScreen = pantallaMainMenu_part1;
            else if (_indexes[cont_item_SubMenuExtrusores] == 1) *_currentScreen = pantallaSubmenuSetTempExt;
          break;
          /****************/
          case pagConfMenuExt_part2: //menu EXTRUSORES (parte 2)
            if(_indexes[cont_item_SubMenuExtrusores] == 0) *_currentScreen = pantallaMainMenu_part1;
            else if (_indexes[cont_item_SubMenuExtrusores] == 1) *_currentScreen = pantallaSubmenuSetTempExt;
          break;
          /****************/ 
          case pagConfMenuExt_part3: //menu EXTRUSORES (parte 3)
            if(_indexes[cont_item_SubMenuExtrusores] == 0) *_currentScreen = pantallaMainMenu_part1;
            else if (_indexes[cont_item_SubMenuExtrusores] == 1) *_currentScreen = pantallaSubmenuSetTempExt;
          break;
          /****************/                   
          case pagConfSubMenuExt:  //sub menu EXTRUSORES
            if(_indexes[cont_item_SubMenuColectores] == 0) *_currentScreen = pantallaMainMenu_part1;
            else if(_indexes[cont_item_SubMenuColectores] == 1) *_currentScreen = pantallaSubMenuSelVelocidadColFil;
          break;
          /****************/
          case pagConfSubMenuExtTemp:  //sub menu EXTRUSORES confugracion Tª de trabajo
            if(_indexes[cont_item_SubMenuAlarmas] == 0) *_currentScreen = pantallaMainMenu_part1;
            else if(_indexes[cont_item_SubMenuAlarmas] == 1)
            {
              if(_petFilConv -> lightAlarm_state == false)  _petFilConv -> lightAlarm_state = true;
              else  _petFilConv -> lightAlarm_state = false;
              //changeAlarms = true;
            }
            else if(_indexes[cont_item_SubMenuAlarmas] == 2)
            {
              if(_petFilConv -> soundAlarm_state == false)  _petFilConv -> soundAlarm_state = true;
              else  _petFilConv -> soundAlarm_state = false;
              //changeAlarms = true;
            }

            //if((_petFilConv -> soundAlarm_state == true)||(_petFilConv -> lightAlarm_state == true))  _petFilConv -> filDetector_state = true;
            //else _petFilConv -> filDetector_state = false;
          break;
          /****************/
          case pagConfMenuCol_part1:  //menu COLECTORES (parte 1)
            if(_indexes[cont_item_SubMenuHistorial] == 0) *_currentScreen = pantallaMainMenu_part2;
            else{} //borra el valor acumulador del extrusor
          break;
          /****************/
          case pagConfMenuCol_part2:  //menu COLECTORES (parte 2)

            if(_indexes[cont_item_SubMenuSelVelCol] == 0)
            {
              if(*_vel_col != APAGADO)
              {
                *_vel_col = APAGADO;
                _petFilConv -> col_state = false;
                ControlMotor(_petFilConv);
              }
            }

            else if(_indexes[cont_item_SubMenuSelVelCol] == 1)
            {
              if(*_vel_col != VELOCIDAD_1)
              {
                *_vel_col = VELOCIDAD_1;
                _petFilConv -> col_state = true;
                ControlMotor(_petFilConv);
              }
            }

            else if(_indexes[cont_item_SubMenuSelVelCol] == 2)
            {
              if(*_vel_col != VELOCIDAD_2)
              {
                *_vel_col = VELOCIDAD_2;
                _petFilConv -> col_state = true;
                ControlMotor(_petFilConv);
              }
            }
            else if(_indexes[cont_item_SubMenuSelVelCol] == 3)
            {
              if(*_vel_col != VELOCIDAD_3)
              {
                *_vel_col = VELOCIDAD_3;
                _petFilConv -> col_state = true;
                ControlMotor(_petFilConv);
              }
            }

            *_currentScreen = pantallaSubMenuColectoresFil;
          break;
          /****************/
          case pagConfMenuCol_part3:  //menu COLECTORES (parte 3)
            *_currentScreen = pantallaMainMenu_part2;
          break;
          /****************/
          case pagCreditos_part1: //menu CREDITOS (parte 1)
            *_currentScreen = pantallaMainMenu_part2;
          break;
          /****************/
          case pagCreditos_part2: //menu CREDITOS (parte 2)
            _petFilConv -> setExtTemp = _petFilConv -> previousSetExtTemp;
            *_currentScreen = pantallaSubMenuExtrusores;
          break;
          /****************/
          default: break;
        }
      }

      else if(*_btn == Left) //rotaron encoder a izq
      {
          switch(*_currentScreen)
          {
            case pantallaWorkingScreen: //pantalla de trabajo
              //NADA
            break;

            case pantallaMainMenu_part1:
              if(_indexes[cont_item_PrinMenu] == 0) _indexes[cont_item_PrinMenu] = 0;
              else  _indexes[cont_item_PrinMenu]--;
            break;

            case pantallaMainMenu_part2:
              if(_indexes[cont_item_PrinMenu] <= 4)
              {
                *_currentScreen = pantallaMainMenu_part1;
                _indexes[cont_item_PrinMenu] = 3;
              }
              else  _indexes[cont_item_PrinMenu]--;
            break;

            case pantallaSubMenuSelVelocidadColFil:
              if(_indexes[cont_item_SubMenuSelVelCol] == 0) _indexes[cont_item_SubMenuSelVelCol] = 0;
              else  _indexes[cont_item_SubMenuSelVelCol]--;
            break;

            case pantallaSubMenuAcercaDe_part2: *_currentScreen = pantallaSubMenuAcercaDe_part1;
            break;

            case pantallaSubmenuSetTempExt:


            //  if(_petFilConv -> setExtTemp == 0)  _petFilConv -> setExtTemp = 0;
            //  else _petFilConv -> setExtTemp--;
              if(_petFilConv -> previousSetExtTemp == 0) _petFilConv -> previousSetExtTemp = 0;
              else _petFilConv -> previousSetExtTemp--;
            break;

            default:
              if(_indexes[*_currentScreen] == 0) _indexes[*_currentScreen] = 0;
              else _indexes[*_currentScreen]--;
          }
      }


      else if (*_btn == Right)  //rotaron encoder a der
      {
          switch(*_currentScreen)
          {
            case pantallaWorkingScreen: //pantalla de trabajo
              //NADA
            break;

            case pantallaMainMenu_part1:
              if(_indexes[cont_item_PrinMenu] >= 3)
              {
                *__currentScreen = pantallaMainMenu_part2;
                _indexes[cont_item_PrinMenu] = 4;
              }
              else  _indexes[cont_item_PrinMenu]++;
            break;

            case pantallaMainMenu_part2:
              if(_indexes[cont_item_PrinMenu] >= 5) _indexes[cont_item_PrinMenu] = 5;
              else  _indexes[cont_item_PrinMenu]++;
            break;

            case pantallaSubMenuExtrusores:
              if(_indexes[*_currentScreen] == (cantItemsSubMenuExtrusores - 1)) _indexes[*_currentScreen] = cantItemsSubMenuExtrusores - 1;
              else _indexes[*_currentScreen]++;
            break;

            case pantallaSubMenuColectoresFil:
              if(_indexes[*_currentScreen] == (cantItemsSubMenuColectores - 1)) _indexes[*_currentScreen] = cantItemsSubMenuColectores - 1;
              else _indexes[*_currentScreen]++;
            break;

            case pantallaSubMenuSelVelocidadColFil:
              if(_indexes[cont_item_SubMenuSelVelCol] >= 3) _indexes[cont_item_PrinMenu] = 3;
              else  _indexes[cont_item_SubMenuSelVelCol]++;
            break;

            case pantallaSubMenuAlarmas:
              if(_indexes[*_currentScreen] == (cantItemsSubMenuAlarmas - 1)) _indexes[*_currentScreen] = cantItemsSubMenuAlarmas - 1;
              else _indexes[*_currentScreen]++;
            break;

            case pantallaSubMenuHistorial:
              if(_indexes[*_currentScreen] == (cantItemsSubMenuHistorial - 1)) _indexes[*_currentScreen] = cantItemsSubMenuHistorial - 1;
              else _indexes[*_currentScreen]++;
            break;

            case pantallaSubMenuAcercaDe_part1: *_currentScreen = pantallaSubMenuAcercaDe_part2;
            break;

            case pantallaSubmenuSetTempExt: //menu seteo de temperatura COLECTORES

              //if(_petFilConv -> setExtTemp == 250) _petFilConv -> setExtTemp = 250;
              //else _petFilConv -> setExtTemp++; //Temp++
              if(_petFilConv -> previousSetExtTemp == 250) _petFilConv -> previousSetExtTemp = 250;
              else _petFilConv -> previousSetExtTemp++; //Temp++
            break;

            default: break;
          }
      }
  }
}

void ImprimirDisplay(const char _screen[][21])
{
  char sbuff[21];
  
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
