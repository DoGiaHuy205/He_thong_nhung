#include "stm32f10x.h"

void delay_ms(uint32_t t) {
    for (volatile uint32_t i = 0; i < t * 1000; i++) {
        __asm__("nop");
    }
}

int main(void) {
    RCC->APB2ENR |= (1 << 2);    // Bật clock GPIOA
    GPIOA->CRL = 0x22222222;     // Cấu hình PA0 - PA7 là Output Push-Pull 2MHz

    while (1) {
        // Chạy từ trái sang phải (PA0 -> PA7)
        for (int i = 0; i < 8; i++) {
            GPIOA->ODR = (GPIOA->ODR & 0xFF00) | (1 << i);
            delay_ms(150);
        }
        // Đảo chiều chạy từ phải sang trái (PA6 -> PA1)
        for (int i = 6; i > 0; i--) {
            GPIOA->ODR = (GPIOA->ODR & 0xFF00) | (1 << i);
            delay_ms(150);
        }
    }
    return 0;
}

