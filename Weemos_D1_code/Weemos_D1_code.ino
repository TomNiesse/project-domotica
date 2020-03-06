#include <ESP8266WiFi.h>

//SSID of your network
char ssid[] = "Wee-Fee"; //SSID of your Wi-Fi router
char pass[] = "12345678"; //Password of your Wi-Fi router (12345678)
byte pi[] = { 192, 168, 15, 100};

char string[20];

WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to...");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected successfully");
  pinMode(LED_BUILTIN, OUTPUT);

  if (client.connect(pi, 1337))
  {
    Serial.println("EY, er is verbinding");
    client.write("BOE!!");
  }
  else
  {
    Serial.println("Ik kan geen verbinding maken met de server");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (client.available())
  {
    if (readpi("START"))
    {
      Serial.println("Ik zal de robotstofzuiger starten");
      client.write("Robotstofzuiger wordt gestart");
    }
    else if (readpi("STOP"))
    {
      Serial.println("Ik zal de robotstofzuiger stoppen");
      client.write("Robotstofzuiger wordt gestopt");
    }
  }
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
