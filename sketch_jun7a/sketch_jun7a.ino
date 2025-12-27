const int red = 13;
const int yellow = 12;
const int green = 11;
const int red2 = 10;
const int yellow2 = 9;
const int green2 = 8;
const int red3 = 7;
const int yellow3 = 6;
const int green3 = 5;
const int red4 = 4;
const int yellow4 = 3;
const int green4 = 2;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red2, OUTPUT);
  pinMode(yellow2, OUTPUT);
  pinMode(green2, OUTPUT);
  pinMode(red3, OUTPUT);
  pinMode(yellow3, OUTPUT);
  pinMode(green3, OUTPUT);
  pinMode(red4, OUTPUT);
  pinMode(yellow4, OUTPUT);
  pinMode(green4, OUTPUT);
}

void loop() {
  digitalWrite(red, HIGH);
  digitalWrite(red3, HIGH);
  digitalWrite(red4, HIGH);
  digitalWrite(green2, HIGH);
  delay(5000);

  digitalWrite(green2, LOW);
  digitalWrite(yellow2, HIGH);
  delay(2500);

  digitalWrite(yellow2, LOW);
  digitalWrite(red2, HIGH);
  digitalWrite(red3, LOW);
  digitalWrite(green3, HIGH);
  delay(5000);

  digitalWrite(green3, LOW);
  digitalWrite(yellow3, HIGH);
  delay(2500);

  digitalWrite(yellow3, LOW);
  digitalWrite(red4, LOW);
  digitalWrite(green4, HIGH);
  digitalWrite(red3, HIGH);
  delay(5000);

  digitalWrite(green4, LOW);
  digitalWrite(yellow4, HIGH);
  delay(2500);

  digitalWrite(red, LOW);
  digitalWrite(red4, HIGH);
  digitalWrite(yellow4, LOW);
  digitalWrite(green, HIGH);
  delay(5000);

  digitalWrite(green, LOW);
  digitalWrite(yellow, HIGH);
  delay(2500);

  digitalWrite(yellow, LOW);
  digitalWrite(red, HIGH);
  digitalWrite(red2, LOW);
}
