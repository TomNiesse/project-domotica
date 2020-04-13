#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h> 
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>
#include <mariadb/mysql.h>

#define MYSQL_HOST "localhost"
#define MYSQL_USERNAME "domotica"
#define MYSQL_PASSWORD "domotica"
#define MYSQL_DATABASE "domotica"
#define MAX_QUERY_LENGTH 512
#define DEVICE_ID_ROBOTSTOFZUIGER 1

int robotstofzuiger_set_voltage(float voltage);
int robotstofzuiger_check_timestate();
int robotstofzuiger_get_state();
void readArduino(char* buffer, int newsockfd);
void writeArduino(char* bericht, char* response_buffer, int newsockfd);

int main() 
{
	int database, tijd;
	float voltage;
	char antwoord[4];
	int sockfd, newsockfd, stoppen, port_number, message_size;
	socklen_t clilen;											
	char buffer[256];											
	char response_buffer[256];									
	struct sockaddr_in serv_addr, cli_addr;						

	
	port_number = 1337;									
	bzero((char *) &serv_addr, sizeof(serv_addr));		
	serv_addr.sin_family = AF_INET;						
	serv_addr.sin_port = htons(port_number);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	clilen = sizeof(cli_addr);

	/* Probeer een socket te openen */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) 
	{	
		/* returnwaarde van -1 == foute boel */
		printf("socket() is niet gelukt\n");
		exit(1);
	} 
	else 
	{
		printf("socket() was succesvol\n");
	}

	/* Zorg ervoor dat er ook daadwerkelijk iets met de socket kan gebeuren, door er een adres aan vast te hangen. Een soort label */
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	{
		printf("bind() is niet gelukt\n");
		exit(1);
	} 
	else 
	{
		printf("bind() was succesvol\n");
	}
	while (1)
	{
		printf("Aan het luisteren\n");
		/* Vertel C dat er mogelijk verkeer aan zou kunnen komen, en dat ie maar van 4 plekken tegelijk hoeft te ontvangen */

		listen(sockfd, 5);								/* syntax: listen(<socket file descriptor>, <hoeveel inkomende connecties max>); */

		/* Nu is dan eindelijk het punt aangekomen dat we gaan luisteren naar binnenkomend verkeer, dit gaat met accept() */
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if(newsockfd < 0) 
		{
			printf("accept() == reject(). er ging iets fout\n");
			exit(1);
		} 
		else 
		{
			printf("accept() is gelukt\n");
		}

		database = robotstofzuiger_get_state("database");
		tijd = robotstofzuiger_get_state("tijd");

		/* Als de huidige tijd binnen het ingestelde bereik voor vandaag valt of er is via de knoppen op de website aangegeven dat de robotstofzuiger aan moet, dan gaat hij aan */

		if (database == 1 || tijd == 1)
		{
			writeArduino("START", response_buffer, newsockfd);
			/* voor debuggen */
			printf("START GESCHREVEN\n");
		}
		else if (database == 0 || tijd == 0)
		{
			writeArduino("STOP", response_buffer, newsockfd);
			/* voor debuggen */
			printf("STOP GESCHREVEN\n");
		}

		readArduino(buffer, newsockfd);
		voltage = atof(buffer);

		if (voltage != 0)
		{
			robotstofzuiger_set_voltage(voltage);
		}
		/* voor debuggen */
		printf("De waarde van de Arduino = %.2f\n", voltage);

	}
	return 0;
}
/*
 * Het meegegeven voltage wordt in de database gezet.
 * Returnt: 0 en -1 als er iets is foutgegaan
 */
int robotstofzuiger_set_voltage(float voltage) 
{
	char sql_query[MAX_QUERY_LENGTH] = {0};
	MYSQL *sql_connection = mysql_init(NULL);

	/* Test de verbinding met MySQL */
    if (sql_connection == NULL) 
	{
         return -1;
    }
    if (mysql_real_connect(sql_connection, MYSQL_HOST, MYSQL_USERNAME, MYSQL_PASSWORD, MYSQL_DATABASE, 0, NULL, 0) == NULL) 
	{
        return -1;
    }

	/* Zet de nieuwe waarde in de database */
	sprintf(sql_query, "UPDATE DEVICE SET DEVICE_CURRENT_VALUE = %.2f WHERE DEVICE_ID = %d", voltage, DEVICE_ID_ROBOTSTOFZUIGER);
	if(mysql_query(sql_connection, sql_query))
	{
		mysql_close(sql_connection);
		return -1;
	}
	mysql_close(sql_connection);
	return 0;
}
/*
 * Checkt de status in de database of kijkt of de huidige tijd zich tussen het ingestelde tijd voor vandaag bevindt.
 * Returnt: 1 als de robotstofzuiger volgens de meegegeven bron aan zou moeten, 0 als de robotstofzuiger volgens de aangegven bron uit zou moeten zijn en -1 als er iets is foutgegaan.
 */
int robotstofzuiger_get_state(char* tijd_of_database) 
{
	int waarde;
	char sql_query[MAX_QUERY_LENGTH] = {0};
	MYSQL *sql_connection = mysql_init(NULL);
	MYSQL_RES *result;
	MYSQL_ROW row;

	/* Test de verbinding met MySQL */
	if (sql_connection == NULL) 
	{
		return -1;
	}
	if (mysql_real_connect(sql_connection, MYSQL_HOST, MYSQL_USERNAME, MYSQL_PASSWORD, MYSQL_DATABASE, 0, NULL, 0) == NULL) 
	{
		return -1;
	}

	if (strcmp(tijd_of_database, "tijd") == 0)
	{
		sprintf(sql_query, "SELECT IF(CURTIME() >= (SELECT TIME_START_TIME FROM TIME WHERE TIME_DAY_OF_WEEK = WEEKDAY(CURDATE()) AND DEVICE_ID = %d) AND CURTIME() <= (SELECT TIME_STOP_TIME FROM TIME WHERE TIME_DAY_OF_WEEK = WEEKDAY(CURDATE()) AND DEVICE_ID = %d), 1, 0)", DEVICE_ID_ROBOTSTOFZUIGER, DEVICE_ID_ROBOTSTOFZUIGER);
		if (mysql_query(sql_connection, sql_query)) 
		{
			return -1;
		}
	}
	else if (strcmp(tijd_of_database, "database") == 0)
	{
		sprintf(sql_query, "SELECT DEVICE_STATE FROM DEVICE WHERE DEVICE_ID = %d", DEVICE_ID_ROBOTSTOFZUIGER);
		if (mysql_query(sql_connection, sql_query)) 
		{
			return -1;
		}
	}
	
	result = mysql_store_result(sql_connection);

	/* Kijk of MariaDB data heeft teruggestuurd */
	if (result == NULL) 
	{
		/* Er ging iets fout */
		mysql_close(sql_connection);
		return -1;
	}

	/* We verwachten maar één waarde terug, en dus ook één kolom */
	row = mysql_fetch_row(result);
	waarde = atoi(row[0]);
	mysql_free_result(result);
	mysql_close(sql_connection);
	return waarde;
}

/* Lees van de Arduino */
void readArduino(char* buffer, int newsockfd)
{	
	int message_size;

	fd_set fds;
	struct timeval timeout;

	FD_ZERO(&fds);
	FD_SET(newsockfd, &fds);

	/* Om te voorkomen dat er eeuwig op een response wordt gewacht is er een timeout van 3 seconden */

	timeout.tv_sec = 3; /* timeout in seconden */
	timeout.tv_usec = 0;

	if (select(FD_SETSIZE, &fds, NULL, NULL, &timeout) == 0)
	{
		/* Geen antwoord binnen de tijd, ga dus niet lezen */
	}
	else
	{
		/* Maak de buffer leeg */
		bzero(buffer,256);
		/* Lees in wat er binnenkomt */
		message_size = read(newsockfd, buffer, sizeof(buffer));
		if (message_size < 0) 
		{
			printf("de buffer is meer dan leeg\n");
			exit(1);
		}
	}
}

/* Schrijf naar de Arduino */
void writeArduino(char* bericht, char* response_buffer, int newsockfd)
{
	int message_size;
	strcpy(response_buffer, bericht);
	message_size = write(newsockfd, response_buffer, strlen(response_buffer)+1);
	if (message_size < 0) 
	{
		printf("write() is spontaan dyslect geworden\n");
		exit(1);
	} 
	else 
	{
		printf("write() heeft keurig naar socket geschreven\n");
	}
}
