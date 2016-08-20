#include <Servo.h> 
#include <Bounce.h>

int mano_pin = 5;
int mano_pos = 0;
int cabeza_pin = 6;
int cabeza_pos = 0;
int pos; //TODO mientras tanto los 2 servos se mueven igual

int ldr_pin = A0;
int ldr_value;
int ldr_umbral = 10; //TODO
int ultrasonico_trigger_pin = 8;
int ultrasonico_eco_pin = 9;
long ultrasonico_value;
long tiempo;
int ultrasonico_umbral = 45;
int ultrasonico_last;
int led = 13;
int atencion = 0;

Servo mano;
Servo cabeza;

void swipe_servos()
{
  for(pos = 0; pos <= 180; pos++)
  {
    mano.write(pos);
    cabeza.write(pos);
  }
  for(pos = 180; pos>=0; pos--)
  {
    mano.write(pos);
    cabeza.write(pos);
  }
}

void midi_note(int nota, int vel, int chan)
{
  usbMIDI.sendNoteOn(nota, vel, chan);
  usbMIDI.sendNoteOff(nota, 0, chan);
}

void moneda()
{
  midi_note(127,transform_range(ldr_value, 1023),1);
  swipe_servos();
}

long ultrasonico()
{
  digitalWrite(ultrasonico_trigger_pin, LOW);
  delayMicroseconds(5);
  digitalWrite(ultrasonico_trigger_pin, HIGH);
  delayMicroseconds(10);
  tiempo = pulseIn(ultrasonico_eco_pin, HIGH);
  return min(tiempo*0.017, 170);
}

int transform_range(long x,int max)
{
  return (x/max)*127;
}
int transform_range(int x, int max)
{
  return (x/max)*127;
}

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  mano.attach(mano_pin);
  cabeza.attach(cabeza_pin);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  pinMode(ldr_pin, INPUT);
  pinMode(ultrasonico_trigger_pin, OUTPUT);
  pinMode(ultrasonico_eco_pin, INPUT);
  ultrasonico_last = millis();
}

void loop() {
  ldr_value = analogRead(ldr_pin);
  ultrasonico_value = ultrasonico();
  Serial.print("ldr: ");
  Serial.println(ldr_value);
  Serial.print("ultrasonico: ");
  Serial.println(ultrasonico_value);
  //Serial.println(ultrasonico_value*0.017);
  
  if(ldr_value < ldr_umbral)
  {
    moneda();
  }
  else
  {
    midi_note(1,transform_range(ldr_value, 1023),1);
  }

  if(ultrasonico_value < ultrasonico_umbral)
  {
    atencion = 1;
    if (10000 < millis() - ultrasonico_last)
    {
      atencion = 2;
      if (30000 < millis() - ultrasonico_last)
      {
        atencion = 3;
      }
    }
  }
  else
  {
    atencion = 0;
    ultrasonico_last = millis();
  }
  midi_note(atencion,transform_range(ultrasonico_value, 170),2);
  delay(100);
}
