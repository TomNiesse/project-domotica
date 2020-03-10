#include <ESP8266WiFi.h>

//SSID of your network
char ssid[] = "Wee-Fee"; //SSID of your Wi-Fi router
char pass[] = "12345678"; //Password of your Wi-Fi router (12345678)
byte pi[] = { 192, 168, 15, 100};

char string[20];

WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);

  // Connect to Wi-Fi network
  //Serial.println();
  //Serial.println();
  //Serial.print("Connecting to...");
  //Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.println("Wi-Fi connected successfully");

}

void loop() {
  // put your main code here, to run repeatedly:

   if (client.connect(pi, 1337))
   {
    //Serial.println("EY, er is verbinding");
    client.write("Er is verbinding");
    while (!client.available())
    {
     ;
    }
    while (client.available())
    {  
      if (readpi("START"))
      {
        Serial.println('r');
        client.write("Robotstofzuiger wordt gestart");
      }
      else if (readpi("STOP"))
      {
        Serial.println('p');
        client.write("Robotstofzuiger wordt gestopt");
      }
    }
  }
  delay(500);
  client.stop();
}

int readpi(char* vergelijking)
{
  int i = 0;
  char response;
  char string[20];

  while (client.available())
  {
    response = client.read();
    string[i] = response;
    i++;
  }
  if (strcmp(string, vergelijking) == 0)
  {
    Serial.println(string);
    return 1;
  }
  return 0;
}
