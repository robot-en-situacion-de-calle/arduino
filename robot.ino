#include <Servo.h> 
#include <Bounce.h>

// the MIDI channel number to send messages
const int channel = 1;

int mano_pin = 5;
int mano_pos = 0;
int cabeza_pin = 6;
int cabeza_pos = 0;
int pos; //TODO mientras tanto los 2 servos se mueven igual

int ldr_pin = 7;
int ldr_value;
int ldr_umbral = 10; //TODO
int ultrasonico_trigger_pin = 8;
int ultrasonico_eco_pin = 9;
int ultrasonico_value;
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


void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  mano.attach(mano_pin);
  cabeza.attach(cabeza_pin);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)

}

void loop() {
  ldr_value = analogRead(ldr_pin);
  ultrasonico_value = analogRead(ultrasonico_pin);

  if(ldr_value > ldr_umbral)
  {
    moneda();
  }

  if(ultrasonico_value > ultrasonico_umbral)
  {
    
  }  
}
