#include "stm32f10x.h"

void delay_ms(uint16_t t) {
    for (volatile uint32_t i = 0; i < (uint32_t)t * 1000; i++) {
        __asm__("nop");
    }
}

int main(void) {
    RCC->APB2ENR |= 0xFC;        // Bật clock tất cả GPIO
    GPIOC->CRH = 0x44344444;     // PC13 là General purpose output push-pull, 50MHz

    while(1) {
        GPIOC->ODR ^= (1 << 13); // Đảo trạng thái PC13 (chớp tắt LED)
        delay_ms(1000);
    }
    return 0;
}
