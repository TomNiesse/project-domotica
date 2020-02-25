# Wat is dit?

Dit is de "Domme Onveilige Metadata Negotiator In Netwerkomgevingen".<br>
Domnin in het kort.<br>
<img style="width: 50px; height: auto;" src="https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fpl.scdn.co%2Fimages%2Fpl%2Fdefault%2F4d609d0686d6ae9dc71a7c2e9c4fa5072615f3b1&f=1&nofb=1" alt="Afbeelding van domnin buiten werktijden"><br>
<i>Figuur 1: Domnin buiten werktijden of als er geen webverzoeken binnenkomen</i>


# Wat doet het?

Domnin is een socket server die al het dataverkeer tussen 4 arduino's en een mariadb database afhandelt.
Domnin zal altijd inkomende requests afhandelen, maar gaat nooit proactief op zoek naar werk.
Arduino's moeten dus zelf steeds aan Domnin vragen om (nieuwe) gegevens.
