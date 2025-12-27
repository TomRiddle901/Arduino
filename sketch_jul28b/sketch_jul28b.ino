#include <LiquidCrystal.h>

#define TRIG_PIN 9
#define ECHO_PIN 10
#define BUZZER_ATTIVO 8
#define BUZZER_PASSIVO 6
#define BUTTON_PIN 7

const int DISTANCE_THRESHOLD = 10; // cm
bool buzzerLatched = false;
bool buzzerState = false;
unsigned long lastBeepTime = 0;

// LCD in modalità 8-bit: RS, E, D0-D7
LiquidCrystal lcd(12, 11, 2, 3, 4, 5, A0, A1, A2, A3);

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_ATTIVO, OUTPUT);
  pinMode(BUZZER_PASSIVO, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.print("Sistema pronto");
  delay(1000);
  lcd.clear();
}

long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;
  return duration / 58;
}

void loop() {
  long distance = readDistanceCM();

  // Mostra distanza sempre su riga 2
  lcd.setCursor(0, 1);
  if (distance > 0) {
    lcd.print("Distanza: ");
    lcd.print(distance);
    lcd.print(" cm   ");
  } else {
    lcd.print("Distanza: ??? cm");
  }

  // Se distanza sotto soglia e non è già in allarme
  if (distance > 0 && distance < DISTANCE_THRESHOLD && !buzzerLatched) {
    buzzerLatched = true;
    lcd.setCursor(0, 0);
    lcd.print("ALLARME ATTIVO! ");
    Serial.println("Allarme attivo!");
  }

  // Se in allarme, suona con effetto sirena
  if (buzzerLatched) {
    unsigned long now = millis();
    if (now - lastBeepTime >= 200) {
      buzzerState = !buzzerState;

      // Buzzer attivo ON/OFF
      digitalWrite(BUZZER_ATTIVO, buzzerState);

      // Buzzer passivo alterna due toni
      if (buzzerState) {
        tone(BUZZER_PASSIVO, 1200);  // Tono alto
      } else {
        tone(BUZZER_PASSIVO, 800);   // Tono basso
      }

      lastBeepTime = now;
    }
  } else {
    digitalWrite(BUZZER_ATTIVO, LOW);
    noTone(BUZZER_PASSIVO);

    lcd.setCursor(0, 0);
    lcd.print("Sistema OK      ");
  }

  // Pulsante per disattivare
  if (buzzerLatched && digitalRead(BUTTON_PIN) == LOW) {
    delay(50);
    if (digitalRead(BUTTON_PIN) == LOW) {
      buzzerLatched = false;
      buzzerState = false;
      digitalWrite(BUZZER_ATTIVO, LOW);
      noTone(BUZZER_PASSIVO);
      lcd.setCursor(0, 0);
      lcd.print("Allarme OFF     ");
      Serial.println("Allarme disattivato");

      while (digitalRead(BUTTON_PIN) == LOW);
      delay(100);
    }
  }

  delay(50);
}