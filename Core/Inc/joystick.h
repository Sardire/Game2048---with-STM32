#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_
#include "main.h"
typedef enum {
    JOY_NONE  = 0,
    JOY_UP,
    JOY_DOWN,
    JOY_LEFT,
    JOY_RIGHT
} JoyDirection;
void Joystick_Calibrate(void);
void Joystick_GetRaw(uint16_t *x, uint16_t *y);
JoyDirection Joystick_Read(void);
#endif /* INC_JOYSTICK_H_ */
