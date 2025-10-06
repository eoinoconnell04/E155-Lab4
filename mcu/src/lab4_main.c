/*
Author: Eoin O'Connell
Email: eoconnell@hmc.edu
Date: Sep. 28, 2025
File function: Play Fur Elise by toggeling GPIO pin at different frequencies for different durations

Adapted from lab4_starter.c from E155 Lab 4
*/


// Include necessary header files:

#include "STM32L432KC_FLASH.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_RCC.h"
#include "STM32L432KC_TIM.h"

// I will be using pin 6
#define PIN 6


// Pitch in Hz, duration in ms
const int fe_notes[][2] = {
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	250},
{  0,	125},
{494,	125},
{523,	125},
{587,	125},
{659,	375},
{392,	125},
{699,	125},
{659,	125},
{587,	375},
{349,	125},
{659,	125},
{587,	125},
{523,	375},
{330,	125},
{587,	125},
{523,	125},
{494,	250},
{  0,	125},
{330,	125},
{659,	125},
{  0,	250},
{659,	125},
{1319,	125},
{  0,	250},
{623,	125},
{659,	125},
{  0,	250},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	500},
{  0,	0}};

// Happy Birthday Notes
// I wrote this in the key of C and used the A4 = 440
// Pitch in Hz, duration in ms
const int hb_notes[][2] = {
    {261.6, 100},  // C4 - quarter
    {0, 25},       // rest to be able to hear repeated note
    {261.6, 125},  // C4 - quarter
    {293.7, 250},  // D4 - half
    {261.6, 250},  // C4 - half
    {349.2, 250},  // F4 - half
    {329.6, 500}, // E4 - whole

    {261.6, 100},  // C4 - quarter
    {0, 25},       // rest to be able to hear repeated note
    {261.6, 125},  // C4 - quarter
    {293.7, 250},  // D4 - half
    {261.6, 250},  // C4 - half
    {392.0, 250},  // G4 - half
    {349.2, 500}, // F4 - whole

    {261.6, 100},  // C4 - quarter
    {0, 25},       // rest to be able to hear repeated note
    {261.6, 125},  // C4 - quarter
    {523.3, 250},  // C5 - half
    {440.0, 250},  // A4 - half
    {349.2, 250},  // F4 - half
    {329.6, 250},  // E4 - half
    {293.7, 500}, // D4 - whole

    {466.2, 125},  // Bb4 - quarter
    {0, 25},       // rest to be able to hear repeated note
    {466.2, 125},  // Bb4 - quarter
    {440.0, 250},  // A4 - half
    {349.2, 250},  // F4 - half
    {392.0, 250},  // G4 - half
    {349.2, 500}, // F4 - whole

    {0, 0}         // End
};

void playTone(int freq, int dur) {
    setFreq(freq);
    setDur(dur);
}

int main(void) {
	// configure flash
    configureFlash();

    // configure clock (configureClock also configures the PLL)
    // clock running at 80 MHz
    configureClock();

    // configure GPIO
    RCC->AHB2ENR |= (1 << 0); // turn GPIOA clock
    RCC->APB2ENR |= (1 << 16); // enable TIM15
    RCC->APB2ENR |= (1 << 17); // enable TIM16

    // use pin 6
    pinMode(6, GPIO_ALT); // set pin 6 to alt function
    GPIO->AFRL &= ~(0b1111<<4*6);
    GPIO->AFRL |= (14<<4*6); // set pin 6 to alt func 14

    // Configure TIM15 and TIM16
    // TIM15 will handle note duration
    // TIM16 will handle note frequency
    configureTIM15();
    configureTIM16(); ;

    // For testing frequency:
    /*
    setFreq(200);
    while(1){
    }
    */

    // For testing duration:
    /*
    while(1){
        setFreq(0);
        setDur(200);
        setFreq(200);
        setDur(200);
    }
    */

    // play Fur Elise and Happy Birthday Back to Back (keep alternating)
    
    while (1) {
        // play Fur Elise
        for (int i = 0; (fe_notes[i][0] != 0 || fe_notes[i][1] != 0); i++) {
            int freq = fe_notes[i][0];
            int dur  = fe_notes[i][1];

            playTone(freq, dur);
        }

        // play happy birthday
        for (int i = 0; (hb_notes[i][0] != 0 || hb_notes[i][1] != 0); i++) {
            int freq = hb_notes[i][0];
            int dur  = hb_notes[i][1];

            playTone(freq, dur);
        }
    }
	
}