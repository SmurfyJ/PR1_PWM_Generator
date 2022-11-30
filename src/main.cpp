#include <avr/interrupt.h>

/**
 * Timer 1 als PWM Ausgang
 * Variable Frequenz -> Fast PWM mit unterschiedlichen Taktteilern und Top?
 * 1Hz - 16Mhz !!
 * Modus Percise: Tastgrad in 1% Schritten -> Output bis 160 kHz (ein PWM Signal = n*100 Zählen)
 *  Von 160kHz       - 1,6kHz   Prescale: 1     (160kHz       - 244Hz)
*                               Von 16kHZ        - 1,6kHz   Prescale: 8     (20kHZ        - 30 Hz)
 *  Von 1,6 kHz      - 160 Hz   Perscale: 64    (2,5 kHz      - 4 Hz )
 *                              Von 160Hz        - 1 Hz     Prescale: 256   (625Hz        - 1 Hz )
 *
 * Modus Fast: Tastgrad in 10% Schritten -> Output bis 1,6 MHz (ein PWM Signal = n*10 Zählen)
 *  Von 1,6MHz -  160kHz Prescale: 1
 *
 *  TOP = CLK/(f_PWM*Prescale)
 *
 * Variabler Tastgrad
 * Gleiche Masse auf beiden Geräten
 */

uint16_t pwm_frequency  = 55;                       // 4-1600
uint8_t  pwm_fast       = 1;                        // 0 = Hz, x = kHz || Ändert prescaler
float    pwm_width      = .25;                      // 0-1.0 %
double   count_top;


int main() {

    DDRB |= (1 << PB1);                             // set D9 (OC1A) to output
    TCCR1A = TCCR1B = 0;                            // reset timer 1

    /* mode: fast PWM, TOP = OCR1A */
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13);

    TCCR1A |= (1 << COM1A1);                        // N-INV
    TCCR1B |= (1 << CS00);                          // Prescale 1

    count_top = 16000.0 / pwm_frequency;

    if (pwm_fast == 0) {
        TCCR1B |= (1 << CS01);                      // Prescale 64
        count_top *= 15.6;
    }

    ICR1    = (uint16_t) count_top;                 // set top
    OCR1A   = (uint16_t) (count_top * pwm_width);   // set puls-width

    while (1) {

    }

}