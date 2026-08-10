#include "uart.h"
#include <stdio.h>

int main(void) {
    printf("=== STM32 Bare-Metal UART Driver Test ===\n\n");

    // 以 16MHz 時脈初始化 UART 為 115200 Baud Rate
    uart_init(16000000, 115200);

    // 測試字串傳送
    uart_send_string("Hello, Bare-Metal STM32 UART Driver!\r\n");
    uart_send_string("Driver Initialization Successful.\r\n");

    return 0;
}