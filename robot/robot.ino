#include <Servo.h> 

int mano_pin = 5;
int cabeza_pin = 3;
int pos;

int ldr_pin = A0;
int ldr_value;
int ldr_umbral = 130;
int ultrasonico_trigger_pin = 8;
int ultrasonico_eco_pin = 9;
long ultrasonico_value;
long tiempo;
int ultrasonico_umbral = 45;
int ultrasonico_last;
int led = 13;
int atencion = 0;
int moneda_status = 0;

int chan1 = 144;
int chan2 = 145;

int cabeza_status = 0;
Servo mano;
Servo cabeza;

#define CW   1800
#define STOP 1550
#define CCW  1000

void abrir_mano()
{
  mano.write(200);

}

void cerrar_mano()
{
  mano.write(15);

}

void subir_cabeza()
{
  if (cabeza_status == 0)
  {
    delay(100);
    cabeza.writeMicroseconds(CCW);
  for(pos = 0; pos < 190; pos++)
  {
    cabeza.writeMicroseconds(CW-pos);
    delay(3);
  }
  cabeza.writeMicroseconds(STOP);
  cabeza_status = 1;
  }
}

void bajar_cabeza()
{
  if (cabeza_status)
  {
  cabeza.writeMicroseconds(CCW);
  //delay(100);
  for(pos = 0; pos < 190; pos++)
  {
    cabeza.writeMicroseconds(CCW+pos);
    delay(1);
  }
  cabeza.writeMicroseconds(STOP);
  cabeza_status = 0; 
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
  if(moneda_status == 0){
    moneda_status = 1;
    delay(200);
    abrir_mano();
    //subir_cabeza();
    midi_note(chan1,1,transform_range(ldr_value, 1023));
    delay(1000);
    cerrar_mano();
    //bajar_cabeza();   
  }
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
  int result = map(x, 0, max, 0, 127);
  //Serial.print("result!! ");
  //Serial.println(result);
  return result;
}
int transform_range(int x, int max)
{
  int result = map(x, 0, max, 0, 127);
  //Serial.print("result!! ");
  //Serial.println(result); 
  return result;
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
    moneda_status = 0;
    //midi_note(chan1,0,transform_range(ldr_value, 1023));
  }

  if(ultrasonico_value < 170)
  {
    atencion = 1;
  }
  if(ultrasonico_value < ultrasonico_umbral)
  {
    subir_cabeza();
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
    delay(400);
    bajar_cabeza();
  }
  midi_note(chan2, atencion, transform_range(ultrasonico_value, 170));
}
