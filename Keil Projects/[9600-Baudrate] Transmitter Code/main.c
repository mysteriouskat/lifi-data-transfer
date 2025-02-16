#include "LCD.h"
#include <lpc214x.h>
#include<stdio.h>
#define PCLK (12000000) // Assuming PCLK is 12MHz

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

// Function to generate a delay in milliseconds
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 8000; j++); // Adjust this loop for 1ms delay at 12 MHz PCLK
    }
}

int main() {
    char buf[16];
    
    uart0_init(); // Initialize UART0
    LCD_init();
    
    sprintf(buf, "Li-Fi Transmit"); //convert to string and store in a buffer
    LCD_display(2, 1, buf);                     //display buffer
    
    while (1) {
        
        sprintf(buf, "Sending.   "); //convert to string and store in a buffer
        LCD_display(1, 1, buf);                     //display buffer
        delay_ms(200); // Delay for 100 milliseconds
        
        sprintf(buf, "Sending..  "); //convert to string and store in a buffer
        LCD_display(1, 1, buf);                     //display buffer
        delay_ms(200); // Delay for 100 milliseconds
        
        sprintf(buf, "Sending... "); //convert to string and store in a buffer
        LCD_display(1, 1, buf);                     //display buffer
        delay_ms(200); // Delay for 100 milliseconds

        UART_SendString("Hello, World! \n");
        delay_ms(200); // Delay for 100 milliseconds

    }
}
