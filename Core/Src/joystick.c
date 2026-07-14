#include "joystick.h"

#define DEADZONE  800   /* Ngưỡng lệch để tính là gạt */

static uint16_t s_center_x = 2048;
static uint16_t s_center_y = 2048;
static JoyDirection s_last = JOY_NONE;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

/* ADC1: PA1 (IN1) = trục X,  ADC2: PC3 (IN13) = trục Y */
static uint16_t read_adc(ADC_HandleTypeDef *hadc)
{
    HAL_ADC_Start(hadc);
    
    // Chờ chuyển đổi hoàn tất với timeout 100ms
    if (HAL_ADC_PollForConversion(hadc, 100) != HAL_OK)
    {
        // Nếu có lỗi, dừng ADC và trả về giá trị trung tâm để tránh hành vi lạ
        HAL_ADC_Stop(hadc);
        return 2048;
    }

    uint16_t v = (uint16_t)HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);
    return v;
}

void Joystick_GetRaw(uint16_t *x, uint16_t *y)
{
    // Các kênh ADC đã được cấu hình sẵn trong MX_ADC1_Init và MX_ADC2_Init
    *y = read_adc(&hadc2);   /* PC3  */
    *x = read_adc(&hadc1);   /* PA1  */
}

void Joystick_Calibrate(void)
{
    uint32_t sum_x = 0, sum_y = 0;
    uint16_t x, y;

    for (int i = 0; i < 3; i++) {
        Joystick_GetRaw(&x, &y);
        HAL_Delay(5);
    }

    for (int i = 0; i < 8; i++) {
        Joystick_GetRaw(&x, &y);
        sum_x += x;
        sum_y += y;
        HAL_Delay(5);
    }
    s_center_x = sum_x / 8;
    s_center_y = sum_y / 8;
}

JoyDirection Joystick_Read(void)
{
    uint16_t x, y;
    Joystick_GetRaw(&x, &y);

    int32_t dx = (int32_t)x - s_center_x;
    int32_t dy = (int32_t)y - s_center_y;

    JoyDirection dir = JOY_NONE;

    if (dx > DEADZONE)       dir = JOY_RIGHT;
    else if (dx < -DEADZONE) dir = JOY_LEFT;
    else if (dy > DEADZONE)  dir = JOY_DOWN;
    else if (dy < -DEADZONE) dir = JOY_UP;

    if (dir != JOY_NONE && s_last == JOY_NONE) { s_last = dir; return dir; }
    if (dir == JOY_NONE) s_last = JOY_NONE;
    return JOY_NONE;
}
