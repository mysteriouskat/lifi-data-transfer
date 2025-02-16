#include "adc.h"
#include "LCD.h"
#include <stdio.h>
#include <lpc214x.h>

#define BUFFER_SIZE 8 // Adjust buffer size as needed

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

    // Set baud rate to 4800
    U0LCR = 0x83;           // 8-bit character length, enable Divisor Latch Access Bit (DLAB)
    U0DLM = 0x00;           // Set divisor MSB to 0 for baud rate 4800 (PCLK = 12 MHz)
    U0DLL = 0x60;           // Set divisor LSB to 96 for baud rate 4800
    U0LCR = 0x03;           // Disable DLAB and set 8-bit character length
}

// Function to initialize UART1
void initUART1() {
    PINSEL0 |= 0x00050000;  // Enable UART1 Rx and Tx pins

    // Set baud rate to 4800
    U1LCR = 0x83;           // 8-bit character length, enable Divisor Latch Access Bit (DLAB)
    U1DLM = 0x00;           // Set divisor MSB to 0 for baud rate 4800 (PCLK = 12 MHz)
    U1DLL = 0x60;           // Set divisor LSB to 96 for baud rate 4800
    U1LCR = 0x03;           // Disable DLAB and set 8-bit character length
}

// Function to transmit a character through UART0
void sendCharUART0(char data) {
    while (!(U0LSR & 0x20));  // Wait until the Transmit Holding Register (THR) is empty
    U0THR = data;               // Transmit the character
}

int binaryToDecimal(int binary[], int length) {
    int decimal = 0;
    int power = 1; // Initialize power of 2
		int i;
	
    // Traverse the binary array in reverse order
    for (i = length - 1; i >= 0; i--) {
        // Update decimal by adding the current bit multiplied by power of 2
        decimal += binary[i] * power;
        // Update power of 2 for the next bit
        power *= 2;
    }
    
    return decimal;
}

char BinToChar(int binary[]){
		int ascii_value = binaryToDecimal(binary, 8);
		char ascii_char = ascii_value;
	
		return ascii_char;
}

int main() {
    unsigned int temp;
    char buf[16];
		char transmitted_char;
	
    int buffer[BUFFER_SIZE];
    int buffer_index = 0;
	
    LCD_init();
    ADCInit();
    initTimer0(); // Initialize Timer0 for delay
    initUART0(); // Initialize UART0 for transmission
    initUART1(); // Initialize UART1 for reception

    sprintf(buf, "Li-Fi Receiver"); //convert to string and store in a buffer
    LCD_display(2, 1, buf);

    while (1) {
        temp = ADC_Read(1); //read AN0.1

        if (temp > 140) {
            buffer[buffer_index++] = 1; // Store '1' in the buffer
        } else {
            buffer[buffer_index++] = 0; // Store '0' in the buffer
        }

        sprintf(buf, "New:%4u", temp); //convert to string and store in a buffer
        LCD_display(1, 1, buf);                     //display buffer

        // If buffer is full send the buffer through UART0
        if (buffer_index >= BUFFER_SIZE) {
						transmitted_char = BinToChar(buffer); //convert the binary array into ascii character
						sendCharUART0(transmitted_char);
					
            buffer_index = 0; // Reset buffer index after transmission
        }

        delay_us_timer(217); // Adjust the delay according to your microcontroller's execution time
    }
}
