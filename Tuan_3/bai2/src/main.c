#include "stm32f10x.h"
#include "max7219.h"

// Mảng 8 byte chứa mã hiển thị hình trái tim cho LED Matrix
uint8_t heart_pattern[8] = {
    0x66, // 01100110
    0xFF, // 11111111
    0xFF, // 11111111
    0xFF, // 11111111
    0x7E, // 01111110
    0x3C, // 00111100
    0x18, // 00011000
    0x00  // 00000000
};

// Hàm rỗng để ngăn GCC gọi SystemInit từ system_stm32f10x.c
void SystemInit(void) {
}

// Cấu hình Clock 72MHz sử dụng HSE
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

int main(void) {
    // Cấu hình thạch anh ngoài lên 72MHz
    SystemClock_Config();

    // Khởi tạo SPI và cấu hình MAX7219 cho LED Matrix
    MAX7219_Init();
    
    // Hiển thị hình trái tim
    MAX7219_DisplayPattern(heart_pattern);

    // Vòng lặp chính để trống vì hệ thống chỉ cần hiển thị tĩnh 1 lần
    while(1) {
    }
}
