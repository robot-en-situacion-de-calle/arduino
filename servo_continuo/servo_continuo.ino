#include <Servo.h>

#define SERVO_PIN  9

#define CW   1900
#define STOP 1550
#define CCW  1000

Servo myServo;

void setup()
{
    Serial.begin(57600);
    myServo.attach(SERVO_PIN);
    myServo.writeMicroseconds(STOP);
}

void loop()
{
   while (Serial.available() > 0) {
   int salida = Serial.parseInt();
   myServo.writeMicroseconds(salida);
        
   }
    
}


