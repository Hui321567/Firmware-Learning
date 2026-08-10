#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

/* 模擬 STM32 UART 暫存器結構體 */
typedef struct {
    volatile uint32_t SR;   /* Status Register (狀態暫存器) */
    volatile uint32_t DR;   /* Data Register (資料暫存器) */
    volatile uint32_t BRR;  /* Baud Rate Register (波特率設定暫存器) */
    volatile uint32_t CR1;  /* Control Register 1 (控制暫存器 1) */
} UART_TypeDef;

/* Status Register (SR) Bit Definitions */
#define UART_SR_TXE   (1 << 7)  /* Transmit Data Register Empty (1: 可寫入新資料) */
#define UART_SR_RXNE  (1 << 5)  /* Read Data Register Not Empty (1: 有新資料可讀取) */

/* Control Register 1 (CR1) Bit Definitions */
#define UART_CR1_UE   (1 << 13) /* USART Enable (1: 啟用 UART) */
#define UART_CR1_TE   (1 << 3)  /* Transmitter Enable (1: 啟用傳送) */
#define UART_CR1_RE   (1 << 2)  /* Receiver Enable (1: 啟用接收) */

/* API Function Declarations */
void uart_init(UART_TypeDef *uart, uint32_t baudrate);
void uart_send_char(UART_TypeDef *uart, char ch);
void uart_send_string(UART_TypeDef *uart, const char *str);
char uart_receive_char(UART_TypeDef *uart);

#endif // UART_H