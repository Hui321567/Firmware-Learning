#include <stdint.h>

/* Forward Declarations */
void Reset_Handler(void);
int main(void);

/* Linker Script 匯入符號（記憶體邊界位址） */
extern uint32_t _estack;   // SRAM 頂部位址 (Initial Stack Pointer)
extern uint32_t _sidata;   // FLASH 裡 .data 初始值的起點 (LMA)
extern uint32_t _sdata;    // SRAM 裡 .data 的起點 (VMA)
extern uint32_t _edata;    // SRAM 裡 .data 的終點 (VMA)
extern uint32_t _sbss;     // SRAM 裡 .bss 的起點
extern uint32_t _ebss;     // SRAM 裡 .bss 的終點

/* Minimal ARM Cortex-M4 Vector Table
   放在 .isr_vector 區段，Linker Script 會將它排在 Flash 最開頭 (0x08000000) */
__attribute__((section(".isr_vector")))
uint32_t *vector_table[] = {
    (uint32_t *)&_estack,       // 1. Initial Main Stack Pointer (MSP)
    (uint32_t *)Reset_Handler   // 2. Reset Vector (通電第一條執行指令)
};

/* Hardware Reset Handler
   MCU 通電後執行的第一段軟體程式碼，負責建立完整 C 語言執行期環境 (C Runtime Environment) */
void Reset_Handler(void) {
    /* 1. 將 .data 區段（已初始化的全域變數）從 FLASH 拷貝至 SRAM */
    uint32_t *pSource = &_sidata;
    uint32_t *pDest   = &_sdata;

    while (pDest < &_edata) {
        *pDest++ = *pSource++;
    }

    /* 2. 將 .bss 區段（未初始化的全域變數）在 SRAM 中全部清零 */
    uint32_t *pBss = &_sbss;
    while (pBss < &_ebss) {
        *pBss++ = 0;
    }

    /* 3. SRAM 記憶體環境配置完成，跳轉至 main() */
    main();

    /* 4. 避免 main() 意外結束時 CPU 跑飛 */
    while (1);
}

/* STM32F401RE Physical Memory Base Addresses */
#define PERIPH_BASE           (0x40000000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)

/* RCC (Reset and Clock Control) Register Map */
#define RCC_BASE              (AHB1PERIPH_BASE + 0x00003800UL)

/* GPIO Port A Register Map (User LED LD2 is on PA5 on NUCLEO-F401RE) */
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x00000000UL)

/* Hardware Register Structures */
typedef struct {
    volatile uint32_t CR;            // Clock control register
    volatile uint32_t PLLCFGR;       // PLL configuration register
    volatile uint32_t CFGR;          // Clock configuration register
    volatile uint32_t CIR;           // Clock interrupt register
    volatile uint32_t AHB1RSTR;      // AHB1 peripheral reset register
    volatile uint32_t AHB2RSTR;      // AHB2 peripheral reset register
    uint32_t          RESERVED0[2];
    volatile uint32_t AHB1ENR;       // AHB1 peripheral clock enable register
} RCC_TypeDef;

typedef struct {
    volatile uint32_t MODER;         // Mode register
    volatile uint32_t OTYPER;        // Output type register
    volatile uint32_t OSPEEDR;       // Output speed register
    volatile uint32_t PUPDR;         // Pull-up/pull-down register
    volatile uint32_t IDR;           // Input data register
    volatile uint32_t ODR;           // Output data register
    volatile uint32_t BSRR;          // Bit set/reset register
    volatile uint32_t LCKR;          // Configuration lock register
    volatile uint32_t AFR[2];        // Alternate function registers
} GPIO_TypeDef;

/* Peripheral Pointer Definitions */
#define RCC                   ((RCC_TypeDef *) RCC_BASE)
#define GPIOA                 ((GPIO_TypeDef *) GPIOA_BASE)

/* Bit Definitions */
#define RCC_AHB1ENR_GPIOAEN   (1U << 0)   // Enable GPIOA clock

void delay_cycles(volatile uint32_t count) {
    while (count--) {
        __asm__("nop"); // Execute No-Operation assembly instruction
    }
}

int main(void) {
    /* 1. Enable GPIOA peripheral clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* 2. Configure PA5 as General Purpose Output Mode
          MODER5[1:0] = 01 (General purpose output mode)
          Clear bit 11, Set bit 10 */
    GPIOA->MODER &= ~(3U << (5 * 2)); // Clear MODER5 bits [11:10]
    GPIOA->MODER |=  (1U << (5 * 2)); // Set bit 10 (01)

    /* 3. Infinite loop: Toggle PA5 LED */
    while (1) {
        GPIOA->ODR ^= (1U << 5);     // Toggle PA5 output pin
        delay_cycles(500000);        // Software delay
    }

    return 0;
}

