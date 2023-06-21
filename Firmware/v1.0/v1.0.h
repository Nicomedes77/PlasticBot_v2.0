#ifndef H_V10
#define H_V10

typedef struct PETfilConv{
  //variables de estado
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
}PETfilConv;

#endif // H_V10
