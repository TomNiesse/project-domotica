#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

#define h_addr h_addr_list[0] /* dit moet want muh backward compatibility. saus: https://stackoverflow.com/questions/11405819/does-struct-hostent-have-a-field-h-addr */

#define SERVER_PORT 1337
#define SERVER_ADDR "127.0.0.1"

int main() {
		/* Define vars */
		int sockfd;
		int port_number;
		int message_length;
		struct sockaddr_in serv_addr;
		struct hostent *server;
		/*char incomming_message_buffer[256];*/
		char outgoing_message_buffer[256];

		/* Set vars */
		port_number = SERVER_PORT;
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		server = gethostbyname(SERVER_ADDR);

		/* Try to open a socket */
		if(sockfd < 0) {
				printf("socket() heeft er geen zin in\n");
		} else {
				printf("socket() deed iets nuttigs\n");
		}

		/* Check if the server address actually exists (we hardcode this so it should be just fine for now) */
		if (server == NULL) {
				printf("Dit server adres lijkt niet op een goeie jongen\n");
				exit(1);
		} else {
				printf("Server adres lijkt prima in orde\n");
		}

		/* Clear the server_addr variable */
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET; /* Should always be set to AF_INET */
		/* Set server variables */
		bcopy((char *)server->h_addr,
		      (char *)&serv_addr.sin_addr.s_addr,
              server->h_length);
		serv_addr.sin_port = htons(port_number);


		/* Now then, let's try to connect to the server */
		if (connect(sockfd, (const struct sockaddr_in *) &serv_addr, sizeof(serv_addr)) < 0) {
				printf("Oopsie woopsie de server is opeens weggetoverd. Kan niet verbinden\n");
				exit(1);
		} else {
				printf("Verbonden met server");
				/* Even zo een berichie sturen naar de server, 1sec */
				strcpy(outgoing_message_buffer, "Ewa server neefje\0");
				message_length = write(sockfd, outgoing_message_buffer ,strlen(outgoing_message_buffer));
		}
		return 0;
}
