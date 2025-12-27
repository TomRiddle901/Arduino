#include <IRremote.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define IRPIN 3

#define PWM_IR_FANS 5
#define TACH_IR_FANS 6
#define PWM_DHT_FAN 9
#define TACH_DHT_FAN 10

// --- Oggetti sensori ---
DHT dht(DHTPIN, DHTTYPE);
IRrecv irrecv(IRPIN);
decode_results results;

// --- Variabili controllo ---
int speedIR = 125;        // velocità iniziale ventole IR (0–255)
int speedDHT = 0;         // velocità ventola automatica
unsigned long rpmIR = 0;
unsigned long rpmDHT = 0;
volatile unsigned long pulseCountIR = 0;
volatile unsigned long pulseCountDHT = 0;
unsigned long lastMillis = 0;

void countPulseIR() { pulseCountIR++; }
void countPulseDHT() { pulseCountDHT++; }

void setup() {
  Serial.begin(9600);
  Serial.println("Avvio sistema ventole IR + DHT11");

  dht.begin();
  irrecv.enableIRIn();

  pinMode(PWM_IR_FANS, OUTPUT);
  pinMode(TACH_IR_FANS, INPUT_PULLUP);
  pinMode(PWM_DHT_FAN, OUTPUT);
  pinMode(TACH_DHT_FAN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(TACH_IR_FANS), countPulseIR, FALLING);
  attachInterrupt(digitalPinToInterrupt(TACH_DHT_FAN), countPulseDHT, FALLING);
}

void loop() {
  // --- IR Remote ---
  if (irrecv.decode(&results)) {
    Serial.print("Codice IR: ");
    Serial.println(results.value, HEX);

    if (results.value == 0xE5CFBD7F) { // Freccia su
      speedIR += 20;
      if (speedIR > 255) speedIR = 255;
    }
    else if (results.value == 0xF076C13B) { // Freccia giù
      speedIR -= 20;
      if (speedIR < 0) speedIR = 0;
    }

    irrecv.resume();
  }

  // --- DHT11 ---
  float t = dht.readTemperature();
  if (!isnan(t)) {
    speedDHT = map(t, 25, 50, 80, 255); // da 20°C min a 35°C max
    if (speedDHT < 0) speedDHT = 0;
    if (speedDHT > 255) speedDHT = 255;
  }

  // --- Applica PWM ---
  analogWrite(PWM_IR_FANS, speedIR);
  analogWrite(PWM_DHT_FAN, speedDHT);

  // --- Calcolo RPM ogni secondo ---
  if (millis() - lastMillis >= 1000) {
    noInterrupts();
    rpmIR = pulseCountIR * 30;   // 2 impulsi per giro tipici
    rpmDHT = pulseCountDHT * 30;
    pulseCountIR = 0;
    pulseCountDHT = 0;
    interrupts();

    Serial.print("Temp: ");
    Serial.print(t);
    Serial.print("°C | IR Fans Power: ");
    Serial.print(speedIR);
    Serial.print(" pwm | DHT Fans Power: ");
    Serial.print(speedDHT);
    Serial.println(" pwm");
    lastMillis = millis();
  }
}
