#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>
#include <mariadb/mysql.h>

#define MYSQL_HOST       "localhost"
#define MYSQL_DATABASE   "domotica"
#define MYSQL_USERNAME   "domotica"
#define MYSQL_PASSWORD   "domotica"
#define MAX_QUERY_LENGTH 512
#define DEVICE_ID_LAMP  3

int lamp_get_desired_value();
int lamp_set_current_value(int new_current_value);

int main() {
	/* Maak een paar variabelen aan die met sockets te maken hebben */
	int server_socket_file_descriptor, client_socket_file_descriptor, bytes_received, bytes_written;
	short unsigned int socket_server_port_number;
	socklen_t clilen;
	char input_buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

	char   arduino_sensor_data;					/* Hier komt de arduino sensor-data in te staan */
	int    arduino_desired_value = lamp_get_desired_value();	/* Hier komt de ingestelde waarde van de database in te staan */

	if(arduino_desired_value == -1) {
		printf("Database error. Kon geen data ophalen.\n");
		exit(1);
        }

	/* Zorg ervoor dat er een IP-adres en poort aan de socket wordt gehangen, zodat bind() er straks wat mee kan */
	socket_server_port_number = 1337;
	bzero((char *) &serv_addr, sizeof(serv_addr)); 			/* verschoont het server address om \0'en naar zijn geheugen address*/
	serv_addr.sin_family = AF_INET; 						/* Moet altijd AF_NET Zijn want IPv4*/
	serv_addr.sin_port = htons(socket_server_port_number);	/* Zet het portnummer in een geheugen address op volgorde van MSB naar LSB*/
	serv_addr.sin_addr.s_addr = INADDR_ANY;					/* Verkrijgt het IP van de machine die probeert te verbinden met de server */
	clilen = sizeof(cli_addr);

	/* Probeer een socket te openen */
	server_socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket_file_descriptor < 0) {
		printf("socket() deed het niet\n");
		exit(1);
	}

	/* Zorg ervoor dat er ook daadwerkelijk iets met de socket kan gebeuren, door er een adres aan vast te hangen. Een soort label */
	if(bind(server_socket_file_descriptor, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("bind() deed het niet\n");
		exit(1);
	}

	/* Zorg ervoor dat de server altijd luistert voor inkomend verkeer */
	while(1) {
		/* Luister voor binnenkomende verbindingen */
		listen(server_socket_file_descriptor, 5);

		/* Er is een apparaat die met de server wil verbinden, accepteer dit verzoek */
		client_socket_file_descriptor = accept(server_socket_file_descriptor, (struct sockaddr *) &cli_addr, &clilen);
		if(client_socket_file_descriptor < 0) {
			printf("accept() deed het niet\n");
			exit(1);
		}

		/* Maak de input_buffer leeg met bzero() */
		bzero(input_buffer,256);
		/* Lees welke waarde er binnenkomt */
		bytes_received = read(client_socket_file_descriptor, (int*) &arduino_sensor_data, 1);
		/* Lees maar 1 byte aan gegevens. Deze waarde is vast, want er komt altijd maar een byte binnen.*/

		if (bytes_received < 0) {
			printf("De input_buffer is minder dan nul in lengte.\n");
			exit(1);
		}

		/* Print wat voor waarde er binnenkomt*/
		printf("Arduino heeft als waarde %dC gestuurd\n", (int)arduino_sensor_data);
		
		/* Zet de nieuwe waarde in de database */
		lamp_set_current_value((int)arduino_sensor_data);
		
		if((int)arduino_sensor_data > arduino_desired_value) {
			/* De lamp stond uit maar is nu aan*/
			printf("De lamp staat nu aan\n");
		} else {
			printf("De lamp stond uit en nog steeds uit.\n");
		}

		/* Stuur de gewenste lampwaarde terug naar de arduino */
		bytes_written = write(client_socket_file_descriptor, &arduino_desired_value, sizeof(arduino_desired_value));
		if (bytes_written < 0) {
			printf("write() deed het niet\n");
			exit(1);
		}
		else{
			printf("Write() heeft zijn werk gedaan!\n");
		}
		/* Haal opnieuw de ingestelde waarde op, voor als de arduino er weer om vraagt */
		arduino_desired_value = lamp_get_desired_value();
		if(arduino_desired_value == -1) {
			printf("Database error. Kon geen data ophalen.\n");
			exit(1);
		}
		else{
			printf("Database heeft succesvol data kunnen ophalen.\n");
		}
	}
	return 0;
}

int lamp_get_desired_value() {
	int desired_value;
	char sql_query[MAX_QUERY_LENGTH] = {0};
	MYSQL *sql_connection = mysql_init(NULL);
	MYSQL_RES *result;
	MYSQL_ROW row;

	/* Test de verbinding met het database*/
	if (sql_connection == NULL) {
		return -1;
	}
	if (mysql_real_connect(sql_connection, MYSQL_HOST, MYSQL_USERNAME, MYSQL_PASSWORD, MYSQL_DATABASE, 0, NULL, 0) == NULL) {
		return -1;
	}

	/* Haal de waarde op van de lamp */
	sprintf(sql_query, "SELECT DEVICE_DESIRED_VALUE FROM DEVICE WHERE DEVICE_ID = %d", DEVICE_ID_LAMP);
	if (mysql_query(sql_connection, sql_query)) {
		return -1;
	}
	result = mysql_store_result(sql_connection);

	/* Kijk of MariaDB data heeft teruggestuurd */
	if (result == NULL) {
		/* Er ging iets fout */
		mysql_close(sql_connection);
		return -1;
	}

	/* We verwachten maar een waarde terug, en dus ook een enkele kolom */
	row = mysql_fetch_row(result);
	desired_value = atoi(row[0]); /*int naar string omzetten */
	mysql_close(sql_connection);
	return desired_value;
}

int lamp_set_current_value(int new_current_value) {
	char sql_query[MAX_QUERY_LENGTH] = {0};
	MYSQL *sql_connection = mysql_init(NULL);

	/* Test de verbinding met MySQL */
        if (sql_connection == NULL) {
                return -1;
        }
        if (mysql_real_connect(sql_connection, MYSQL_HOST, MYSQL_USERNAME, MYSQL_PASSWORD, MYSQL_DATABASE, 0, NULL, 0) == NULL) {
                return -1;
        }

	/* Zet de nieuwe waarde in de database */
	sprintf(sql_query, "UPDATE DEVICE SET DEVICE_CURRENT_VALUE = %d WHERE DEVICE_ID = %d", new_current_value, DEVICE_ID_LAMP);
	if(mysql_query(sql_connection, sql_query))
	{
		mysql_close(sql_connection);
		return -1;
	}
	mysql_close(sql_connection);
	return 0;
}
