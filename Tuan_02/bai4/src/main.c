#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

void SystemInit(void) {}

// Cấu hình Clock 72MHz bằng thạch anh ngoài
void SystemClock_Config(void) {
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    FLASH->ACR |= FLASH_ACR_PRFTBE; 
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_2; 
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; 
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; 
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
    RCC->CFGR |= (RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void PWM_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 1. Cấp clock cho GPIOA và Timer 2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 2. Cấu hình PA0, PA1, PA2, PA3 ở chế độ Alternate Function Push-Pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. Cấu hình Timer 2 với tần số PWM = 1KHz
    // Tần số xung nhịp TIM2 = 72MHz.
    // Timer đếm 100 bước (từ 0 đến 99) rồi lặp lại.
    TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1; 
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;    
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 4. Khởi tạo cấu hình kênh xuất xung PWM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // ---- Kênh 1 (PA0): Độ sáng 10% ----
    TIM_OCInitStructure.TIM_Pulse = 10;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // ---- Kênh 2 (PA1): Độ sáng 30% ----
    TIM_OCInitStructure.TIM_Pulse = 30;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // ---- Kênh 3 (PA2): Độ sáng 50% ----
    TIM_OCInitStructure.TIM_Pulse = 50;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // ---- Kênh 4 (PA3): Độ sáng 70% ----
    TIM_OCInitStructure.TIM_Pulse = 70;
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // 5. Kích hoạt Timer 2
    TIM_Cmd(TIM2, ENABLE);
}

int main(void) {
    SystemClock_Config();
    PWM_Config();

    while(1) {
        // Chip ngủ đông để tiết kiệm điện, phần cứng Timer tự động chạy độc lập
        __WFI(); 
    }
}
