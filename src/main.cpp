#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Set the CE & CSN pins
#define CE_PIN   9
#define CSN_PIN  8

// This is the address used to send/receive
const byte rxAddr[6] = "00001";

// Create a Radio
RF24 radio(CE_PIN, CSN_PIN); 
const int SW_pin = 7; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

int padX = 0;
int padY = 0;
int prevPadX = 0;
int prevPadY = 0;

void setup() {
  while (!Serial);
  Serial.begin(9600);
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.setRetries(0, 15);
  radio.openWritingPipe(rxAddr);
  radio.stopListening();
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
}

void getPads(int &padX, int &padY) {
  padX = map(analogRead(X_pin), 0, 1023, 105, -105);
  padY = map(analogRead(Y_pin), 0, 1023, 105, -105);
  padX = (int)(padX / 20) * 20;
  padY = (int)(padY / 20) * 20;
}

void loop() {
  getPads(padX, padY);
  // if (padX != prevPadX || padY != prevPadY) {
    prevPadY = padY;
    prevPadX = padX;

    String str = String(padY) + " " + String(-padX);
    int str_len = str.length() + 1; 
    char char_array[str_len];
    
    str.toCharArray(char_array, str_len);

    radio.write(&char_array, sizeof(char_array));

    Serial.print("Sent Message: ");
    Serial.print( char_array );
    Serial.println("");
  // }
  delay(100);
}