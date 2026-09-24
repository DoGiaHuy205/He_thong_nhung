#ifndef STM32F10X_H
#define STM32F10X_H

#include <stdint.h>
#define __IO volatile

typedef struct {
    __IO uint32_t CR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t APB2RSTR;
    __IO uint32_t APB1RSTR;
    __IO uint32_t AHBENR;
    __IO uint32_t APB2ENR;
    __IO uint32_t APB1ENR;
    __IO uint32_t BDCR;
    __IO uint32_t CSR;
} RCC_TypeDef;

typedef struct {
    __IO uint32_t CRL;
    __IO uint32_t CRH;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t BRR;
    __IO uint32_t LCKR;
} GPIO_TypeDef;

#define PERIPH_BASE       ((uint32_t)0x40000000)
#define APB2PERIPH_BASE   (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE    (PERIPH_BASE + 0x20000)

#define GPIOA_BASE        (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE        (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE        (APB2PERIPH_BASE + 0x1000)
#define RCC_BASE          (AHBPERIPH_BASE + 0x1000)

#define GPIOA             ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB             ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC             ((GPIO_TypeDef *) GPIOC_BASE)
#define RCC               ((RCC_TypeDef *) RCC_BASE)

#endif
