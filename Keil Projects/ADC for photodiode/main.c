#include "adc.h"
#include "LCD.h"
#include <stdio.h>
#include <lpc214x.h>

// Function to initialize Timer0
void initTimer0() {
    T0CTCR = 0x00;       // Timer Mode: every rising PCLK edge
    T0PR = 11;           // Increment TC at every 12th rising PCLK edge
}

// Function to generate a delay in microseconds using Timer0
void delay_us_timer(unsigned int us) {
    T0TCR = 0x02;        // Reset Timer0
    T0TCR = 0x01;        // Enable Timer0
    while(T0TC < us);    // Wait until TC reaches the desired value
    T0TCR = 0x00;        // Disable Timer0
}

// Function to initialize UART0
void initUART0() {
    PINSEL0 |= 0x00000005;  // Enable UART0 Rx and Tx pins
    U0LCR = 0x83;           // 8-bit character length, enable Divisor Latch Access Bit (DLAB)
    U0DLM = 0x00;           // Set divisor to 96 for baud rate 9600 (PCLK = 12 MHz)
    U0DLL = 0x60;           // Effective baud rate = PCLK / (16 * divisor) = 9600
    U0LCR = 0x03;           // Disable DLAB and set 8-bit character length
}

// Function to initialize UART1
void initUART1() {
    PINSEL0 |= 0x00050000;  // Enable UART1 Rx and Tx pins
    U1LCR = 0x83;           // 8-bit character length, enable Divisor Latch Access Bit (DLAB)
    U1DLM = 0x00;           // Set divisor to 96 for baud rate 9600 (PCLK = 12 MHz)
    U1DLL = 0x60;           // Effective baud rate = PCLK / (16 * divisor) = 9600
    U1LCR = 0x03;           // Disable DLAB and set 8-bit character length
}

// Function to transmit a character through UART0
void sendCharUART0(char ch) {
    while (!(U0LSR & 0x20));  // Wait until the Transmit Holding Register (THR) is empty
    U0THR = ch;               // Transmit the character
}

// Function to receive a character through UART1
char receiveCharUART1() {
    while (!(U1LSR & 0x01));  // Wait until a character is received
    return U1RBR;              // Return the received character
}

int main() {
    unsigned int temp, bit;
    char buf[16];
    char received_char;

    LCD_init();
    ADCInit();
    initTimer0(); // Initialize Timer0 for delay
    initUART0(); // Initialize UART0 for transmission
    initUART1(); // Initialize UART1 for reception

    while (1) {
        // The rest of your code
        temp = ADC_Read(1); //read AN0.1
        IO0DIR |= (1 << 4); // Set P0.4 as output

        if (temp > 140) {
            IO0SET = (1 << 4); // Set bit 4 of Port 0
            bit = 1;
        } else {
            IO0CLR = (1 << 4); // Clear bit 4 of Port 0
            bit = 0;
        }

        sprintf(buf, "Test 16:%4u =%u", temp, bit); //convert to string and store in a buffer
        LCD_display(1, 1, buf);                     //display buffer

				 // Check for received data on UART1 and transmit it through UART0
        if (U1LSR & 0x01) {
            received_char = receiveCharUART1(); // Receive character from UART1
            sendCharUART0(received_char);       // Transmit received character through UART0
        }
				
        delay_us_timer(104); // Adjust the delay according to your microcontroller's execution time
    }
}
