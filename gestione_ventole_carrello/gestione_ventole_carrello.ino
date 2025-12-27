#include <IRremote.h>
#include <DHT.h>

// Definizione pin per sensore di tempertura e IR
#define DHTPIN 2
#define DHTTYPE DHT11
#define IRPIN 3

// Definizione pin ventole
#define PWM_IR_FANS 5
#define TACH_IR_FANS 6
#define PWM_DHT_FAN 9
#define TACH_DHT_FAN 10

// Oggetti sensori
DHT dht(DHTPIN, DHTTYPE);
IRrecv irrecv(IRPIN);
decode_results results;

// Variabili di controllo
int speedIR = 120; // Velocità iniziale ventole IR
int speedDHT = 0; // Velocità iniziale ventola automatica
unsigned long rpmIR = 0;
unsigned long rpmDHT = 0;
volatile unsigned long pulseCountIR = 0;
volatile unsigned long pulseCountDHT = 0;
unsigned long lastMillis = 0;

void countPulseIR(){
  pulseCountIR++;
}

void countPulseDHT(){
  pulseCountDHT++;
}

void setup(){
  Serial.begin(9600);
  Serial.println("Avvio sistema di raffreddamento del carrello in corso...");
  delay(1000);

  dht.begin();
  irrecv.enableIRIn();

  pinMode(PWM_IR_FANS, OUTPUT);
  pinMode(TACH_IR_FANS, INPUT_PULLUP);
  pinMode(PWM_DHT_FAN, OUTPUT);
  pinMode(TACH_DHT_FAN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(TACH_IR_FANS), countPulseIR, FALLING);
  attachInterrupt(digitalPinToInterrupt(TACH_DHT_FAN), countPulseDHT, FALLING);

  // Test iniziale delle ventole
  Serial.println("Test delle venttole in corso...");
  analogWrite(PWM_IR_FANS, 255);
  analogWrite(PWM_DHT_FAN, 255);

  for (int i = 10; i < 10; i--){
    Serial.println(i);
    delay(1000);
  }
}

void loop(){
  // IR Remote
  if (irrecv.decode(&results)){
    Serial.print("Codice IR: ");
    Serial.println(results.value, HEX);

    if (results.value == 0xE5CFBD7F){
      speedIR += 20;

      if (speedIR > 255){
        speedIR = 255;
      }
    } else if (results.value == 0xF076C13B){
      speedIR -= 20;

      if (speedIR < 0){
        speedIR = 0;
      }
    }
    irrecv.resume();
  }

  // DHT 11
  float t = dht.readTemperature();
  if (!isnan(t)){
    speedDHT = map(t, 30, 60, 80, 255); // da 30 a 60 gradi (>=60 gradi ventola sempre al massimo)
    if (speedDHT < 0){
      speedDHT = 0;
    }
    if (speedDHT > 255){
      speedDHT = 255;
    }

    // Applicazione velocità
    analogWrite(PWM_IR_FANS, speedIR);
    analogWrite(PWM_DHT_FAN, speedDHT);

    // Calcolo e stampa RPM
    if (millis() - lastMillis >= 100){
      noInterrupts();
      rpmIR = pulseCountIR * 30; // 2 impulsi per giro
      rpmDHT = pulseCountDHT * 30;
      pulseCountIR = 0;
      pulseCountDHT = 0;
      interrupts();

      Serial.print("Temp: ");
      Serial.print(t);
      Serial.print("°C | IR Fans Speed: ");
      Serial.print(speedIR);
      Serial.print(" pwm | DHT Fans Speed: ");
      Serial.print(speedDHT);
      Serial.println(" pwm");
      lastMillis = millis();
    }
  }
}