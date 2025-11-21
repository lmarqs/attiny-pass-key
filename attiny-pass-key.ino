#include <DigiKeyboard.h>

#ifndef ATTINY_PASS_KEY_PASSWORD
#define ATTINY_PASS_KEY_PASSWORD "default_password"
#endif

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  DigiKeyboard.sendKeyStroke(0);
  DigiKeyboard.delay(1000); 
  DigiKeyboard.println(ATTINY_PASS_KEY_PASSWORD);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}