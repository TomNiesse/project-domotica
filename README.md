# Project domotica

## Onderwerp

_We hebben nog een verhaaltje nodig en een lijst aan sensoren en actuatoren_

## TODO

- ~~POST-requests parsen door QUERY_STRING enzo uit te lezen en er een parser voor te schrijven~~ is klaar
- ~~Arduino socket client maken~~ code kan gewoon gejat worden van groepsgenoten, dus imho klaar
- ~~Sensoren werkend krijgen~~
- ~~Actuatoren werkend krijgen~~
- DB beheren met C
- Twan moet zn shit fixen
- Alle docs in deze repo zetten en organiseren

## Hoe werkt alles?
- cgi-bin-map moven naar /var/www/cgi-bin (vergeet de -r flag niet als je cp doet ipv mv)
- socket server, ja gooi die maar letterlijk overal neer, die doet het gewoon (compile hem wel even voor je eigen pc door make uit te voeren + de socket server is niet voor windows bedoeld, ik wil dus geen vragen als "Windows zegt x en y tijdens compilen" zien).

Voor de rest bestaan er aparte docs die in de discord staan
