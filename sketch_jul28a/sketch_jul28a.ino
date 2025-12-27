#define BUZZER_ATTIVO_PIN 8
#define BUZZER_PASSIVO_PIN 9

// Note musicali (4ª e 5ª ottava)
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659

// Funzione per generare toni manuali (buzzer passivo)
void manualTone(int pin, unsigned int freq, unsigned long duration) {
  unsigned long delayVal = 1000000 / (freq * 2);
  unsigned long cycles = (freq * duration) / 1000;
  for (unsigned long i = 0; i < cycles; i++) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(delayVal);
    digitalWrite(pin, LOW);
    delayMicroseconds(delayVal);
  }
}

// Funzione beep per buzzer attivo (percussioni)
void beat(unsigned int times, unsigned long duration, unsigned long pause) {
  for (unsigned int i = 0; i < times; i++) {
    digitalWrite(BUZZER_ATTIVO_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_ATTIVO_PIN, LOW);
    delay(pause);
  }
}

void setup() {
  pinMode(BUZZER_ATTIVO_PIN, OUTPUT);
  pinMode(BUZZER_PASSIVO_PIN, OUTPUT);
}

void loop() {
  // Intro percussione
  beat(4, 60, 60);

  // Prima frase
  manualTone(BUZZER_PASSIVO_PIN, NOTE_E5, 250);
  beat(1, 80, 0);
  delay(50);

  manualTone(BUZZER_PASSIVO_PIN, NOTE_D5, 250);
  beat(1, 80, 0);
  delay(50);

  manualTone(BUZZER_PASSIVO_PIN, NOTE_C5, 250);
  beat(2, 40, 40);
  delay(50);

  manualTone(BUZZER_PASSIVO_PIN, NOTE_D5, 400);
  beat(1, 100, 50);
  delay(100);

  // Seconda frase
  manualTone(BUZZER_PASSIVO_PIN, NOTE_E5, 250);
  beat(1, 80, 0);
  delay(50);

  manualTone(BUZZER_PASSIVO_PIN, NOTE_E5, 250);
  beat(1, 80, 0);
  delay(50);

  manualTone(BUZZER_PASSIVO_PIN, NOTE_E5, 400);
  beat(1, 100, 0);
  delay(100);

  // Terza frase
  manualTone(BUZZER_PASSIVO_PIN, NOTE_D5, 250);
  beat(1, 80, 0);
  delay(50);

  manualTone(BUZZER_PASSIVO_PIN, NOTE_D5, 250);
  beat(1, 80, 0);
  delay(50);

  manualTone(BUZZER_PASSIVO_PIN, NOTE_E5, 250);
  beat(1, 80, 0);
  delay(50);

  manualTone(BUZZER_PASSIVO_PIN, NOTE_D5, 250);
  beat(1, 80, 0);
  delay(50);

  manualTone(BUZZER_PASSIVO_PIN, NOTE_C5, 400);
  beat(1, 100, 0);
  delay(100);

  // Pausa lunga
  delay(1500);
}
