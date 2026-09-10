#include "stm32f10x.h"

void delay_ms(uint32_t t) {
    for (volatile uint32_t i = 0; i < t * 1000; i++) {
        __asm__("nop");
    }
}

int main(void) {
    // Bật clock cho GPIOA (nút nhấn) và GPIOC (LED PC13)
    RCC->APB2ENR |= (1 << 2) | (1 << 4);

    // PC13 là Output Push-Pull 2MHz
    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |= (0x2 << 20);
    GPIOC->ODR |= (1 << 13); // Tắt LED ban đầu (LED PC13 sáng mức 0, tắt mức 1)

    // PA0 là Input Pull-up
    GPIOA->CRL &= ~(0xF << 0);
    GPIOA->CRL |= (0x8 << 0);
    GPIOA->ODR |= (1 << 0);  // Kéo lên mức 1

    uint8_t last_btn_state = 1;

    while (1) {
        uint8_t current_btn_state = (GPIOA->IDR & (1 << 0)) ? 1 : 0;

        // Phát hiện chuyển trạng thái từ 0 (đang nhấn) sang 1 (vừa nhả ra)
        if (last_btn_state == 0 && current_btn_state == 1) {
            delay_ms(20); // Chống rung phím (debounce)
            GPIOC->ODR ^= (1 << 13); // Đảo trạng thái LED
        }

        last_btn_state = current_btn_state;
        delay_ms(10);
    }
    return 0;
}

