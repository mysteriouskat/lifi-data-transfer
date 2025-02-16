#include <lpc214x.h>

#define PCLK (15000000) // Assuming PCLK is 15MHz

void uart0_init(void) {
    PINSEL0 |= 0x00000005; // Enable TXD0 and RXD0
    U0LCR = 0x83; // 8-bit character length, 1 stop bit, DLAB = 1
    U0DLM = 0; // Set DLM to 0 for baud rate 9600
    U0DLL = 97; // Set DLL to 97 for baud rate 9600
    U0LCR = 0x03; // DLAB = 0
}

void uart0_send(char data) {
    while (!(U0LSR & 0x20)); // Wait until THR is empty
    U0THR = data; // Load data into THR
}

void UART_SendString(const char *str) {
    while (*str) {
        uart0_send(*str++);
    }
}

int main() {
    uart0_init(); // Initialize  UART0
    // Send "Hello, World!" in binary
	UART_SendString("Hello, World!");
}
