#include <dht11.h>
 
dht11 DHT11;
#define DHT11PIN 8 
 
void setup()
{
  Serial.begin(9600);
  Serial.println("Initialisierung...");
}
 
void loop()
{
  int chk = DHT11.read(DHT11PIN);

  Serial.print("Temp: ");
  Serial.print((float)DHT11.temperature, 2);
  Serial.println(" C");

  Serial.print("RelF: ");
  Serial.print((float)DHT11.humidity, 2);
  Serial.println(" %");

  delay(2000);
}
