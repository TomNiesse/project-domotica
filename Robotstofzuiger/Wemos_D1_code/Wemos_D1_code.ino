#include <ESP8266WiFi.h>

//SSID of your network
char ssid[] = "Wee-Fee"; //SSID van de Wi-Fi router
char pass[] = "test3737"; //Wachtwoord van de Wi-Fi router
byte pi[] = { 192, 168, 1, 103};

bool last_start;
bool last_stop;

char string[20];
char nano_response[10];
float voltage;

int i;

WiFiClient client;

void setup() {

  Serial.begin(9600);
  delay(1000);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }

  delay(5000); //Wacht 5 seconden, omdat de robotstofzuiger daarna klaar is om signalen te ontvangen.

}

void loop() {

  if (client.connect(pi, 1337))
  {
    if (Serial.available())
    {
      i = 0;
      while (Serial.available())
      {
        nano_response[i] = Serial.read();
        i++;
      }
      client.print(nano_response);
    }
    while(!client.available())
    {
      ;
    }
    while (client.available())
    {
      if (readpi("START"))
      {
        if (!last_start)
        {
          Serial.println('r');
        }
        last_start = true;
        last_stop = false;
      }
      else if (readpi("STOP"))
      {
        if (!last_stop)
        {
          Serial.println('p');
        }
        last_stop = true;
        last_start = false;
      }
    }
  }
  delay(500);
  client.stop();
}

/*
 * Ontvang een response van de Raspberry Pi en vergelijk deze met een meegegeven waarde
 * Returnt: 1 als de waardes gelijk zijn en 0 als ze niet gelijk zijn.
 */
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
    return 1;
  }
  return 0;
}
