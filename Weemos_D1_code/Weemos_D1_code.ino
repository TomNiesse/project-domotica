#include <ESP8266WiFi.h>

//SSID of your network
char ssid[] = "Wee-Fee"; //SSID of your Wi-Fi router
char pass[] = "12345678"; //Password of your Wi-Fi router (12345678)

char response;
char string[900];
int i = 0;

void setup()
{
  byte pi[] = { 192, 168, 15, 106};
  Serial.begin(115200);
  delay(10);

  WiFiClient client;

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


  delay(1000);

  Serial.println("Proberen om connectie te maken met de PI");

  if (client.connect(pi, 1337))
  {
      Serial.println("EY, er is verbinding");
      client.print("BOE");
      while(client.available()!= 1)
      {
          if(client.available())
          {
           response = (char)client.read();
           string[i] = response;
           i ++;
          }
      }
      Serial.print(string);
    }
}


// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
