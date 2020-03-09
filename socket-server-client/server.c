#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>

int main() {
	/* Define vars */
	char alarmEnabled = 0;
	time_t currentTime = 60;
	time_t desiredTime = 1;
	int sockfd, newsockfd, port_number, message_size;	/* File descriptor, file descriptor, port number, size of bytes written to or from the socket */
	socklen_t clilen;									/* Length of client address */
	char buffer = 0;									/* Incoming chars go into this buffer */
	char testResponse = 42;							/* Outgoing chars go into this buffer */
	struct sockaddr_in serv_addr, cli_addr;	/* A sockaddr_in is a structure containing an internet address. This structure is defined in <netinet/in.h> */

	/* Set vars */
	port_number = 1337;									/* Hardcode the port number because it's going to be a "smart" thing. So, it should be dumb as fuck */
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
	} else {
		printf("socket() deed iets goeds\n");
	}

	/* Zorg ervoor dat er ook daadwerkelijk iets met de socket kan gebeuren, door er een adres aan vast te hangen. Een soort label */
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("bind() heeft er geen zin in\n");
		exit(1);
	} else {
		printf("bind() deed gewoon zn werk een keer\n");
	}

	/* Vertel C dat er mogelijk verkeer aan zou kunnen komen, en dat ie maar van 4 plekken tegelijk hoeft te ontvangen */
	while(1)
	{
		listen(sockfd, 4);								/* syntax: listen(<socket file descriptor>, <hoeveel inkomende connecties max>); */

		/* Nu is dan eindelijk het punt aangekomen dat we gaan luisteren naar binnenkomend verkeer, dit gaat met accept() */
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if(newsockfd < 0) {
			printf("accept() == reject(). er ging iets fout\n");
		} else {
			printf("accept() heeft gewoon mijn memes geaccepteerd en doet zn werk\n");
		}

		/* Lees in wat er binnenkomt */
		message_size = read(newsockfd, &buffer, 1);

		if (message_size < 0) {
			printf("de buffer is meer dan leeg\n");
			exit(1);
		}
		printf("Bericht voor u b0ss: %d\n", (int)buffer);
		printf("We zullen even een bericht terug sturen naar de afzender. 1sec\n");

		while(1)
		{
			/* Kijk of het al tijd is voor dat alarmpie */
			if(currentTime >= desiredTime) alarmEnabled = 1;

			message_size = write(newsockfd, &alarmEnabled, 1);
			if (message_size < 0) {
				printf("Verbinding is onverwachts gesloten.\n");
				break;
			} else {
				printf("write() heeft keurig naar socket geschreven\n");
			}
			sleep(1);
		}
	}
	return 0;
}
