#ifndef H_V10
#define H_V10

//sensores de tira PET
#define senFil_1   14  //PCINT10
#define senFil_2   15  //PCINT9
#define senFil_3   A8   //PCINT16
#define senFil_4   A9   //PCINT17
#define senFil_5   A10  //PCINT18
#define senFil_6   A11  //PCINT19
#define senFil_7   A12  //PCINT20

//Encoder
#define pin_DT    A13   //PCINT21
#define pin_CLK   A14   //PCINT22
#define pin_SW    A15   //PCINT23

//LCD I2C + buzzer
#define buzzer    16    //PIN = D16
#define SDA       20    //PIN = D20
#define SCL       21    //PIN = D21

//Extrusores
#define ext1_pwm  2        //PIN = D2
#define ext1_adc  A0        //PIN = A0
#define ext2_pwm  3        //PIN = D3
#define ext2_adc  A1        //PIN = A1
#define ext3_pwm  4        //PIN = D4
#define ext3_adc  A2        //PIN = A2
#define ext4_pwm  5        //PIN = D5
#define ext4_adc  A3        //PIN = A3
#define ext5_pwm  6        //PIN = D6
#define ext5_adc  A4        //PIN = A4
#define ext6_pwm  7        //PIN = D7
#define ext6_adc  A5        //PIN = A5
#define ext7_pwm  8        //PIN = D8
#define ext7_adc  A6        //PIN = A6

//colectores
#define col1_pwm  9             //PIN = D9
#define col2_pwm  10            //PIN = D10
#define col3_pwm  11            //PIN = D11
#define col4_pwm  12            //PIN = D12
#define col5_pwm  44            //PIN = D44
#define col6_pwm  45            //PIN = D45
#define col7_pwm  46            //PIN = D46


struct PETfilConv{
  //variables de estado
  bool  petSensor_state;  //determina si hay o no tira PET a procesar
  
  //variables de sensado de temperatura
  double tempActualExtr;    //temperatura actual
  double tempAnteriorExtr;    //temperatura actual
  double constTemp;
  int corrTemp;

  //variables de control PID de tempratura
  double agrKp,agrKi,agrKd;
  double conKp,conKi,conKd;

  //variables control velocidad motor colector
  int velMotor;
  
/*  
  bool  soundAlarm_state;
  bool  velCol_value;
  //variables de temp
  int currentExtTemp;
  int previousSetExtTemp;
  int setExtTemp;
  //variables de control PID
  float PID_p;
  float PID_i;
  float PID_d;
  float PID_error;
  float previous_error;
  float elapsedTime, time, timePrev;
  int factor_cor;
  */
}PETfilConv;

#endif // H_V10
