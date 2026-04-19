#include <DHT.h>

// Definizione pin per sensore di temperatura
#define DHTPIN 2
#define DHTTYPE DHT11

// Definizione pin ventola
#define PWM_DHT_FAN 9
#define TACH_DHT_FAN 10

// Oggetto sensore
DHT dht(DHTPIN, DHTTYPE);

// Variabili
int speedDHT = 0;
unsigned long rpmDHT = 0;
volatile unsigned long pulseCountDHT = 0;
unsigned long lastMillis = 0;

// Conteggio impulsi tachimetrico
void countPulseDHT(){
  pulseCountDHT++;
}

void setup(){
  Serial.begin(9600);
  Serial.println("Avvio sistema di raffreddamento automatico...");
  delay(1000);

  dht.begin();

  pinMode(PWM_DHT_FAN, OUTPUT);
  pinMode(TACH_DHT_FAN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(TACH_DHT_FAN), countPulseDHT, FALLING);

  // Test iniziale ventola
  Serial.println("Test ventola...");
  analogWrite(PWM_DHT_FAN, 255);
  delay(3000);
}

void loop(){
  float t = dht.readTemperature();

  if (!isnan(t)){
    // Mappa temperatura -> velocità ventola
    speedDHT = map(t, 30, 60, 80, 255);

    // Limiti di sicurezza
    if (speedDHT < 0) speedDHT = 0;
    if (speedDHT > 255) speedDHT = 255;

    // Applica velocità
    analogWrite(PWM_DHT_FAN, speedDHT);

    // Calcolo RPM
    if (millis() - lastMillis >= 1000){
      noInterrupts();
      rpmDHT = pulseCountDHT * 30; // 2 impulsi per giro
      pulseCountDHT = 0;
      interrupts();

      Serial.print("Temp: ");
      Serial.print(t);
      Serial.print("°C | Velocità: ");
      Serial.print(speedDHT);
      Serial.print(" PWM | RPM: ");
      Serial.println(rpmDHT);

      lastMillis = millis();
    }
  } else {
    Serial.println("Errore lettura DHT!");
  }
}
