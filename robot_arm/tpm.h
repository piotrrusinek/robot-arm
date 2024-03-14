#ifndef TPM_H
#define TPM_H

#include "frdm_bsp.h"
#include "led.h"


void TPM1_Init_InputCapture(void);

void TPM0_Init_PWM(void);


uint32_t TPM1_GetVal(void);

void TPM0_SetVal_X(uint32_t value);
void TPM0_SetVal_Y(uint32_t value);
void TPM0_SetVal_Z(uint32_t value);
void TPM0_SetVal_G(uint32_t value);

#endif /* TPM_H */
