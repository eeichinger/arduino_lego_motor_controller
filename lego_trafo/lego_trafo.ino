
const int PIN_DIAL = A0; // Generating interrupts using CLK signal
const int PIN_M_EN = 9;
const int PIN_M_DIRA = 8;
const int PIN_M_DIRB = 7;

void setup ()  {
  pinMode(PIN_DIAL, INPUT);
  pinMode(PIN_M_EN, OUTPUT);
  pinMode(PIN_M_DIRA, OUTPUT);
  pinMode(PIN_M_DIRB, OUTPUT);
  digitalWrite(PIN_M_EN, LOW);
  digitalWrite(PIN_M_DIRA, LOW);
  digitalWrite(PIN_M_DIRB, HIGH);
  
  Serial.begin(9600);
}

int val = -1;
int motorSpeed = 0;

void loop ()  {
  delay(50);
  val = analogRead(PIN_DIAL);
  motorSpeed = map(val, 0, 1023, 0, 255);
  Serial.println(motorSpeed);
  SetMotorControl(0, 1, motorSpeed);  
}

void SetMotorControl(int motorId, int motorDirection, int motorSpeed)
/*
  L293 logic:    EN1,2   1A    2A
               H       H     L    Motor turns left  (Forward; motorDirection == 1)
               H       L     H    Motor turns right (Reverse; motorDirection == 0)

  Motor speed:   PWM signal on EN1,2 (490 Hz; digital output value 0..255 for motorSpeed)
*/
{
//  if (motorDirection == 1)               //Forward
//  {
//    digitalWrite(PIN_M_DIRA, 0);
//    digitalWrite(PIN_M_DIRB, 1);
//  }
//  else if (motorDirection == -1)               //Backward
//  {
//    digitalWrite(PIN_M_DIRA, 1);
//    digitalWrite(PIN_M_DIRB, 0);
//  }
//  else
//  {
//    // break
//    digitalWrite(PIN_M_DIRA, 0);
//    digitalWrite(PIN_M_DIRB, 0);
//  }
  analogWrite(PIN_M_EN, motorSpeed);
}
