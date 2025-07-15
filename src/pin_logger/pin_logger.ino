
#define PIN_DATA 0
#define PIN_LED 13
#define FRAME_IDENTIFIER_BITS 15

int lastState = LOW;
int highCount = 0;
unsigned long lastCheckMicros = 0;
const unsigned long intervalMicros = 8200;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_DATA, INPUT);
  pinMode(PIN_LED, OUTPUT);

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
        Serial.println();
      }
      highCount = 0;
    }
    lastState = state;

    Serial.print(state);
    digitalWrite(PIN_LED, state);
  }

}