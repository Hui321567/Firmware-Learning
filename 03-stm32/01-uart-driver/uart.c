#include "uart.h"
#include <stdio.h>

// 模擬硬體 USART 暫存器實體空間
static USART_TypeDef USART_Mock;
USART_TypeDef *USART_Dev = &USART_Mock;

/**
 * @brief 通用 UART 初始化
 */
void uart_init(uint32_t pclk, uint32_t baudrate) {
    // 1. 設定 Baud Rate
    USART_Dev->BRR = pclk / baudrate;

    // 2. 啟用 TX, RX 與 USART 本體
    USART_Dev->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);

    // 模擬硬體初始狀態：傳送暫存器預設為空 (TXE = 1)
    USART_Dev->SR |= USART_SR_TXE;
}

/**
 * @brief 傳送單一字元 (Polling)
 */
void uart_send_char(char c) {
    // 1. 等待 TXE (Transmit Data Register Empty) 變為 1
    while (!(USART_Dev->SR & USART_SR_TXE)) {
    }

    // 2. 寫入資料到 DR 暫存器
    USART_Dev->DR = (uint32_t)c;

    // 模擬 Console 印出
    putchar(c);
}

/**
 * @brief 接收單一字元 (Polling)
 */
char uart_receive_char(void) {
    // 1. 等待 RXNE (Read Data Register Not Empty) 變為 1
    while (!(USART_Dev->SR & USART_SR_RXNE)) {
    }

    // 2. 從 DR 讀取資料
    return (char)(USART_Dev->DR & 0xFF);
}

/**
 * @brief 傳送字串
 */
void uart_send_string(const char *str) {
    while (*str) {
        uart_send_char(*str++);
    }
}