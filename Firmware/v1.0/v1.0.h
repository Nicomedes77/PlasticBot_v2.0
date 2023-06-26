#ifndef H_V10
#define H_V10




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
