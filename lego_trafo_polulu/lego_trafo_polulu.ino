#include "DualTB9051FTGMotorShield.h"

const int PIN_DIAL = A2;
const int PIN_BTN_STOP = 3;
const int PIN_BTN_START = 5;

DualTB9051FTGMotorShield md;
int currentSpeed = 0;

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

void setMotorSpeed(int currentSpeed) {
  
}

void loop()
{
  delay(50);
  stopIfFault();

  int btnStart = digitalRead(PIN_BTN_START);
  if (stopped && btnStart == LOW) {
    stopped = false;
    md.setM1Speed(0);
    currentSpeed = 0;
//    md.enableDrivers();
  }
  int btnStop = digitalRead(PIN_BTN_STOP);
  if (!stopped && btnStop == LOW) {
    stopped = true;
//    while (abs(currentSpeed) > 50) {
//      int dir = currentSpeed/abs(currentSpeed);
//      currentSpeed = currentSpeed - dir * 10;
//      md.setM1Speed(currentSpeed);
//    }    
//    currentSpeed = 0;
//    md.setM1Speed(currentSpeed);
//    delay(10);
//    md.disableDrivers();
  }


  int dial_pos = analogRead(PIN_DIAL);
  int desiredSpeed = map(dial_pos, 0, 1023, -400, 400);
  if (stopped || abs(desiredSpeed) < 50) {
    desiredSpeed = 0;
  }
//  if (stopped) {
//    Serial.println("Stopped");
//    return;
//  }

  int diff = desiredSpeed - currentSpeed;
  int incr_step = 10 * diff / abs(diff);
  currentSpeed = currentSpeed + incr_step;
  if ( abs(diff) < 50) {
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

  //  for (int i = 0; i <= 400; i++)
  //  {
  //    md.setM1Speed(i);
  //    stopIfFault();
  //    if (abs(i)%200 == 100)
  //    {
  //      Serial.print("M1 current: ");
  //      Serial.println(md.getM1CurrentMilliamps());
  //    }
  //    delay(2);
  //  }

  //  for (int i = 400; i >= -400; i--)
  //  {
  //    md.setM1Speed(i);
  //    stopIfFault();
  //    if (abs(i)%200 == 100)
  //    {
  //      Serial.print("M1 current: ");
  //      Serial.println(md.getM1CurrentMilliamps());
  //    }
  //    delay(2);
  //  }
  //
  //  for (int i = -400; i <= 0; i++)
  //  {
  //    md.setM1Speed(i);
  //    stopIfFault();
  //    if (abs(i)%200 == 100)
  //    {
  //      Serial.print("M1 current: ");
  //      Serial.println(md.getM1CurrentMilliamps());
  //    }
  //    delay(2);
  //  }
  //
  //  for (int i = 0; i <= 400; i++)
  //  {
  //    md.setM2Speed(i);
  //    stopIfFault();
  //    if (abs(i)%200 == 100)
  //    {
  //      Serial.print("M2 current: ");
  //      Serial.println(md.getM2CurrentMilliamps());
  //    }
  //    delay(2);
  //  }
  //
  //  for (int i = 400; i >= -400; i--)
  //  {
  //    md.setM2Speed(i);
  //    stopIfFault();
  //    if (abs(i)%200 == 100)
  //    {
  //      Serial.print("M2 current: ");
  //      Serial.println(md.getM2CurrentMilliamps());
  //    }
  //    delay(2);
  //  }
  //
  //  for (int i = -400; i <= 0; i++)
  //  {
  //    md.setM2Speed(i);
  //    stopIfFault();
  //    if (abs(i)%200 == 100)
  //    {
  //      Serial.print("M2 current: ");
  //      Serial.println(md.getM2CurrentMilliamps());
  //    }
  //    delay(2);
  //  }
  //  delay(5000);
  //  md.disableDrivers();
  //  delay(500);
}
