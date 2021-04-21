/*
 * Date:         21.04.2021
 * Author:       Luis Rothenhäusler
 * Description:  This program repeats messages which have been received via serial connetion, back to said connection
 */

void setup() {
  // put your setup code here, to run once:
  // Open a serial connection for communication
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    Serial.println(Serial.readString());
  }
}
