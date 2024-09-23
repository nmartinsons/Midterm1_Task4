#include <Arduino.h>

// TASK4

#define LED1 9  // LED1
#define LED2 10 // LED2
#define LED3 11 // LED3
#define A 2     // Increase brightness
#define B 3     // Decrease brightness
#define C 4     // Select LED

int selectedLED = LED1; // LED1 is selected first when program is started
// Intially all LEDs have 0 brightness
int brightnessLED1 = 0; // Brightness for LED1
int brightnessLED2 = 0; // Brightness for LED2
int brightnessLED3 = 0; // Brightness for LED3
int increment = 10;     // Amount to increase or decrease brightness by

unsigned long lastDebounceTimeA = 0;
unsigned long lastDebounceTimeB = 0;
unsigned long lastDebounceTimeC = 0;
unsigned long debounceDelay = 50; // Debounce delay

// Button states, buttons not pressed
int lastButtonStateA = LOW;
int lastButtonStateB = LOW;
int lastButtonStateC = LOW;

// Function to decrease brightness
void decrease()
{
  switch (selectedLED)
  {
  case LED1:
    if (brightnessLED1 > 0)
      brightnessLED1 -= increment;
    break;
  case LED2:
    if (brightnessLED2 > 0)
      brightnessLED2 -= increment;
    break;
  case LED3:
    if (brightnessLED3 > 0)
      brightnessLED3 -= increment;
    break;
  }
}

// Function to increase brightness
// Max brightness is 255
void increase()
{
  switch (selectedLED)
  {
  case LED1:
    if (brightnessLED1 < 255)
      brightnessLED1 += increment;
    break;
  case LED2:
    if (brightnessLED2 < 255)
      brightnessLED2 += increment;
    break;
  case LED3:
    if (brightnessLED3 < 255)
      brightnessLED3 += increment;
    break;
  }
}

// Function to display selected LED
void blinkLED(int pin)
{
  analogWrite(pin, 255);
  delay(100);
  analogWrite(pin, 0);
  delay(100);

  // Determine the previous brightness and restores it
  int previousBrightness = 0;
  if (pin == LED1)
  {
    previousBrightness = brightnessLED1;
  }
  else if (pin == LED2)
  {
    previousBrightness = brightnessLED2;
  }
  else if (pin == LED3)
  {
    previousBrightness = brightnessLED3;
  }

  // Restores previous brightness
  analogWrite(pin, previousBrightness);
}

// Function to select chosen LED
void select()
{
  if (selectedLED == LED1)
  {
    selectedLED = LED2;
    blinkLED(LED2);
  }
  else if (selectedLED == LED2)
  {
    selectedLED = LED3;
    blinkLED(LED3);
  }
  else
  {
    selectedLED = LED1;
    blinkLED(LED1);
  }
}

void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(A, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(B, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(C, INPUT_PULLUP); // Use internal pull-up resistor

  // Initial LED states, intially all OFF
  analogWrite(LED1, brightnessLED1);
  analogWrite(LED2, brightnessLED2);
  analogWrite(LED3, brightnessLED3);
}

void loop()
{
  unsigned long currentMillis = millis();

  // Read button states
  int readA = digitalRead(A);
  int readB = digitalRead(B);
  int readC = digitalRead(C);

  // If button A, then increase brightness
  if (readA == LOW && lastButtonStateA == HIGH && (currentMillis - lastDebounceTimeA) > debounceDelay)
  {
    lastDebounceTimeA = currentMillis;
    increase();
  }
  lastButtonStateA = readA;

  // If button B, then decrease brightness
  if (readB == LOW && lastButtonStateB == HIGH && (currentMillis - lastDebounceTimeB) > debounceDelay)
  {
    lastDebounceTimeB = currentMillis;
    decrease();
  }
  lastButtonStateB = readB;

  // If button C, then select wanted LED
  if (readC == LOW && lastButtonStateC == HIGH && (currentMillis - lastDebounceTimeC) > debounceDelay)
  {
    lastDebounceTimeC = currentMillis;
    select();
  }
  lastButtonStateC = readC;

  // Writes corresponding brightness to LEDs
  analogWrite(LED1, brightnessLED1);
  analogWrite(LED2, brightnessLED2);
  analogWrite(LED3, brightnessLED3);
}
