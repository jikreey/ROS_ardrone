#define led 13  // built-in LED
#define on 2
#define off 6

int ByteReceived;

void setup()
{
  Serial.begin(9600);
    pinMode(led, OUTPUT);  
    pinMode(on, OUTPUT);  
    pinMode(off, OUTPUT);
    digitalWrite(on,HIGH);
    digitalWrite(off,HIGH);  
}

void loop()  
{
  if (Serial.available() > 0)
  {
    ByteReceived = Serial.read();
    if(ByteReceived == 'H')
    {
      digitalWrite(led,HIGH);
      digitalWrite(on,LOW);
      delay(250);
      digitalWrite(on,HIGH);
      Serial.print(" PORTAL NAIK ");
    }
    
    if(ByteReceived == 'L')
    {
      digitalWrite(led,LOW);
      digitalWrite(off,LOW);
      delay(250);
      digitalWrite(off,HIGH);
      Serial.print(" PORTAL TURUN ");
    }
    
    Serial.println();    // End the line

  }
}
