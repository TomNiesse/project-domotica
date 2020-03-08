#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>

/* Blauwdruk van data die arduino stuurt */
struct sensor_data_t {
  int device_id;          /* Database-waarde "DEVICE_ID" */
  float sensor_value;     /* Database-waarde "DEVICE_CUR_VALUE" */
};

int main() {
		/* Define vars */
		int sockfd, newsockfd, port_number, message_size;	/* File descriptor, file descriptor, port number, size of bytes written to or from the socket */
		socklen_t clilen;									/* Length of client address */
		char buffer[256];									/* Incoming chars go into this buffer */
		struct sockaddr_in serv_addr, cli_addr;				/* A sockaddr_in is a structure containing an internet address. This structure is defined in <netinet/in.h> */





		char   arduino_sensor_data;								/* Initialiseer de data struct van arduino data */
		int    arduino_desired_value = 18;						/* Dit getal wordt door de server naar de arduino teruggestuurd */




		/* Set vars */
		port_number = 6969;									/* Hardcode the port number because it's going to be a "smart" thing. So, it should be dumb as fuck */
		bzero((char *) &serv_addr, sizeof(serv_addr));		/* Clear the server address by writing all \0's to it's memory address */
		serv_addr.sin_family = AF_INET;						/* Should always be AF_INET */
		serv_addr.sin_port = htons(port_number);
		serv_addr.sin_addr.s_addr = INADDR_ANY;				/* There is this symbolic constant called INADDR_ANY that gets the machine's IP address */
		
		clilen = sizeof(cli_addr);

		/* Probeer een socket te openen */
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0) {									/* returnwaarde van -1 == foute boel */
				printf("socket() heeft er geen zin in\n");
				exit(1);
		}
		
		/* Zorg ervoor dat er ook daadwerkelijk iets met de socket kan gebeuren, door er een adres aan vast te hangen. Een soort label */
		if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
				printf("bind() heeft er geen zin in\n");
				exit(1);
		}

		/* Zorg ervoor dat de server altijd luistert voor inkomend verkeer */
		while(1) {
			/* Vertel C dat er mogelijk verkeer aan zou kunnen komen, en dat ie maar van 4 plekken tegelijk hoeft te ontvangen */
			listen(sockfd, 4);								/* syntax: listen(<socket file descriptor>, <hoeveel inkomende connecties max>); */
			
			/* Nu is dan eindelijk het punt aangekomen dat we gaan luisteren naar binnenkomend verkeer, dit gaat met accept() */
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
			if(newsockfd < 0) {
					printf("accept() werkte niet\n");
					exit(1);
			}
		
			/* Maak de buffer leeg */
			bzero(buffer,256);
			/* Lees in wat er binnenkomt */
			message_size = read(newsockfd, (int*) &arduino_sensor_data, 1);   /* Lees niet meer dan de grootte van de struct, om buffer overflows te voorkomen */
		
			if (message_size < 0) {
					printf("de buffer is meer dan leeg\n");
					exit(1);
			}
			
			printf("Arduino heeft als waarde %d gestuurd\n", (int)arduino_sensor_data);
		    if((int) arduino_sensor_data > arduino_desired_value) {
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
