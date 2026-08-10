#ifndef UART_H
#define UART_H

#include <stdint.h>

/* =========================================================================
 * 1. STM32 USART 暫存器結構體映射 (Memory-Mapped Registers)
 * ========================================================================= */
typedef struct {
    volatile uint32_t SR;   // Status Register           (Offset: 0x00)
    volatile uint32_t DR;   // Data Register             (Offset: 0x04)
    volatile uint32_t BRR;  // Baud Rate Register        (Offset: 0x08)
    volatile uint32_t CR1;  // Control Register 1        (Offset: 0x0C)
    volatile uint32_t CR2;  // Control Register 2        (Offset: 0x10)
    volatile uint32_t CR3;  // Control Register 3        (Offset: 0x14)
} USART_TypeDef;

/* =========================================================================
 * 2. 暫存器 Bit Mask 定義
 * ========================================================================= */
// Status Register (SR) Bits
#define USART_SR_TXE   (1U << 7)  // Transmit Data Register Empty
#define USART_SR_RXNE  (1U << 5)  // Read Data Register Not Empty

// Control Register 1 (CR1) Bits
#define USART_CR1_UE   (1U << 13) // USART Enable
#define USART_CR1_TE   (1U << 3)  // Transmitter Enable
#define USART_CR1_RE   (1U << 2)  // Receiver Enable

/* =========================================================================
 * 3. 通用 UART 驅動程式 API 宣告
 * ========================================================================= */
void uart_init(uint32_t pclk, uint32_t baudrate);
void uart_send_char(char c);
char uart_receive_char(void);
void uart_send_string(const char *str);

#endif // UART_H