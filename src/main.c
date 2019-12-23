#include <xc.h>
#include <stdint.h>

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Detect (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

void __interrupt() isr(void) {
  TMR1H = 0x0b;
  TMR1L = 0xdc;
  GP0   = ~GP0;
  T1IF  = 0;
}

void main(void) {
  OSCCON = 0x60;
  GPIO   = 0x00;
  TRISIO = 0x08;
  ANSEL  = 0x00;
  CMCON0 = 0x07;
  WPU    = 0x00;
  nGPPU  = 1;

  T1CON = 0x30;
  TMR1H = 0x0b;
  TMR1L = 0xdc;

  TMR1IE = 1;
  PEIE   = 1;
  GIE    = 1;

  TMR1ON = 0;

  while(1) {
    if (GP3) {
      TMR1ON == 0 ? GP0 = 1 : 0;
    } else {
      TMR1ON = TMR1ON ^ 1;
      while (GP3 == 0);
      __delay_ms(50);
    }
  }
}
