#include "DualTB9051FTGMotorShield.h"

const int PIN_DIAL = A2;
const int PIN_LED_STARTED = 11;
const int PIN_LED_STOPPED = 13;
const int PIN_BUZZER = A3;
const int PIN_BTN_STOP = 3;
const int PIN_BTN_START = 5;
const int step_size_start = 5;
const int step_size_stop = 25;

DualTB9051FTGMotorShield md;
int currentSpeed = 0;
int currentStepSize = step_size_start;
bool isStopped = true;


void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    md.disableDrivers();
    while (1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    md.disableDrivers();
    while (1);
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Dual TB9051FTG Motor Shield");
  pinMode(PIN_DIAL, INPUT);
  pinMode(PIN_LED_STARTED, OUTPUT);
  pinMode(PIN_LED_STOPPED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_LED_STARTED, LOW);
  digitalWrite(PIN_LED_STOPPED, HIGH);
  pinMode(PIN_BTN_STOP, INPUT_PULLUP);
  pinMode(PIN_BTN_START, INPUT_PULLUP);

  md.init();
  // Uncomment to flip a motor's direction:
  //md.flipM1(true);
  //md.flipM2(true);
  md.enableDrivers();
  delay(1); // wait for drivers to be enabled so fault pins are no longer low
}

void loop()
{
  delay(5);
  stopIfFault();

  int btnStart = digitalRead(PIN_BTN_START);
  if (isStopped && btnStart == LOW) {
    isStopped = false;
    md.setM1Speed(0);
    currentSpeed = 0;
    currentStepSize = step_size_start;
    digitalWrite(PIN_LED_STARTED, HIGH);
    digitalWrite(PIN_LED_STOPPED, LOW);
    noTone(PIN_BUZZER);
    tone(PIN_BUZZER, 440, 2000);
    delay(1000);
    //    md.enableDrivers();
  }
  int btnStop = digitalRead(PIN_BTN_STOP);
  if (!isStopped && btnStop == LOW) {
    isStopped = true;
    currentStepSize = step_size_stop;
    digitalWrite(PIN_LED_STARTED, LOW);
    digitalWrite(PIN_LED_STOPPED, HIGH);
    noTone(PIN_BUZZER);
    tone(PIN_BUZZER, 220, 750);
  }


  int dial_pos = analogRead(PIN_DIAL);
  int desiredSpeed = map(dial_pos, 0, 1023, -400, 400);
  if (isStopped || abs(desiredSpeed) < 50) {
    desiredSpeed = 0;
  }

  int diff = desiredSpeed - currentSpeed;
  int incr_step = currentStepSize * diff / abs(diff);
  currentSpeed = currentSpeed + incr_step;
  if ( abs(diff) < currentStepSize) {
    currentSpeed = desiredSpeed;
  }
  md.setM1Speed(currentSpeed);

  Serial.print("Stopped: ");
  Serial.print(isStopped);
  Serial.print(", Start: ");
  Serial.print(btnStart);
  Serial.print(", Stop: ");
  Serial.print(btnStop);
  Serial.print(", M1 desired speed: ");
  Serial.print(desiredSpeed);
  Serial.print(", M1 current speed: ");
  Serial.print(currentSpeed);
  Serial.print(", M1 current: ");
  Serial.println(md.getM1CurrentMilliamps());
}
