#ifndef H_PET_ALARMS
#define H_PET_ALARMS

#include "v1.0.h"

void ControlAlarms(bool *_sensorFil , PETfilConv *_petFilConv);
void ActivaAlarmaLuminica(void);
void ActivaAlarmaSonora(void);
void DesactivaAlarmas(void);

#endif // H_PET_ALARMS
