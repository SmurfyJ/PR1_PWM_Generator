#include <avr/interrupt.h>

uint16_t pwm_frequency  = 500;                   // 250 - 50000
uint8_t  pwm_multiplier = 1;                       // 1 - 2
float    pwm_width      = .85;                      // 0-1.0 %

uint16_t count_top;

int main() {

    DDRB |= (1 << PB1);                             // set D9 (OC1A) to output
    TCCR1A = TCCR1B = 0;                            // reset timer 1

    /* mode: fast PWM, TOP = OCR1A */
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13);

    TCCR1A |= (1 << COM1A1);                        // N-INV
    TCCR1B |= (1 << CS00);                          // Prescale 1

    count_top = (uint16_t) ((16000.0 / pwm_frequency) * 1000) / pwm_multiplier;

    ICR1  = count_top;                              // set top
    OCR1A = (uint16_t) (count_top * pwm_width);     // set puls-width

    while (1) {
       ;
    }

}