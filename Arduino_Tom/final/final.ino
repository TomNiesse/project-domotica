/* Zorg ervoor dat arduino weet wat een TCP is */
#include <Ethernet.h>
#include <SPI.h>
/* Zorg ervoor dat arduino de temperatuur-sensor herkent */
#include <dht11.h>
dht11 DHT11;
/* Definieer een paar pins */
#define FAN_PIN 9
#define ARDUINO_LED 13
#define DHT11PIN 8
/* Hoe vaak moet de arduino met de server verbinden (in milliseconden) */
#define SERVER_POLL_INTERVAL 10000
#define SERVER_PORT 6969

/* Definieer en initialiseer EthernetClient (waarden) */
byte mac[] = { 0xC0, 0x0F, 0xDE, 0xAD, 0xBE, 0xEF };  /* MAC-adres van arduino */
byte server[] = { 10, 42, 0, 1 };                     /* IP van mijn GNU/Linux-PC, waarop socket draait */
EthernetClient client;

int  desired_temperature = 500;                       /* Maak de init temperatuur heel hoog, zodat de fan niet gaat draaien */

void setup()
{
  /* Verbind met DHCP */
  Ethernet.begin(mac);
  delay(1000);
  /* Alleen voor debug doeleinden opnenen we een Serial */
  Serial.begin(9600);
  /* Zorg ervoor dat we fanspin krijgen als we naar de pin schrijven */
  pinMode(FAN_PIN, OUTPUT);
  pinMode(ARDUINO_LED, OUTPUT);
  /* Praat een eerste keer met de server om de gewenste temperatuur op te halen */
  negotiate_data(read_temperature());
}
 
void loop()
{
  Serial.print("Huidige temperatuur:\t ");
  Serial.println((int) read_temperature());
  Serial.print("Gewenste temperatuur:\t ");
  Serial.println(desired_temperature);
  if((int)read_temperature() <= desired_temperature) {
    digitalWrite(ARDUINO_LED, LOW);
    digitalWrite(FAN_PIN, LOW);
  } else {
    digitalWrite(ARDUINO_LED, HIGH);
    digitalWrite(FAN_PIN, HIGH);
  }

  /* Wacht even met de server aanspreken */
  delay(SERVER_POLL_INTERVAL);
  /* Spreek de server aan */
  negotiate_data(read_temperature());
}

float read_temperature() {
  int chk = DHT11.read(DHT11PIN);
  return (float)DHT11.temperature;
}

void negotiate_data(int sensor_value) {
  int desired_value;
  if (client.connect(server, SERVER_PORT)) {
    Serial.println("Verbonden met server. Overleg vindt nu plaats...");
    client.print((char)sensor_value);
    /* Luister nu of de server wat heeft te zeggen */
    while (!client.available()) {
      ;
    }
    /* Server heeft wat data */
    while (client.available()) {
      int desired_value = client.read();
      if(desired_value != 0 && (int)desired_value != (int)desired_temperature) {
        desired_temperature = desired_value;
        Serial.print("Server heeft de gewenste temperatuur veranderd naar ");
        Serial.print(desired_temperature);
        Serial.println("C");
      }
    }
    /* We zijn voor nu klaar met de server, verbreek de verbinding */
    client.stop();
    Serial.println("Verbinding met server verbroken.");
  } else {
    Serial.println("Er kon niet met server worden verbonden. We proberen het straks nogmaals. Actuator is preventief uitgeschakeld.");
    /* Schakel de actuator uit totdat de server weer te bereiken is door de gewenste temperatuur heel hoog te zetten */
    desired_temperature = 500;
  }
}
