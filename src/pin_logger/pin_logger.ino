#include <SPI.h>
#include <SD.h>

#define PIN_DATA 6
#define PIN_LED 13
#define PIN_SD_CS 10
#define FRAME_IDENTIFIER_BITS 15
#define MAX_CHAR_PER_LINE 500

int lastState = LOW;
int highCount = 0;
unsigned long lastCheckMicros = 0;
const unsigned long intervalMicros = 8200;

String outStr = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_DATA, INPUT);
  pinMode(PIN_LED, OUTPUT);

  Serial.print("Initializing SD card...");
  if(!SD.begin(PIN_SD_CS)){
    Serial.println("initialization failed.");
    while(true);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long now = micros();

  if(now - lastCheckMicros > 8200){
    lastCheckMicros = now;

    int state = digitalRead(PIN_DATA);
    if(state){
      highCount++;
    }else{
      if(lastState && highCount >= FRAME_IDENTIFIER_BITS){
        printAndReset();
      }
      highCount = 0;
    }
    lastState = state;

    outStr += state;
    // Serial.print(state);
    digitalWrite(PIN_LED, state);

    if(outStr.length() >= MAX_CHAR_PER_LINE){
      printAndReset();
    }
  }

}

void printAndReset(){
  File dataFile = SD.open("ecu_log.txt", FILE_WRITE);
  if(dataFile){
    dataFile.println(outStr);
    dataFile.close();
  }else{
    Serial.println("error opening datalog file.");
  }

  Serial.println(outStr);
  outStr = "";
  lastState = LOW;
}