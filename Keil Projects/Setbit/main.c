#include <lpc214x.h>

int main() {
    // Set the PINSEL register to GPIO mode for pin P0.8
    PINSEL0 &= ~(3 << 16); // Clear bits 16 and 17
    PINSEL0 |= (1 << 16);  // Set bit 16 to 1

    // Set pin P0.8 as output
    IO0DIR |= (1 << 8);

    // Set pin P0.8 HIGH
    IO0SET |= (1 << 8);

    while(1);

}
