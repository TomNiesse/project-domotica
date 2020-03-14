#include <Ethernet.h>
#include <SPI.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //MAC adres dat de Arduino aanneemt.
byte server[] = { 192, 168, 15, 101 }; //IP adres waarmee je verbind. (In dit geval het IP adres van de Raspberry Pi.)

EthernetClient client; //Creeert een client waarmee je verbinding kan maken met een gespecifieerde internet IP adres en port.

void setup()
{
  Ethernet.begin(mac); //Initieert de ethernet library en netwerkt instellingen.
  Serial.begin(9600); //Stelt de data rate voor serial data transmission in in bits per second.
  
  delay(1000);
  
  Serial.println("connecting..."); //Print data naar de serial port in de vorm van ASCII text.

  if (client.connect(server, 1337)) { //Verbind met een gespecifieert IP adres en port. client.connect returned een 1 (int) als de connectie succesvol is.
    Serial.println("connected");
    client.println((char)42);
  } else {
    Serial.println("connection failed");
  }
}

void loop()
{ 
  if (client.available()) {//Krijgt een client die met de server verbonden is en die data verkrijgbaar heeft om te lezen.
    //Serial.println("Er is text beschikbaar.");
    char c = client.read(); //Leest de volgende byte die ontvangen is van de server waarmee de client verbonden is.
    //Serial.println((int)c);

    if ((int)c == 1) //Zet buzzer aan.
    Serial.println("TEEERRRRIIINGGGGGG!");
    else
    Serial.println("Buzzer is uitgezet.");
    
  }

  if (!client.connected()) {
    Serial.println("Reconnecting...");
    client.connect(server, 1337);
    client.println((char)42);
    delay(100);
  }
}
