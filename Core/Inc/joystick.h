#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_
#include "main.h"
// Enum cho 5 trạng thái: 4 hướng + không gạt
typedef enum {
    JOY_NONE = 0,
    JOY_UP,
    JOY_DOWN,
    JOY_LEFT,
    JOY_RIGHT
} JoyDirection;
// Hàm đọc hướng Joystick (gọi mỗi vòng lặp game)
JoyDirection Joystick_Read(ADC_HandleTypeDef *hadc);
#endif
