
byte Chn1 = 144;
byte Chn2 = 145;

void setup(){
  Serial.begin(9600);
 
  
}

void loop()
{
//note2=note2+1;
 MIDImessage(Chn1, 50,100);//turn note on
  
delay(600);//wait 200ms until triggering next note

  MIDImessage(Chn1, 50,0);//turn note off (note on with velocity 0)
  delay(600);//wait 200ms until triggering next note

}




void MIDImessage(byte command, byte data1, byte data2)
{
  Serial.write(command);
  Serial.write(data1);
  Serial.write(data2);
}

