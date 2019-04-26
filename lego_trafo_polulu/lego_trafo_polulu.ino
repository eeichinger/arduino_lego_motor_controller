#include "DualTB9051FTGMotorShield.h"

const int PIN_DIAL = A2;
const int PIN_BTN_STOP = 3;
const int PIN_BTN_START = 5;
const int step_size_start = 5;
const int step_size_stop = 25;

DualTB9051FTGMotorShield md;
int currentSpeed = 0;
int currentStepSize = step_size_start;

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
  pinMode(PIN_BTN_STOP, INPUT_PULLUP);
  pinMode(PIN_BTN_START, INPUT_PULLUP);

  md.init();
  // Uncomment to flip a motor's direction:
  //md.flipM1(true);
  //md.flipM2(true);
  md.enableDrivers();
  delay(1); // wait for drivers to be enabled so fault pins are no longer low
}

bool stopped = true;

void loop()
{
  delay(100);
  stopIfFault();

  int btnStart = digitalRead(PIN_BTN_START);
  if (stopped && btnStart == LOW) {
    stopped = false;
    md.setM1Speed(0);
    currentSpeed = 0;
    currentStepSize = step_size_start;
//    md.enableDrivers();
  }
  int btnStop = digitalRead(PIN_BTN_STOP);
  if (!stopped && btnStop == LOW) {
    stopped = true;
    currentStepSize = step_size_stop;
  }


  int dial_pos = analogRead(PIN_DIAL);
  int desiredSpeed = map(dial_pos, 0, 1023, -400, 400);
  if (stopped || abs(desiredSpeed) < 50) {
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
  Serial.print(stopped);
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
