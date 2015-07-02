#define led 13  // built-in LED

int ByteReceived;

void setup()
{
  Serial.begin(9600);
}

void loop()  
{
  if (Serial.available() > 0)
  {
    ByteReceived = Serial.read();
    if(ByteReceived == 'H')
    {
      digitalWrite(led,HIGH);
      Serial.print(" LED ON ");
      delay(1000);
    }
    
    if(ByteReceived == 'L')
    {
      digitalWrite(led,LOW);
      delay(1000);
      Serial.print(" LED OFF");
    }
    
    Serial.println();    // End the line

  }
}
