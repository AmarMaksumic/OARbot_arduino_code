
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
#include <SPI.h>

// Input pins for limit switches
#define BOT_LIM_SWITCH 2 
#define TOP_LIM_SWITCH 2

// Slave Select pin for the encoder
#define SLAVE_SEL_ENC 7

// These hold the current encoder count.
signed long encoder_one_count = 0;

void initEncoders() {
  
  // Set slave selects as outputs
  pinMode(SLAVE_SEL_ENC, OUTPUT);
  
  // Raise select pins
  // Communication begins when you drop the individual select signsl
  digitalWrite(SLAVE_SEL_ENC,HIGH);
  
  SPI.begin();
  
  // Initialize encoder 1
  //    Clock division factor: 0
  //    Negative index input
  //    free-running count mode
  //    x4 quatrature count mode (four counts per quadrature cycle)
  // NOTE: For more information on commands, see datasheet
  digitalWrite(SLAVE_SEL_ENC,LOW);        // Begin SPI conversation
  SPI.transfer(0x88);                       // Write to MDR0
  SPI.transfer(0x03);                       // Configure to 4 byte mode
  digitalWrite(SLAVE_SEL_ENC,HIGH);       // Terminate SPI conversation 
}

long readEncoder() {
  
  // Initialize temporary variables for SPI read
  unsigned int count_1, count_2, count_3, count_4;
  long count_value;  
  
  digitalWrite(SLAVE_SEL_ENC,LOW);      // Begin SPI conversation
  SPI.transfer(0x60);                     // Request count
  count_1 = SPI.transfer(0x00);           // Read highest order byte
  count_2 = SPI.transfer(0x00);           
  count_3 = SPI.transfer(0x00);           
  count_4 = SPI.transfer(0x00);           // Read lowest order byte
  digitalWrite(SLAVE_SEL_ENC,HIGH);     // Terminate SPI conversation 
  
  // Calculate encoder count
  count_value = (count_1 << 8) + count_2;
  count_value = (count_value << 8) + count_3;
  count_value = (count_value << 8) + count_4;
  
  return count_value;
}

void clearEncoderCount() {
    
  // Set encoder1's data register to 0
  digitalWrite(SLAVE_SEL_ENC,LOW);      // Begin SPI conversation  
  // Write to DTR
  SPI.transfer(0x98);    
  // Load data
  SPI.transfer(0x00);  // Highest order byte
  SPI.transfer(0x00);           
  SPI.transfer(0x00);           
  SPI.transfer(0x00);  // lowest order byte
  digitalWrite(SLAVE_SEL_ENC,HIGH);     // Terminate SPI conversation 
  
  delayMicroseconds(100);  // provides some breathing room between SPI conversations
  
  // Set encoder1's current data register to center
  digitalWrite(SLAVE_SEL_ENC,LOW);      // Begin SPI conversation  
  SPI.transfer(0xE0);    
  digitalWrite(SLAVE_SEL_ENC,HIGH);     // Terminate SPI conversation   
}


void setup() {
  Serial.begin(9600);      // Serial com for data output
  
  initEncoders();       
  Serial.println("Encoders Initialized...");  
  clearEncoderCount();  
  Serial.println("Encoders Cleared...");
  pinMode(BOT_LIM_SWITCH, INPUT);
  pinMode(TOP_LIM_SWITCH, INPUT); 
}

void loop() {
  delay(500);      // Ssmall buffer to make reading serial output easier.

  encoder_one_count = readEncoder();      // Retrieve current encoder counters

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
