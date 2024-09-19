#define velUp 3
#define velDown 2

int leds[] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4};

int time = 100;

void setup() {
  for (int i = 0; i < 10; i++) {
    pinMode(leds[i], OUTPUT);
  }
  pinMode(velUp, INPUT_PULLUP);
  pinMode(velDown, INPUT_PULLUP);
}

void loop() {
  // Encender LEDs de izquierda a derecha
  for (int i = 0; i < 10; i++) {
    digitalWrite(leds[i], HIGH);
    delay(time);
    digitalWrite(leds[i], LOW);
      CheckButton();
  }
  
  // Encender LEDs de derecha a izquierda
  for (int i = 9; i >= 0; i--) {
    digitalWrite(leds[i], HIGH);
    delay(time);
    digitalWrite(leds[i], LOW);
      CheckButton();
  }
}

  void CheckButton()
  {
    if (!digitalRead(velUp))
      {
    time -= 100;
      }
   if (!digitalRead(velDown))
      {
    time +=100;
      }
  }