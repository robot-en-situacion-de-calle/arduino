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

int chan1 = 144;
int chan2 = 145;

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

void MIDImessage(byte command, byte data1, byte data2)
{
  Serial.write(command);
  Serial.write(data1);
  Serial.write(data2);
}

void midi_note(int chan, int nota, int vel)
{
  int temp_millis = millis();
  MIDImessage(chan, nota, vel);
  delay(100);
  MIDImessage(chan, nota, 0);
}

void moneda()
{
  midi_note(chan1,127,transform_range(ldr_value, 1023));
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
  //Serial.print("ldr: ");
  //Serial.println(ldr_value);
  //Serial.print("ultrasonico: ");
  //Serial.println(ultrasonico_value);
  
  if(ldr_value < ldr_umbral)
  {
    moneda();
  }
  else
  {
    midi_note(chan1,0,transform_range(ldr_value, 1023));
  }

  if(ultrasonico_value < 170)
  {
    atencion = 1;
  }
  if(ultrasonico_value < ultrasonico_umbral)
  {
    atencion = 2;
    if (10000 < millis() - ultrasonico_last)
    {
      atencion = 3;
      if (30000 < millis() - ultrasonico_last)
      {
        atencion = 4;
      }
    }
  }
  else
  {
    atencion = 0;
    ultrasonico_last = millis();
  }
  midi_note(chan2, atencion, transform_range(ultrasonico_value, 170));
}
