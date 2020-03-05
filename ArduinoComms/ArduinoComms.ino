#include <Ethernet.h>
#include <SPI.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte server[] = { 172, 217, 17, 110 }; // Youtube's IP address

EthernetClient client;

bool status = 0;

void toggle_led() {
  if(status) {
    status = 0;
    return;
  }
  status = 1;
  return;
}

void setup()
{
  Ethernet.begin(mac);
  Serial.begin(9600);

  pinMode(13, OUTPUT);

  delay(1000);

  Serial.println("connecting...");

  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println("GET /search?q=arduino HTTP/1.0");
    client.println();
  } else {
    Serial.println("connection failed");
  }
}

void loop()
{
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
    toggle_led();
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    for(;;)
      ;
  }
}
