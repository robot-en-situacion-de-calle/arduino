#include <Servo.h> 
#include <Bounce.h>

// the MIDI channel number to send messages
const int channel = 1;

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
int ultrasonico_umbral = 10; //TODO
int led = 13;

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

void midi_note()
{
  usbMIDI.sendNoteOn(60, 99, channel);  // 60 = C4  
}

void moneda()
{
  midi_note();
  swipe_servos();
}

long ultrasonico()
{
  digitalWrite(ultrasonico_trigger_pin, LOW);
  delayMicroseconds(5);
  digitalWrite(ultrasonico_trigger_pin, HIGH);
  delayMicroseconds(10);
  tiempo = pulseIn(ultrasonico_eco_pin, HIGH);
  return tiempo*0.017;
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
}

void loop() {
  ldr_value = analogRead(ldr_pin);
  ultrasonico_value = ultrasonico();
  Serial.print("ldr: ");
  Serial.println(ldr_value);
  Serial.print("ultrasonico: ");
  Serial.println(ultrasonico_value);
  //Serial.println(ultrasonico_value*0.017);
  
  if(ldr_value > ldr_umbral)
  {
    moneda();
  }

  if(ultrasonico_value > ultrasonico_umbral)
  {
    
  }  
  delay(100);
}
