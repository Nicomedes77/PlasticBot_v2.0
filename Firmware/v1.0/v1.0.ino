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
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Rotative Encoder constants
#define NoPressed   0
#define Ok          1
#define Left        2
#define Right       3

uint8_t g_btnPressed = NoPressed;   //guarda estado del encoder

//numeracion de pantallas
#define pagPpal_part1             1
#define pagPpal_part2             2
#define pagPpal_part3             3
#define pagConfMenuPpal_part1     4
#define pagConfMenuPpal_part2     5
#define pagConfMenuExt_part1      6
#define pagConfMenuExt_part2      7
#define pagConfMenuExt_part3      8
#define pagConfSubMenuExt         9
#define pagConfSubMenuExtTemp     10
#define pagConfMenuCol_part1      11
#define pagConfMenuCol_part2      12
#define pagConfMenuCol_part3      13
#define pagCreditos_part1         14
#define pagCreditos_part2         15

//cantidad de items por pantalla
#define cantItemsPagPpal               3
#define cantItemsConfMenuPpal          5
#define cantItemsConfMenuExt           9
#define cantItemsConfSubMenuExt        3
//#define cantItemsConfSubMenuExtTemp
#define cantItemsConfMenuCol           7
#define cantItemsCreditos              2

#define indicePagPpal         0
#define indiceConfMenuPpal    1
#define indiceConfMenuExt     2
#define indiceConfSubMenuExt  3
#define indiceConfMenuCol     4
#define indiceCreditos        5

//numero de item por pantalla
uint8_t indices[6] = { 0, //indicePpal -> menú principal
                       0, //indiceConfMenuPpal -> menu de configuración principal
                       0, //indiceConfMenuExt -> menu de configuración extrusores
                       0, //indiceConfSubMenuExt -> menu de configuración colectores
                       0, //indiceConfMenuCol -> menu de configuración colectores
                       0  //indiceCreditos -> créditos
                     };

//Al iniciar el equipo, luego de la presentación, muestra en pantalla la 1er parte del menu de configuración
uint8_t pantallaActual = pagConfMenuPpal_part1;

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

const char screenPagCreditos_1[4][21]PROGMEM = {
  "  PlasticBot es un  ",
  "   proyecto libre   ",
  "Esp.Ing.Vargas Alice",
  "      Año 2023      ",
};

const char screenPagCreditos_2[4][21]PROGMEM = {
  "      Licencia      ",
  "  Creative Commons  ",
  "                    ",
  "      Año 2023      ",
};

const char iconos[3][2]PROGMEM = {"X" , ">" , " "};

//***************************************************************
//******************* PROTOTIPOS DE FUNCIONES *******************
//***************************************************************

void PrintScreen(const char _screen[][21]);
void Modulos_init(void);
void PetConv_Init(PETfilConv *_petFilConv);
void UpdateEncoder();
void UpdateDataGUI(uint8_t *_indexes , uint8_t *_btn , uint8_t *_currentScreen);
void UpdateCursor(uint8_t *_indexes , uint8_t *_currentScreen);
void UpdateLCD(uint8_t *_currentScreen);

void setup()
{
  Modulos_init();
  PrintScreen(screenPresentacion);    // Muestra pantalla presentacion
  delay(1000);
  PrintScreen(screenPagConfMenuPpal_1);    // Muestra pantalla presentacion
  UpdateLCD(&pantallaActual);
  UpdateCursor(indices , &pantallaActual);
}

void loop()
{
  if (g_btnPressed != NoPressed)
  {
    //void UpdateDataGUI(uint8_t *_indexes , uint8_t *_btn , uint32_t *_currentScreen);
    UpdateDataGUI(indices , &g_btnPressed , &pantallaActual);
    UpdateLCD(&pantallaActual);
    //updateCursor(indexes,&PETfilConv1,&currentScreen,&g_velocidad_Col1);
    UpdateCursor(indices , &pantallaActual);
    g_btnPressed = NoPressed;
  }
}

void Modulos_init(void)
{
  //inicializa monitor serie
  Serial.begin(9600);

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
  PcInt::attachInterrupt(pin_DT , UpdateEncoder , CHANGE);
  //pinMode(pin_CLK, INPUT_PULLUP);
  //PcInt::attachInterrupt(pin_CLK , UpdateEncoderCCW , CHANGE);
  pinMode(pin_SW, INPUT_PULLUP);
  PcInt::attachInterrupt(pin_SW , UpdateEncoderSW , RISING);

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

void PrintScreen(const char _screen[][21])
{
  char sbuff[21];
  // Limpia el display
  ClearScreen();
  for ( uint8_t i = 0; i < 4; i++ )
  {
    lcd.setCursor(0, i);
    strcpy_P( sbuff, &_screen[i][0] );
    lcd.print(sbuff);
  }
}

void PrintScreenWorking_1(const char _screen[][21])
{
  char sbuff[21];
  // Limpia el display
  ClearScreen();
  for ( uint8_t i = 0; i < 4; i++ )
  {
    lcd.setCursor(0, i);
    strcpy_P( sbuff, &_screen[i][0] );
    lcd.print(sbuff);
  }
}

void PrintScreenWorking_2(const char _screen[][21])
{
  char sbuff[21];
  // Limpia el display
  ClearScreen();
  for ( uint8_t i = 0; i < 4; i++ )
  {
    lcd.setCursor(0, i);
    strcpy_P( sbuff, &_screen[i][0] );
    lcd.print(sbuff);
  }
}

void PrintScreenWorking_3(const char _screen[][21])
{
  char sbuff[21];
  // Limpia el display
  ClearScreen();
  for ( uint8_t i = 0; i < 4; i++ )
  {
    lcd.setCursor(0, i);
    strcpy_P( sbuff, &_screen[i][0] );
    lcd.print(sbuff);
  }
}

void PrintScreenSettingTemp(const char _screen[][21])
{
  char sbuff[21];
  // Limpia el display
  ClearScreen();
  for ( uint8_t i = 0; i < 4; i++ )
  {
    lcd.setCursor(0, i);
    strcpy_P( sbuff, &_screen[i][0] );
    lcd.print(sbuff);
  }
}

void UpdateLCD(uint8_t *_currentScreen)
{
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
  static uint8_t previousScreen;

  if (*_currentScreen != previousScreen) //si la pantalla actual es distinto a la anterior o si la pantalla actual es la de seteo de temp de extrusor
  {
    previousScreen = *_currentScreen;

    switch (*_currentScreen)
    {
      case pagPpal_part1: PrintScreenWorking_1(screenPagPpal_1);
        break;

      case pagPpal_part2: PrintScreenWorking_2(screenPagPpal_2);
        break;

      case pagPpal_part3: PrintScreenWorking_3(screenPagPpal_3);
        break;

      case pagConfMenuPpal_part1: PrintScreen(screenPagConfMenuPpal_1);
        break;

      case pagConfMenuPpal_part2: PrintScreen(screenPagConfMenuPpal_2);
        break;

      case pagConfMenuExt_part1: PrintScreen(screenPagConfMenuExt_1);
        break;

      case pagConfMenuExt_part2: PrintScreen(screenPagConfMenuExt_2);
        break;

      case pagConfMenuExt_part3: PrintScreen(screenPagConfMenuExt_3);
        break;

      case pagConfSubMenuExt: PrintScreen(screenPagConfSubMenuExt_1);
        break;

      case pagConfSubMenuExtTemp: PrintScreenSettingTemp(screenPagConfSubMenuExtTemp_1);
        break;

      case pagConfMenuCol_part1: PrintScreen(screenPagConfMenuCol_1);
        break;

      case pagConfMenuCol_part2: PrintScreen(screenPagConfMenuCol_2);
        break;

      case pagConfMenuCol_part3: PrintScreen(screenPagConfMenuCol_3);
        break;

      case pagCreditos_part1: PrintScreen(screenPagCreditos_1);
        break;

      case pagCreditos_part2: PrintScreen(screenPagCreditos_2);
        break;
      default: break;
    }
  }
  /*
    if((_petFilConv -> previousSetExtTemp != _petFilConv -> setExtTemp) && (*currentScreen == pantallaSubmenuSetTempExt))
    {
      printSettingTemp(_petFilConv);
    }
  */
}

void ClearScreen()
{
  lcd.clear();
}

void CleanAllCursors(void)
{
  // Limpia el cursor de todos las lineas
  for ( uint8_t i = 0; i < 4; i++ )
  {
    lcd.setCursor(19, i);
    lcd.print(" ");
  }
}

void PrintCursor(uint8_t _pos)
{
  //imprime en la linea correspondiente
  switch (_pos)
  {
    case 0: 
      lcd.setCursor(19, 0);  //primera linea
      lcd.print(">");
    break;

    case 1: 
      lcd.setCursor(19, 1);  //segunda linea
      lcd.print(">");
    break;

    case 2:
      lcd.setCursor(19, 2);  //tercera linea
      lcd.print(">");
    break;

    case 3: 
      lcd.setCursor(19, 3);  //cuarta linea
      lcd.print(">");
    break;

    case 4: 
      lcd.setCursor(19, 0);  //primer linea
      lcd.print(">");
    break;

    case 5: 
      lcd.setCursor(19, 1);  //segunda linea
      lcd.print(">");
    break;

    case 6: 
      lcd.setCursor(19, 2);  //tercera linea
      lcd.print(">");
    break;

    case 7: 
      lcd.setCursor(19, 3);  //cuarta linea
      lcd.print(">");
    break;

    case 8: 
      lcd.setCursor(19, 0);  //primera linea
      lcd.print(">");
    break;

    case 9: 
      lcd.setCursor(19, 1);  //segunda linea
      lcd.print(">");
    break;

    case 10: 
      lcd.setCursor(19, 2);  //tercera linea
      lcd.print(">");
    break;

    default: break;
  }
}

void UpdateCursor(uint8_t *_indexes , uint8_t *_currentScreen)
{

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

    #define indicePagPpal         0
    #define indiceConfMenuPpal    1
    #define indiceConfMenuExt     2
    #define indiceConfSubMenuExt  3
    #define indiceConfMenuCol     4
    #define indiceCreditos        5
  */
  //limpia todos los cursores existentes
  CleanAllCursors();
  switch (*_currentScreen)
  {
    case pagPpal_part1:
      break;

    case pagPpal_part2:
      break;

    case pagPpal_part3:
      break;

    case pagConfMenuPpal_part1:
      PrintCursor(_indexes[indiceConfMenuPpal]);
      break;

    case pagConfMenuPpal_part2:
      PrintCursor(_indexes[indiceConfMenuPpal]);
      break;

    case pagConfMenuExt_part1:
      PrintCursor(_indexes[indiceConfMenuExt]);
      break;

    case pagConfMenuExt_part2:  PrintCursor(_indexes[indiceConfMenuExt]);
      break;

    case pagConfMenuExt_part3:  PrintCursor(_indexes[indiceConfMenuExt]);
      break;

    case pagConfSubMenuExt: PrintCursor(_indexes[indiceConfSubMenuExt]);
      break;

    case pagConfSubMenuExtTemp:
      break;

    case pagConfMenuCol_part1:  PrintCursor(_indexes[indiceConfMenuCol]);
      break;

    case pagConfMenuCol_part2:  PrintCursor(_indexes[indiceConfMenuCol]);
      break;

    case pagConfMenuCol_part3:  PrintCursor(_indexes[indiceConfMenuCol]);
      break;

    case pagCreditos_part1:
      break;

    case pagCreditos_part2:
      break;
  }
}


void UpdateEncoder()
{
  static int previousTick;
  int currentTick = millis();

  if (currentTick - previousTick > 200)
  {
    if (digitalRead(pin_DT) == digitalRead(pin_CLK))
    {
      g_btnPressed = Right;
      //Serial.println("RIGHT");
    }

    else
    {
      g_btnPressed = Left;
      //Serial.println("LEFT");
    }

  }
  previousTick = millis();
}
/*
  void UpdateEncoderCCW()
  {
  static int previousTick;
  int currentTick = millis();

  if(currentTick - previousTick > 300)
  {
    if(digitalRead(pin_DT) != digitalRead(pin_CLK))
    {
      g_btnPressed = Right;
      Serial.println("RIGHT");
    }
    else
    {
      g_btnPressed = Left;
      Serial.println("LEFT");
    }
  }

  previousTick = millis();
  }
*/
void UpdateEncoderSW()
{
  g_btnPressed = Ok;
  //Serial.println("CLICK");
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

  #define indicePagPpal         0
  #define indiceConfMenuPpal    1
  #define indiceConfMenuExt     2
  #define indiceConfSubMenuExt  3
  #define indiceConfMenuCol     4
  #define indiceCreditos        5
*/
void UpdateDataGUI(uint8_t *_indexes , uint8_t *_btn , uint8_t *_currentScreen)
//void updateDataGUI()
{
  if (*_btn != NoPressed) //si movieron el encoder
  {
    if (*_btn == Ok) //rotaron encoder a izq
    {
      switch (*_currentScreen)
      {
        case pagPpal_part1: //pantalla de trabajo
          *_currentScreen = pagConfMenuPpal_part1;
          _indexes[indiceConfMenuPpal] = 0;
          break;
        /****************/
        case pagPpal_part2: //pantalla de trabajo
          *_currentScreen = pagConfMenuPpal_part1;
          _indexes[indiceConfMenuPpal] = 0;
          break;
        /****************/
        case pagPpal_part3: //pantalla de trabajo
          *_currentScreen = pagConfMenuPpal_part1;
          _indexes[indiceConfMenuPpal] = 0;
          break;
        /****************/
        case pagConfMenuPpal_part1: //menu principal (parte 1)
          if (_indexes[indiceConfMenuPpal] == 0) *_currentScreen = pagPpal_part1;
          else if (_indexes[indiceConfMenuPpal] == 1) *_currentScreen = pagConfMenuExt_part1;
          else if (_indexes[indiceConfMenuPpal] == 2) *_currentScreen = pagConfMenuCol_part1;
          else if (_indexes[indiceConfMenuPpal] == 3) //ACTIVA/DESACTIVA ALARMA
            break;
        /****************/
        case pagConfMenuPpal_part2: //menu principal (parte 2)
          if (_indexes[indiceConfMenuPpal] == 4) *_currentScreen = pagCreditos_part1;
          break;
        /****************/
        case pagConfMenuExt_part1: //menu EXTRUSORES (parte 1)
          if (_indexes[indiceConfMenuExt] == 0) *_currentScreen = pagConfMenuPpal_part1;
          else if (_indexes[indiceConfMenuExt] == 1) *_currentScreen = pagConfSubMenuExtTemp;
          else if (_indexes[indiceConfMenuExt] == 2) *_currentScreen = pagConfSubMenuExtTemp;
          else if (_indexes[indiceConfMenuExt] == 3) *_currentScreen = pagConfSubMenuExtTemp;
          break;
        /****************/
        case pagConfMenuExt_part2: //menu EXTRUSORES (parte 2)
          if (_indexes[indiceConfMenuExt] == 4) *_currentScreen = pagConfMenuPpal_part1;
          else if (_indexes[indiceConfMenuExt] == 5) *_currentScreen = pagConfSubMenuExtTemp;
          else if (_indexes[indiceConfMenuExt] == 6) *_currentScreen = pagConfSubMenuExtTemp;
          else if (_indexes[indiceConfMenuExt] == 7) *_currentScreen = pagConfSubMenuExtTemp;
          break;
        /****************/
        case pagConfMenuExt_part3: //menu EXTRUSORES (parte 3)
          if (_indexes[indiceConfMenuExt] == 8) *_currentScreen = pagConfMenuPpal_part1;
          else if (_indexes[indiceConfMenuExt] == 9) *_currentScreen = pagConfSubMenuExtTemp;
          break;
        /****************/
        case pagConfSubMenuExtTemp:  //sub menu EXTRUSORES configuracion Tª de trabajo
          //
          if ((_indexes[indiceConfMenuExt] == 1) ||
              (_indexes[indiceConfMenuExt] == 2) ||
              (_indexes[indiceConfMenuExt] == 3)
             ) *_currentScreen = pagConfMenuExt_part1;
          //
          else if ((_indexes[indiceConfMenuExt] == 5) ||
                   (_indexes[indiceConfMenuExt] == 6) ||
                   (_indexes[indiceConfMenuExt] == 7)
                  ) *_currentScreen = pagConfMenuExt_part2;
          //
          else if (_indexes[indiceConfMenuExt] == 9) *_currentScreen = pagConfMenuExt_part3;
          break;
        /****************/
        case pagConfMenuCol_part1:  //menu COLECTORES (parte 1)
          if (_indexes[indiceConfMenuCol] == 0) *_currentScreen = pagConfMenuPpal_part1;
          else if ((_indexes[indiceConfMenuCol] == 1) ||
                   (_indexes[indiceConfMenuCol] == 2) ||
                   (_indexes[indiceConfMenuCol] == 3)
                  ) *_currentScreen = pagConfMenuCol_part1;
          break;
        /****************/
        case pagConfMenuCol_part2:  //menu COLECTORES (parte 2)
          if (_indexes[indiceConfMenuCol] == 4) *_currentScreen = pagConfMenuPpal_part1;
          else if ((_indexes[indiceConfMenuCol] == 5) ||
                   (_indexes[indiceConfMenuCol] == 6) ||
                   (_indexes[indiceConfMenuCol] == 7)
                  ) *_currentScreen = pagConfMenuCol_part2;
          break;
        /****************/
        case pagConfMenuCol_part3:  //menu COLECTORES (parte 3)
          if (_indexes[indiceConfMenuCol] == 8) *_currentScreen = pagConfMenuPpal_part1;
          else if (_indexes[indiceConfMenuCol] == 9) *_currentScreen = pagConfMenuCol_part3;
          break;
        /****************/
        case pagCreditos_part1: //menu CREDITOS (parte 1)
          *_currentScreen = pagConfMenuPpal_part2;
          break;
        /****************/
        case pagCreditos_part2: //menu CREDITOS (parte 2)
          *_currentScreen = pagConfMenuPpal_part2;
          break;
        /****************/
        default: break;
      }
    }

    else if (*_btn == Left) //rotaron encoder a izq
    {
      switch (*_currentScreen)
      {
        case pagPpal_part1: //pantalla de trabajo
          *_currentScreen = pagPpal_part1;
          if (_indexes[indicePagPpal] == 0)  _indexes[indicePagPpal] = 0; //decrementa le n° de indice
          break;
        /****************/
        case pagPpal_part2: //pantalla de trabajo
          if (_indexes[indicePagPpal] == 1)  *_currentScreen = pagPpal_part1;
          _indexes[indicePagPpal]--;  //decrementa le n° de indice
          break;
        /****************/
        case pagPpal_part3: //pantalla de trabajo
          if (_indexes[indicePagPpal] == 2)  *_currentScreen = pagPpal_part2;
          _indexes[indicePagPpal]--;  //decrementa le n° de indice
          break;
        /****************/
        case pagConfMenuPpal_part1: //menu principal (parte 1)
          *_currentScreen = pagConfMenuPpal_part1;
          if (_indexes[indiceConfMenuPpal] == 0)  _indexes[indiceConfMenuPpal] = 0; //decrementa le n° de indice
          else  _indexes[indiceConfMenuPpal]--;  //decrementa le n° de indice
          break;
        /****************/
        case pagConfMenuPpal_part2: //menu principal (parte 2)
          if (_indexes[indiceConfMenuPpal] == 4) *_currentScreen = pagConfMenuPpal_part1;
          _indexes[indiceConfMenuPpal]--;  //decrementa le n° de indice
          break;
        /****************/
        case pagConfMenuExt_part1: //menu EXTRUSORES (parte 1)
          if (_indexes[indiceConfMenuExt] == 0) _indexes[indiceConfMenuExt] = 0;
          else _indexes[indiceConfMenuExt]--;  //decrementa le n° de indice
          break;
        /****************/
        case pagConfMenuExt_part2: //menu EXTRUSORES (parte 2)
          if (_indexes[indiceConfMenuExt] == 4) *_currentScreen = pagConfMenuExt_part1;
          _indexes[indiceConfMenuExt]--;  //decrementa le n° de indice
          break;
        /****************/
        case pagConfMenuExt_part3: //menu EXTRUSORES (parte 3)
          if (_indexes[indiceConfMenuExt] == 8) *_currentScreen = pagConfMenuExt_part2;
          _indexes[indiceConfMenuExt]--;  //decrementa le n° de indice
          break;
        /****************/
        case pagConfSubMenuExt:  //sub menu EXTRUSORES
          if (_indexes[indiceConfMenuCol] == 0) _indexes[indiceConfMenuCol] = 0;
          else _indexes[indiceConfMenuCol]--;
          break;
        /****************/
        case pagConfSubMenuExtTemp:  //sub menu EXTRUSORES confugracion Tª de trabajo
          //baja temp del extrusor
          //actualiza pantalla con valor de temp nueva
          break;
        /****************/
        case pagConfMenuCol_part1:  //menu COLECTORES (parte 1)
          if (_indexes[indiceConfMenuCol] == 0) _indexes[indiceConfMenuCol] = 0;
          else _indexes[indiceConfMenuCol]--;  //decrementa le n° de indice
          break;
        /****************/
        case pagConfMenuCol_part2:  //menu COLECTORES (parte 2)
          if (_indexes[indiceConfMenuCol] == 4) *_currentScreen = pagConfMenuCol_part1;
          else _indexes[indiceConfMenuCol]--;  //decrementa le n° de indice
          break;
        /****************/
        case pagConfMenuCol_part3:  //menu COLECTORES (parte 3)
          if (_indexes[indiceConfMenuCol] == 0) *_currentScreen = pagConfMenuCol_part2;
          else _indexes[indiceConfMenuCol]--;  //decrementa le n° de indice
          break;
        /****************/
        case pagCreditos_part1:  //menu CREDITOS (parte 1)
          if (_indexes[indiceCreditos] == 0) _indexes[indiceCreditos] = 0;
          break;
        /****************/
        case pagCreditos_part2:  //menu CREDITOS (parte 2)
          if (_indexes[indiceCreditos] == 1)
          {
            _indexes[indiceCreditos] = 0;
            *_currentScreen = pagCreditos_part1;
          }
          break;
        /****************/
        default: break;
      }
    }
    /*
      #define indicePagPpal         0
      #define indiceConfMenuPpal    1
      #define indiceConfMenuExt     2
      #define indiceConfSubMenuExt  3
      #define indiceConfMenuCol     4
      #define indiceCreditos        5
    */
    else if (*_btn == Right)  //rotaron encoder a der
    {
      switch (*_currentScreen)
      {
        case pagPpal_part1: //pantalla de trabajo
          *_currentScreen = pagPpal_part2;
          _indexes[indicePagPpal]++;  //incrementa el n° de indice
          break;
        /****************/
        case pagPpal_part2: //pantalla de trabajo
          *_currentScreen = pagPpal_part3;
          _indexes[indicePagPpal]++;  //incrementa el n° de indice
          break;
        /****************/
        case pagPpal_part3: //pantalla de trabajo
          //*_currentScreen = pagPpal_part3;
          break;
        /****************/
        case pagConfMenuPpal_part1: //menu principal (parte 1)
          if (_indexes[indiceConfMenuPpal] == 3)
          {
            *_currentScreen = pagConfMenuPpal_part2;
            _indexes[indiceConfMenuPpal]++;
          }
          else  _indexes[indiceConfMenuPpal]++;
          break;
        /****************/
        case pagConfMenuPpal_part2: //menu principal (parte 2)
          _indexes[indiceConfMenuPpal]++;
          if (_indexes[indiceConfMenuPpal] >= cantItemsConfMenuPpal) _indexes[indiceConfMenuPpal] = cantItemsConfMenuPpal - 1;
          //else  _indexes[indiceConfMenuPpal]++;
          break;
        /****************/
        case pagConfMenuExt_part1: //menu EXTRUSORES (parte 1)
          if (_indexes[indiceConfMenuExt] == 3) *_currentScreen = pagConfMenuExt_part2;
          _indexes[indiceConfMenuExt]++;
          break;
        /****************/
        case pagConfMenuExt_part2: //menu EXTRUSORES (parte 2)
          if (_indexes[indiceConfMenuExt] == 7) *_currentScreen = pagConfMenuExt_part3;
          _indexes[indiceConfMenuExt]++;
          break;
        /****************/
        case pagConfMenuExt_part3: //menu EXTRUSORES (parte 3)
          _indexes[indiceConfMenuExt]++;
          if (_indexes[indiceConfMenuExt] >= cantItemsConfMenuExt) _indexes[indiceConfMenuExt] = cantItemsConfMenuExt;
          break;
        /****************/
        case pagConfSubMenuExt:  //sub menu EXTRUSORES
          _indexes[indiceConfSubMenuExt]++;
          if (_indexes[indiceConfSubMenuExt] > cantItemsConfSubMenuExt) _indexes[indiceConfSubMenuExt] = cantItemsConfSubMenuExt;
          break;
        /****************/
        case pagConfSubMenuExtTemp:  //sub menu EXTRUSORES confugracion Tª de trabajo
          //incrementa temp. Si temp > 265 -> temp = 265
          break;
        /****************/
        case pagConfMenuCol_part1:  //menu COLECTORES (parte 1)
          if (_indexes[indiceConfMenuCol] == 3) *_currentScreen = pagConfMenuCol_part2;
          _indexes[indiceConfMenuCol]++;
          break;
        /****************/
        case pagConfMenuCol_part2:  //menu COLECTORES (parte 2)
          if (_indexes[indiceConfMenuCol] == 7) *_currentScreen = pagConfMenuCol_part3;
          _indexes[indiceConfMenuCol]++;
          break;
        /****************/
        case pagConfMenuCol_part3:  //menu COLECTORES (parte 3)
          _indexes[indiceConfMenuCol]++;
          if (_indexes[indiceConfMenuCol] > cantItemsConfMenuCol) _indexes[indiceConfMenuCol] = cantItemsConfMenuCol;
          break;
        /****************/
        case pagCreditos_part1: //menu CREDITOS (parte 1)
          *_currentScreen = pagCreditos_part2;
          break;
        /****************/
        case pagCreditos_part2: //menu CREDITOS (parte 2)

          break;
        /****************/
        default: break;
      }
    }
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
