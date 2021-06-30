#include <LiquidCrystal.h>
#define ENTER_BUTTON_PIN 4
#define UP_BUTTON_PIN 3
#define DOWN_BUTTON_PIN 2

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

void showSplashScreen() {
  lcd.setCursor(0, 0);
  lcd.print("   Welcome to   ");
  lcd.setCursor(0, 1);
  lcd.print("---BRAINMUSIC---");
  splashScreenActive = true;
}

void setup() {
  // put your setup code here, to run once:
   
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
}

void loop() {
  // put your main code here, to run repeatedly:

  int enterButtonState = digitalRead(ENTER_BUTTON_PIN);
  int upButtonState = digitalRead(UP_BUTTON_PIN);
  int downButtonState = digitalRead(DOWN_BUTTON_PIN);
  unsigned long time = millis();

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
      }
      else if(servoValueIndex < 4)
      {
        lcd.print("A");
      }
      else if(servoValueIndex < 6)
      {
        lcd.print("D");
      }
      else
      {
        lcd.print("G");
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
