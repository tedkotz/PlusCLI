#include <Arduino.h>
#include "conio.h"

const char ADDR[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
const char DATA[] = {39, 41, 43, 45, 47, 49, 51, 53};
#define CLOCK 20
#define READ_WRITE 21

static void monitor_loop() {
//static void onClock() {
  //char output[15];

  unsigned int address = 0;
  for (int n = 0; n < 16; n += 1) {
    int bit = digitalRead(ADDR[n]) ? 1 : 0;
    Serial.print(bit);
    address = (address << 1) + bit;
  }
  
  Serial.print("   ");
  
  unsigned int data = 0;
  for (int n = 0; n < 8; n += 1) {
    int bit = digitalRead(DATA[n]) ? 1 : 0;
    Serial.print(bit);
    data = (data << 1) + bit;
  }

  printf(F("   %04x  %c %02x\n"), address, digitalRead(READ_WRITE) ? 'r' : 'W', data);
  //sprintf(output, "   %04x  %c %02x", address, digitalRead(READ_WRITE) ? 'r' : 'W', data);
  //Serial.println(output);  
}

//static void monitor_loop() {
static void onClock() {
}

static void monitor_setup() {
  for (int n = 0; n < 16; n += 1) {
    pinMode(ADDR[n], INPUT);
  }
  for (int n = 0; n < 8; n += 1) {
    pinMode(DATA[n], INPUT);
  }
  pinMode(CLOCK, INPUT);
  pinMode(READ_WRITE, INPUT);

  attachInterrupt(digitalPinToInterrupt(CLOCK), onClock, RISING);
  
  //Serial.begin(57600);
}

int monitor_main (int argc, char** argv)
{
  monitor_setup();
  while(!kbhit()) monitor_loop();
  getch();
  return 0;
}
