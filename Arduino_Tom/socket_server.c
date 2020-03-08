#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
		/* Define vars */
		int sockfd, newsockfd, port_number, message_size;
		socklen_t clilen;
		char buffer[256];
		struct sockaddr_in serv_addr, cli_addr;

		char   arduino_sensor_data;		/* Initialiseer de data struct van arduino data */
		int    arduino_desired_value = 18;	/* Dit getal wordt door de server naar de arduino teruggestuurd */
		/* Set vars */
		port_number = 6969;
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(port_number);
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		
		clilen = sizeof(cli_addr);

		/* Probeer een socket te openen */
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0) {
				printf("socket() deed het niet\n");
				exit(1);
		}
		
		/* Zorg ervoor dat er ook daadwerkelijk iets met de socket kan gebeuren, door er een adres aan vast te hangen. Een soort label */
		if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
				printf("bind() heeft er geen zin in\n");
				exit(1);
		}

		/* Zorg ervoor dat de server altijd luistert voor inkomend verkeer */
		while(1) {
			/* Vertel C dat er mogelijk verkeer aan zou kunnen komen */
			listen(sockfd, 5);
			
			/* Luister voor binnenkomend verkeer */
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
			if(newsockfd < 0) {
					printf("accept() werkte niet\n");
					exit(1);
			}
		
			/* Maak de buffer leeg met bzero() */
			bzero(buffer,256);
			/* Lees in welke temperatuur er binnenkomt */
			message_size = read(newsockfd, (int*) &arduino_sensor_data, 1);   /* Lees maar 1 byte aan gegevens */
		
			if (message_size < 0) {
					printf("Er ging iets mis tijdens de communicatie, de buffer is minder dan nul.\n");
					exit(1);
			}
			
			/* Doe iets met de sensorwaarde van de arduino */
			printf("Arduino heeft als waarde %dC gestuurd\n", (int)arduino_sensor_data);
			if((int)arduino_sensor_data > arduino_desired_value) {
				/* Temperatuur is hoger dan de gewenste temperatuur */
				printf("Dit is te hoog, we zorgen voor fanspin.\n");
			} else {
				printf("Voor deze temperatuur is er geen fanspin nodig.\n");
			}
			/* Stuur de gewenste temperatuurwaarde terug naar de arduino */
			message_size = write(newsockfd, &arduino_desired_value, sizeof(arduino_desired_value));
			if (message_size < 0) {
				printf("write() werkte niet\n");
				exit(1);
			}
		}
		return 0;
}
