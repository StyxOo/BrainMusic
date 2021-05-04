/*
 * Date:         30.04.2021
 * Author:       Luis Rothenhäusler
 * Description:  Plays a buzzer sound depending on the input value recieved. 
 *               The input should be between 0 and 100 
 * Circuit at:   https://www.tinkercad.com/things/96g1CW7JzCZ
 */

#define BUZZER_PIN 13
#define TONE_TIMEOUT 500
#define FREQUENCY_MIN 110 // This is the starting frequency for the buzzer. 110 is a.
#define FREQUENCY_MAX 440 // 440 is also a, but 2 octaves higher.

void setup() {
  // put your setup code here, to run once:
  // Open a serial connection for communication
  Serial.begin(9600);

  tone(BUZZER_PIN, 110, 2000);
}

unsigned long lastIn;

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    int value = Serial.read(); // It is expected, that input arrives in a single byte. Therefore only a single character gets send to the serial port
    int mappedValue = map(value, 0, 100, FREQUENCY_MIN, FREQUENCY_MAX);
    Serial.println(value);
    tone(BUZZER_PIN, mappedValue);
    lastIn = millis();
  } else if(millis() > lastIn + TONE_TIMEOUT) {
    noTone(BUZZER_PIN);
  }
}
