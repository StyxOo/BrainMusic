#include <LiquidCrystal.h>
#include <Servo.h>
#include <Stepper.h>
#define ENTER_BUTTON_PIN 4
#define UP_BUTTON_PIN 3
#define DOWN_BUTTON_PIN 2
#define SERVO_E_PIN 6
#define SERVO_A_PIN 7
#define SERVO_D_PIN 8
#define SERVO_G_PIN 9

LiquidCrystal lcd = LiquidCrystal(A0, A1, A2, A3, A4, A5);
char num[4];
char minMaxString[17];

unsigned long lastPress = 0;
unsigned long buttonDelay = 200;
unsigned long splashTimeout = 10000;
bool splashScreenActive = true;

int enterButtonStateLastTick = 1;
int upButtonStateLastTick = 1;
int downButtonStateLastTick = 1;

int servoValueIndex = 0;
int servoValues [8] = {0,180,0,180,0,180,0,180};

Servo servos[4];
unsigned long lastWrites[4] = {0,0,0,0};
unsigned long writeTimeOut = 500;
int currentServoIndex;

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
const int rolePerMinute = 17;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm


Stepper stepperA(stepsPerRevolution, 10,12,11,13);

void writeServo(int index, float value)
{
  unsigned long t = millis();
  if(lastWrites[index] + writeTimeOut < t)
  {
    int pin = SERVO_E_PIN;
    if (index == 1)
    {
      pin = SERVO_A_PIN;
    }
    else if (index == 2)
    {
      pin = SERVO_D_PIN;
    }
    else if (index == 3)
    {
      pin = SERVO_G_PIN;
    }
    servos[index].attach(pin);
  }
  servos[index].write(value);
  lastWrites[index] = t;
}

void detachServos()
{
  unsigned long t = millis();
  for (int i = 0; i < 4; i++)
  {
    if(lastWrites[i] + writeTimeOut < t)
    {
      servos[i].detach();
    }
  }
}

void showSplashScreen() {
  lcd.setCursor(0, 0);
  lcd.print("   Welcome to   ");
  lcd.setCursor(0, 1);
  lcd.print("---BRAINMUSIC---");
  splashScreenActive = true;
}

void updateUI()
{
  int enterButtonState = digitalRead(ENTER_BUTTON_PIN);
  int upButtonState = digitalRead(UP_BUTTON_PIN);
  int downButtonState = digitalRead(DOWN_BUTTON_PIN);
  unsigned long time = millis();
  //Servo current;

  if(lastPress + buttonDelay < time)
  {
    if(enterButtonState < enterButtonStateLastTick)
    {
      if(splashScreenActive)
      {
        lcd.setCursor(0, 0);
        lcd.print("Servo String -X-");
        splashScreenActive = false;
      } 
      else
      {
        servoValueIndex = (servoValueIndex + 1) % 8;
      }

      lcd.setCursor(14, 0);
      if(servoValueIndex < 2)
      {
        lcd.print("E");
        currentServoIndex = 0;
      }
      else if(servoValueIndex < 4)
      {
        lcd.print("A");
        currentServoIndex = 1;
      }
      else if(servoValueIndex < 6)
      {
        lcd.print("D");
        currentServoIndex = 2;
      }
      else
      {
        lcd.print("G");
        currentServoIndex = 3;
      }

      if(servoValueIndex % 2 == 0)
      {
        sprintf(minMaxString, ">Min:%3d Max:%3d", servoValues[servoValueIndex], servoValues[servoValueIndex + 1]);
      } 
      else
      {
        sprintf(minMaxString, " Min:%3d>Max:%3d", servoValues[servoValueIndex - 1], servoValues[servoValueIndex]);
      }
      lcd.setCursor(0,1);
      lcd.print(minMaxString);

      lastPress = time;
    }
    else if(!splashScreenActive && upButtonState < upButtonStateLastTick)
    {
      int value = servoValues[servoValueIndex] + 1;
    
      if(servoValueIndex % 2 == 0)
      {
        if(value >= servoValues[servoValueIndex + 1])
        {
          value--;
        }
      
        lcd.setCursor(5, 1);
      } 
      else
      {
        if(value > 180)
        {
          value = 180;
        }

        lcd.setCursor(13, 1);
      }
    
      servoValues[servoValueIndex] = value;
      //servos[currentServoIndex].write(value);
      writeServo(currentServoIndex, value);
      sprintf(num, "%3d", value);
      lcd.print(num);

      lastPress = time;
    }
    else if(!splashScreenActive && downButtonState < downButtonStateLastTick)
    {
      int value = servoValues[servoValueIndex] - 1;
      if (value < 0)
      {
        value = 0;
      }

      if(servoValueIndex % 2 == 0)
      {
        if (value < 0)
        {
          value = 0;
        }
 
        lcd.setCursor(5, 1);
      } 
      else
      {
        if(value <= servoValues[servoValueIndex - 1])
        {
          value++;
        }
     
        lcd.setCursor(13, 1);
      }
    
      servoValues[servoValueIndex] = value;
      //servos[currentServoIndex].write(value);
      writeServo(currentServoIndex, value);
      sprintf(num, "%3d", value);
      lcd.print(num);

      lastPress = time;
    }
    else if(!splashScreenActive && lastPress + splashTimeout < time)
    {
      showSplashScreen();
    }
  }


  enterButtonStateLastTick = enterButtonState;
  upButtonStateLastTick = upButtonState;
  downButtonStateLastTick = downButtonState;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   
  // LCD pins
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  // Buttons
  pinMode(ENTER_BUTTON_PIN, INPUT_PULLUP);
  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
  

  lcd.begin(16, 2);
  showSplashScreen();
  for (int i = 0; i < 4; i++)
  {
    writeServo(i, 90);
  }

  stepperA.setSpeed(rolePerMinute);
}

void loop() {
  // put your main code here, to run repeatedly:

  updateUI();
  stepperA.step(-1);
  detachServos();

  if(splashScreenActive)
  {
    if(Serial.available() > 0)
    {
      char buffer[4];
      Serial.readBytes(buffer, 4);
      for(int i = 0; i < 4; i++)
      {
        int value = map(buffer[i], 0, 100, servoValues[i*2], servoValues[i*2 + 1]);
        writeServo(i, value);
      }
      Serial.println(buffer);
    }
  }


/*
  servoD.write(0);
  stepperA.step(-stepsPerRevolution);
  servoD.write(20);
  stepperA.step(-stepsPerRevolution);
  servoD.write(40);
  stepperA.step(-stepsPerRevolution);
  servoD.write(60);
  stepperA.step(-stepsPerRevolution);
  servoD.write(80);
  stepperA.step(-stepsPerRevolution);
  servoD.write(100);
  stepperA.step(-stepsPerRevolution);
  servoD.write(120);
  stepperA.step(-stepsPerRevolution);
  */
}
