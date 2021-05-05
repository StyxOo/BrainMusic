/*
 * Date:         21.04.2021
 * Author:       Luis Rothenhäusler
 * Description:  This is to figure out how to resonably convey data via serial connection
 *               A four character string is expected. One char for each command.
 *               Do not send a newline character. Otherwise it will be weird.
 */

void setup() {
  // put your setup code here, to run once:
  // Open a serial connection for communication
  Serial.begin(9600);
}

bool newMessage = false;

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    char commands[4];
    Serial.readBytes(commands, 4);
    Serial.println(commands);
    newMessage = true;
  }

  if(newMessage)
  {
    Serial.println("End of current loop after recieving message");
    newMessage = false;
  }
}
