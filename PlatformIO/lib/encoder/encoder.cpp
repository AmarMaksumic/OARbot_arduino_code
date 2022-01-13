#include <Arduino.h>
#include <SPI.h>
#include "encoder.h"

void encoder::initEncoder() {
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

long encoder::readEncoder() {
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

void encoder::clearEncoderCount() {
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