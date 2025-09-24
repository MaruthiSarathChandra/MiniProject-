#define LED_PIN 2

void normalBlink(int delayTime) {
  digitalWrite(LED_PIN, HIGH);
  delay(delayTime);
  digitalWrite(LED_PIN, LOW);
  delay(delayTime);
}

void turnOn(){
  digitalWrite(LED_PIN, HIGH);
}
