#include "stm32f10x.h"

// Hàm tạo trễ nhận tham số thời gian trễ (ms tương đối)
void delay_ms(uint32_t t) {
    for (volatile uint32_t i = 0; i < t * 1000; i++) {
        __asm__("nop");
    }
}

int main(void) {
    RCC->APB2ENR |= (1 << 4);    // Clock GPIOC
    GPIOC->CRH &= ~(0xF << 20);  // Xóa cấu hình PC13
    GPIOC->CRH |= (0x2 << 20);   // Output push-pull 2MHz

    uint32_t blink_period = 1000; // Tham số chu kỳ nhấp nháy (thay đổi giá trị này để đổi tốc độ)

    while (1) {
        GPIOC->ODR ^= (1 << 13);
        delay_ms(blink_period);
    }
    return 0;
}

