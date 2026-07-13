#include "joystick.h"
// Ngưỡng phân biệt hướng (ADC 12-bit: 0–4095, giữa ~2048)
#define JOY_THRESHOLD_HIGH  3000  // Vượt ngưỡng này = gạt sang 1 hướng
#define JOY_THRESHOLD_LOW   1000  // Dưới ngưỡng này = gạt sang hướng ngược lại
// Biến nội bộ chống lặp: đảm bảo mỗi lần gạt chỉ tính 1 bước
static JoyDirection lastDir = JOY_NONE;
// Hàm đọc 1 kênh ADC bất kỳ
static uint16_t Read_ADC_Channel(ADC_HandleTypeDef *hadc, uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
    HAL_ADC_ConfigChannel(hadc, &sConfig);
    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, 10);
    uint16_t value = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);
    return value;
}
JoyDirection Joystick_Read(ADC_HandleTypeDef *hadc)
{
    uint16_t x = Read_ADC_Channel(hadc, ADC_CHANNEL_1); // PA1 = trục X
    uint16_t y = Read_ADC_Channel(hadc, ADC_CHANNEL_2); // PA2 = trục Y
    JoyDirection dir = JOY_NONE;
    // Ưu tiên trục có biên độ lệch lớn hơn
    if (x > JOY_THRESHOLD_HIGH)      dir = JOY_RIGHT;
    else if (x < JOY_THRESHOLD_LOW)  dir = JOY_LEFT;
    else if (y > JOY_THRESHOLD_HIGH) dir = JOY_UP;
    else if (y < JOY_THRESHOLD_LOW)  dir = JOY_DOWN;
    // --- Cơ chế Edge Trigger (chống lặp) ---
    // Chỉ trả về hướng khi vừa mới gạt (chuyển từ NONE sang hướng nào đó).
    // Nếu giữ joystick liên tục, chỉ tính 1 lần.
    if (dir != JOY_NONE && lastDir == JOY_NONE) {
        lastDir = dir;
        return dir;  // Hướng mới → trả về
    }
    if (dir == JOY_NONE) {
        lastDir = JOY_NONE;  // Đã thả joystick về giữa → reset
    }
    return JOY_NONE;  // Đang giữ hoặc không gạt → bỏ qua
}
