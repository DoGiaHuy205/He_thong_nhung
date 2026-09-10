#include "stm32f10x.h"

int main(void) {
    // 1. Bật xung nhịp clock cho GPIOA và GPIOB
    RCC->APB2ENR |= (1 << 2) | (1 << 3);

    // 2. Cấu hình PA0 - PA7 làm Input Pull-up
    GPIOA->CRL = 0x88888888;
    GPIOA->ODR |= 0x00FF; // Kéo điện trở Pull-up lên mức 1

    // 3. Cấu hình PA8 - PA12 làm Output Push-Pull 2MHz
    GPIOA->CRH &= ~0x000FFFFF;
    GPIOA->CRH |=  0x00022222;

    // 4. Cấu hình PB13 - PB15 làm Output Push-Pull 2MHz
    // Vi chan PA13 va chan PA14 la cac chan SWDIO va SWCLK
    // chan PA15 la chan JTAG
    // nen em xin phep doi sang chan PB13 va PB14 va PB15
    GPIOB->CRH &= ~0xFFF00000;
    GPIOB->CRH |=  0x22200000;

    while (1) {
        // Đọc 8 bit từ PA0 -> PA7
        uint8_t in_val = (uint8_t)(GPIOA->IDR & 0x00FF);

        // Đảo toàn bộ bit (0 -> 1, 1 -> 0)[cite: 1]
        uint8_t out_val = ~in_val;

        // Tách bit 0..2 đẩy vào PB13..PB15
        // (out_val & 0x07) dịch sang trái 13 bit để vào đúng vị trí bit 13, 14, 15
        uint32_t b_pins = ((uint32_t)(out_val & 0x07)) << 13;
        GPIOB->ODR = (GPIOB->ODR & ~(0x7 << 13)) | b_pins;

        // Tách bit 3..7 đẩy vào PA8..PA12
        // Dịch sang phải 3 bit rồi dịch sang trái 8 bit (tương đương dịch trái 5 bit)
        uint32_t a_pins = ((uint32_t)((out_val >> 3) & 0x1F)) << 8;
        GPIOA->ODR = (GPIOA->ODR & ~(0x1F << 8)) | a_pins;
    }

    return 0;
}

