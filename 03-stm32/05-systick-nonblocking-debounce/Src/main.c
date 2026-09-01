#include <stdbool.h>
#include <stdint.h>

#define RCC_AHB1ENR    (*(volatile uint32_t *)0x40023830U)
#define RCC_APB2ENR    (*(volatile uint32_t *)0x40023844U)
#define GPIOA_MODER    (*(volatile uint32_t *)0x40020000U)
#define GPIOA_BSRR     (*(volatile uint32_t *)0x40020018U)
#define GPIOC_MODER    (*(volatile uint32_t *)0x40020800U)
#define GPIOC_IDR      (*(volatile uint32_t *)0x40020810U)
#define SYSCFG_EXTICR4 (*(volatile uint32_t *)0x40013814U)
#define EXTI_IMR       (*(volatile uint32_t *)0x40013C00U)
#define EXTI_FTSR      (*(volatile uint32_t *)0x40013C0CU)
#define EXTI_PR        (*(volatile uint32_t *)0x40013C14U)
#define NVIC_ISER1     (*(volatile uint32_t *)0xE000E104U)
#define SYSTICK_CTRL   (*(volatile uint32_t *)0xE000E010U)
#define SYSTICK_LOAD   (*(volatile uint32_t *)0xE000E014U)
#define SYSTICK_VAL    (*(volatile uint32_t *)0xE000E018U)

#define GPIOA_EN_BIT       0U
#define GPIOC_EN_BIT       2U
#define LED_PIN            5U
#define BUTTON_PIN         13U
#define DEBOUNCE_MS        50U
#define SYSTICK_RELOAD_1MS 15999U

static volatile uint32_t system_ticks = 0U;
static volatile bool button_event = false;

static void gpio_init(void)
{
    RCC_AHB1ENR |= ((1U << GPIOA_EN_BIT) | (1U << GPIOC_EN_BIT));
    GPIOA_MODER &= ~(3U << (LED_PIN * 2U));
    GPIOA_MODER |= (1U << (LED_PIN * 2U));
    GPIOC_MODER &= ~(3U << (BUTTON_PIN * 2U));
}

static void exti_init(void)
{
    RCC_APB2ENR |= (1U << 14U);
    SYSCFG_EXTICR4 &= ~(15U << 4U);
    SYSCFG_EXTICR4 |= (2U << 4U);
    EXTI_IMR |= (1U << BUTTON_PIN);
    EXTI_FTSR |= (1U << BUTTON_PIN);
    NVIC_ISER1 = (1U << 8U);
}

static void systick_init(void)
{
    SYSTICK_CTRL = 0U;
    SYSTICK_LOAD = SYSTICK_RELOAD_1MS;
    SYSTICK_VAL = 0U;
    SYSTICK_CTRL = ((1U << 2U) | (1U << 1U) | (1U << 0U));
}

static inline bool button_is_pressed(void)
{
    return (GPIOC_IDR & (1U << BUTTON_PIN)) == 0U;
}

static inline void led_on(void)
{
    GPIOA_BSRR = (1U << LED_PIN);
}

static inline void led_off(void)
{
    GPIOA_BSRR = (1U << (LED_PIN + 16U));
}

static inline void cpu_wait_for_interrupt(void)
{
    __asm volatile ("wfi");
}

void EXTI15_10_IRQHandler(void)
{
    if ((EXTI_PR & (1U << BUTTON_PIN)) != 0U)
    {
        EXTI_PR = (1U << BUTTON_PIN);
        button_event = true;
    }
}

void SysTick_Handler(void)
{
    system_ticks++;
}

int main(void)
{
    bool led_state = false;
    bool debounce_pending = false;
    uint32_t debounce_start_ticks = 0U;

    gpio_init();
    exti_init();
    systick_init();
    led_off();

    for (;;)
    {
        if (button_event)
        {
            button_event = false;

            if (!debounce_pending)
            {
                debounce_pending = true;
                debounce_start_ticks = system_ticks;
            }
        }

        if (debounce_pending &&
            ((system_ticks - debounce_start_ticks) >= DEBOUNCE_MS))
        {
            debounce_pending = false;

            if (button_is_pressed())
            {
                led_state = !led_state;
            }
        }

        if (led_state)
        {
            led_on();
        }
        else
        {
            led_off();
        }

        cpu_wait_for_interrupt();
    }
}
