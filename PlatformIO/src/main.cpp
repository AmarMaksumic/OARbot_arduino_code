/*
   LS7366R Quadrature Counter and Dual Limit Switch Test Code
   AUTHOR: Jason Traud (LS7366R Code) 
            and Amar Maksumic (Porting over encoder code and adding limit switches)
   DATE: January 12th, 2022
   
   This is a simple test program to read encoder counts
   collected by the LS7366 breakout board. The counts are
   then displayed in the Arduino's serial monitor at a 
   baud rate of 9600.
   
   Hardware: Arduino Uno R3
   Powered 
   
   LS7366 Breakout    -------------   Arduino
   -----------------                    -------
            MOSI   -------------------   SDO (D11)
            MISO   -------------------   SDI (D12)
            SCK    -------------------   SCK (D13)
            SS1    -------------------   SS1 (D7)
            SS2    -------------------   SS2 (D8)
            GND    -------------------   GND
            VDD    -------------------   VCC (5.0V)
      
   License: CCAv3.0 Attribution-ShareAlike (http://creativecommons.org/licenses/by-sa/3.0/)
   You're free to use this code for any venture. Attribution is greatly appreciated. 
//============================================================================================
*/

// Include the standard Arduino SPI Library, please ensure the SPI pins are
// connected properly for your Arduino version
#include <Arduino.h>
#include <SPI.h>
#include <encoder.h>

// Input pins for limit switches
#define BOT_LIM_SWITCH 2
#define TOP_LIM_SWITCH 3

// Slave Select pin for the encoder
#define SLAVE_SEL_ENC 7

// These hold the current encoder count.
signed long encoder_one_count = 0;
encoder input_enc;

void setup() {
  Serial.begin(9600);      // Serial com for data output
      
  input_enc = encoder(SLAVE_SEL_ENC);
  pinMode(BOT_LIM_SWITCH, INPUT);
  pinMode(TOP_LIM_SWITCH, INPUT); 
}

void loop() {
  delay(500);      // Ssmall buffer to make reading serial output easier.

  encoder_one_count = input_enc.readEncoder();      // Retrieve current encoder counters

  Serial.print("Enc1: "); Serial.print(encoder_one_count); 

  if (digitalRead(BOT_LIM_SWITCH) == LOW) {
    Serial.println("bottom open");
  } else if (digitalRead(BOT_LIM_SWITCH) == HIGH) {
    Serial.println("bottom closed");
  }

  if (digitalRead(TOP_LIM_SWITCH) == LOW) {
    Serial.println("top open");
  } else if (digitalRead(TOP_LIM_SWITCH) == HIGH) {
    Serial.println("top closed");
  }
}
