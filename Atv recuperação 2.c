#include <stdint.h>
#include <stdlib.h>

#define STM32_RCC_BASE       0x40023800
#define STM32_GPIOA_BASE     0x40020000

#define STM32_RCC_AHB1ENR_OFFSET  0x0030

#define STM32_GPIO_MODER_OFFSET   0x0000
#define STM32_GPIO_IDR_OFFSET     0x0010
#define STM32_GPIO_BSRR_OFFSET    0x0018

#define STM32_RCC_AHB1ENR        (*(volatile uint32_t *)(STM32_RCC_BASE + STM32_RCC_AHB1ENR_OFFSET))
#define STM32_GPIOA_MODER        (*(volatile uint32_t *)(STM32_GPIOA_BASE + STM32_GPIO_MODER_OFFSET))
#define STM32_GPIOA_IDR          (*(volatile uint32_t *)(STM32_GPIOA_BASE + STM32_GPIO_IDR_OFFSET))
#define STM32_GPIOA_BSRR         (*(volatile uint32_t *)(STM32_GPIOA_BASE + STM32_GPIO_BSRR_OFFSET))

#define RCC_AHB1ENR_GPIOAEN      (1 << 0)

#define GPIO_MODER_INPUT           (0)
#define GPIO_BSRR_SET(n)           (1 << (n))
#define GPIO_BSRR_RESET(n)         (1 << ((n) + 16))

#define LED_DELAY  100000

int main(int argc, char *argv[])
{
    uint32_t i;
    uint32_t reg;

    uint32_t *pRCC_AHB1ENR  = &STM32_RCC_AHB1ENR;
    uint32_t *pGPIOA_MODER  = &STM32_GPIOA_MODER;
    uint32_t *pGPIOA_IDR    = &STM32_GPIOA_IDR;
    uint32_t *pGPIOA_BSRR   = &STM32_GPIOA_BSRR;

    reg  = *pRCC_AHB1ENR;
    reg |= RCC_AHB1ENR_GPIOAEN;
    *pRCC_AHB1ENR = reg;

    reg = *pGPIOA_MODER;
    reg &= ~(3 << 0); // Clear mode bits for PA0
    *pGPIOA_MODER = reg;

    reg = *pGPIOA_BSRR;
    reg |= GPIO_BSRR_SET(0); // Set PA0 (input pin) to high (pull-up)
    *pGPIOA_BSRR = reg;

    while (1)
    {
        if (!(*pGPIOA_IDR & (1 << 0))) // If PA0 (button) is pressed
        {
            *pGPIOA_BSRR = GPIO_BSRR_RESET(13); // Turn on LED
        }
        else
        {
            *pGPIOA_BSRR = GPIO_BSRR_SET(13); // Turn off LED
        }

        for (i = 0; i < LED_DELAY; i++); // Delay
    }

    return EXIT_FAILURE;
}
