#include <lpc214x.h>

#define P04_OUT (1<<4) // P0.4 as output

void delay(unsigned int count)
{
    unsigned int i;
    for(i=0;i<count;i++);
}

int main()
{
    IO0DIR |= P04_OUT; // Set P0.4 as output

    T0CTCR = 0x00; // Timer Mode: every rising PCLK edge
    T0PR = 0;      // Prescaler value for timer 0
    T0TCR = 0x02;  // Reset Timer 0
    T0TCR = 0x01;  // Enable Timer 0

    while(1)
    {
        IO0SET = P04_OUT; // Set P0.4 high
        delay(10000);     // Adjust delay for frequency
        IO0CLR = P04_OUT; // Clear P0.4 low
        delay(10000);     // Adjust delay for frequency
    }
}
