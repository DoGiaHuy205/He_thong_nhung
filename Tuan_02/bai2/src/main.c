#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// Hàm rỗng chặn GCC gọi thư viện khởi động lỗi
void SystemInit(void) {}

// Khởi tạo thạch anh ngoài lên 72MHz (chuẩn của bạn)
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

// Cấu hình 3 chân GPIO cho 3 LED
void LED_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // Cấp xung clock cho Port A
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // Cấu hình PA1, PA2, PA3 ở chế độ Output Push-Pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// Trình phục vụ ngắt SysTick (Tự động chạy mỗi 1ms)
void SysTick_Handler(void) {
    // Biến static giữ nguyên giá trị sau mỗi lần thoát ngắt
    static uint32_t count_10Hz = 0;
    static uint32_t count_1Hz = 0;
    static uint32_t count_0_1Hz = 0;

    // Xử lý LED 1 (10Hz -> Đảo trạng thái mỗi 50ms)
    if (++count_10Hz >= 50) {
        count_10Hz = 0;
        GPIOA->ODR ^= GPIO_Pin_1; // Phép XOR để lật bit (chớp/tắt)
    }

    // Xử lý LED 2 (1Hz -> Đảo trạng thái mỗi 500ms)
    if (++count_1Hz >= 500) {
        count_1Hz = 0;
        GPIOA->ODR ^= GPIO_Pin_2;
    }

    // Xử lý LED 3 (0.1Hz -> Đảo trạng thái mỗi 5000ms)
    if (++count_0_1Hz >= 5000) {
        count_0_1Hz = 0;
        GPIOA->ODR ^= GPIO_Pin_3;
    }
}

int main(void) {
    // 1. Cấu hình xung nhịp hệ thống 72MHz
    SystemClock_Config();
    
    // 2. Cấu hình các chân LED
    LED_Config();
    
    // 3. Cấu hình SysTick tạo ngắt mỗi 1ms
    // Do System Clock = 72MHz (72.000.000 chu kỳ/giây)
    // -> Cần 72.000 chu kỳ để đạt 1ms
    SysTick_Config(72000);
    
    while(1) {
        // Vi điều khiển không cần làm gì trong vòng lặp chính
        // Hàm __WFI() (Wait For Interrupt) đưa chip vào trạng thái ngủ đông
        // để tiết kiệm điện, nó chỉ thức dậy khi có ngắt SysTick đánh thức.
        __WFI();
    }
}
