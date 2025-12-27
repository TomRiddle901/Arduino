const int pinRed = 9;
const int pinGreen = 10;
const int pinBlue = 11;
const int buttonRed = 2;
const int buttonGreen = 3;
const int buttonBlue = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);

  pinMode(buttonRed, INPUT_PULLUP);
  pinMode(buttonGreen, INPUT_PULLUP);
  pinMode(buttonBlue, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  bool redPressed = digitalRead(buttonRed) == LOW;
  bool greenPressed = digitalRead(buttonGreen) == LOW;
  bool bluePressed = digitalRead(buttonBlue) == LOW;

  if (redPressed){
    setColor(255, 0, 0);
  } else if (greenPressed){
    setColor(0, 255, 0);
  } else if (bluePressed){
    setColor(0, 0, 255);
  }
}

void setColor(int redVal, int greVal, int blVal){
  analogWrite(pinRed, redVal);
  analogWrite(pinGreen, greVal);
  analogWrite(pinBlue, blVal);
}