#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <mariadb/mysql.h>

#define MYSQL_HOST "localhost"
#define MYSQL_USERNAME "root"
#define MYSQL_PASSWORD "1234"
#define MYSQL_DATABASE "domotica"
#define MYSQL_MANAGER_USERNAME "domotica"	/* Dit account kan alleen SELECT, INSERT, UPDATE, en DELETE, niks anders. Ook heeft het account alleen maar toegang tot de domotica-database */
#define MYSQL_MANAGER_PASSWORD "domotica" 	/* Zeer origineel en veilig */

#define MAX_QUERY_LENGTH 512

void exec_query(MYSQL*con, char query[]);

int main(int argc, char **argv)
{
	char keuze;
	char sql_query[MAX_QUERY_LENGTH] = {0};

	MYSQL *con = mysql_init(NULL);

	/* Kijk of mysql_init() zn werk wel heeft gedaan */
	if (con == NULL) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	/* Kijk of er een verbinding mogelijk is met de database */
	if (mysql_real_connect(con, MYSQL_HOST, MYSQL_USERNAME, MYSQL_PASSWORD, NULL, 0, NULL, 0) == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}

	printf("De query zal opnieuw een database:(Domotica) verwijderen en aanmaken. \n");
	printf("Gaat u hiermee akkoord? [j/n] \t");

	scanf("%c", &keuze);


	if(tolower(keuze) != 'j')
	{
		printf("U gaat niet akkoord en bent gestopt.");
		exit(1);
	}

	printf("Ik ga nu aan het werk!\n");
	/* Verwijdert de database */
	if(mysql_query(con, "DROP DATABASE domotica"))
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		/*mysql_close(con);
		  exit(1);*/
		/* De database kan niet worden verwijderd, maar we moeten wel gewoon doorgaan */
		printf("Database bestaat niet, dus er valt weinig te verwijderen. We gaan wel gewoon door.\n");
	}
	printf("De database is foetsie\n");
	/* Maak de database aan */
	if (mysql_query(con, "CREATE DATABASE domotica"))
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}
	printf("De database is aangemaakt\n");
	con = mysql_init(NULL);
	if (mysql_real_connect(con, MYSQL_HOST, MYSQL_USERNAME, MYSQL_PASSWORD, MYSQL_DATABASE, 0, NULL, 0) == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}

	/* Tabellen aanmaken */

	if (mysql_query(con,"CREATE TABLE DEVICE(DEVICE_ID INT AUTO_INCREMENT, " 
				"DEVICE_NAME VARCHAR(255) NOT NULL, "
				"DEVICE_CURRENT_VALUE FLOAT(11,2) NULL, "
				"DEVICE_DESIRED_VALUE INT NULL, primary key(DEVICE_ID), "
				"DEVICE_STATE INT NULL)"))
	{
		printf("%s", mysql_error(con));
		mysql_close(con);
		exit(1);
	}

	printf("De DEVICE tabel is nu aangemaakt met de querie statements\n");

	if (mysql_query(con,"CREATE TABLE TIME(TIME_ID INT AUTO_INCREMENT, " 
				"TIME_START_TIME TIME NOT NULL, "
				"TIME_STOP_TIME TIME NOT NULL, "
				"TIME_DAY_OF_WEEK INT NOT NULL, DEVICE_ID INT NOT NULL, primary key(TIME_ID),"
				"FOREIGN KEY (DEVICE_ID) REFERENCES DEVICE(DEVICE_ID))"))
	{
		printf("%s", mysql_error(con));
		mysql_close(con);
		exit(1);
	}
	printf("De TIME tabel is nu aangemaakt met de querie statements\n");


	/* Tabellen vullen*/

	if (mysql_query(con,"INSERT INTO DEVICE(DEVICE_ID, DEVICE_NAME, DEVICE_CURRENT_VALUE, DEVICE_DESIRED_VALUE, DEVICE_STATE)"
						"VALUES" 
						"(1, 'Robotstofzuiger', 0, NULL, 0),"
						"(4, 'Airco', -1, 19, NULL)"))
	{
		printf("%s", mysql_error(con));
		mysql_close(con);
		exit(1);
	}
	printf("De DEVICE tabel is nu gevuld met waarden\n");


	if (mysql_query(con,"INSERT INTO TIME(TIME_ID, TIME_START_TIME, TIME_STOP_TIME, TIME_DAY_OF_WEEK, DEVICE_ID)"
						"VALUES" 
						"(1, '00:00', '00:00', 0, 1),"
						"(2, '00:00', '00:00', 1, 1),"
						"(3, '00:00', '00:00', 2, 1),"
						"(4, '00:00', '00:00', 3, 1),"
						"(5, '00:00', '00:00', 4, 1),"
						"(6, '00:00', '00:00', 5, 1),"
						"(7, '00:00', '00:00', 6, 1)"))
	{
		printf("%s", mysql_error(con));
		mysql_close(con);
		exit(1);
	}
	printf("De TIME tabel is nu gevuld met waarden\n");



	/*
	 * Maak een gebruiker aan met minimale rechten, voor de veiligheid
	 */
	printf("[ BUSY ]\tGebruiker %s aanmaken...", MYSQL_USERNAME);
	sprintf(sql_query, "CREATE USER '%s'@'localhost' IDENTIFIED BY '%s'", MYSQL_MANAGER_USERNAME, MYSQL_MANAGER_PASSWORD);
	if(mysql_query(con, sql_query)) {
		printf("\r[ FAIL ]\tGebruiker %s aanmaken...\n\t\tFoutmelding van MariaDB: %s\n\t\tWaarschijnlijk bestaat de gebruiker al.\n", MYSQL_MANAGER_USERNAME, mysql_error(con));
	} else {
		printf("\r[  OK  ]\tGebruiker %s is aangemaakt!\n", MYSQL_MANAGER_USERNAME);
	}

	/*
	 * De gebruiker bestaat nu als het goed is. Geef de gebruiker een paar rechten: SELECT, UPDATE, INSERT en DELETE op alle tabellen in MYSQL_DATABASE
	 */
	printf("[ BUSY ]\tGebruiker %s minimale rechten geven...", MYSQL_USERNAME);
	bzero(sql_query, MAX_QUERY_LENGTH); /* Verwijder de laatste query compleet uit het geheugen */
	sprintf(sql_query, "GRANT SELECT, INSERT, UPDATE, DELETE ON %s.* TO '%s'@'localhost';", MYSQL_DATABASE, MYSQL_MANAGER_USERNAME);
	if(mysql_query(con, sql_query)) {
		printf("\r[ FAIL ]\tGebruiker %s minimale rechten geven...\n\t\tFoutmelding van MariaDB: %s\n", MYSQL_MANAGER_USERNAME, mysql_error(con));
	} else {
		printf("\r[  OK  ]\tGebruiker %s heeft de minimale vereiste rechte gekregen!\n", MYSQL_MANAGER_USERNAME);
	}


	/*
	 * Alles is klaar, sluit de verbinding met MariaDB
	 */
	mysql_close(con);
	exit(0);
}
/*
   void exec_query(MYSQL*con, char query[]) {
   if (mysql_query(con, query))
   {
   printf("%s", mysql_error(con));
   mysql_close(con);
   exit(1);
   }
   printf("DEVICE Table zou nu moeten bestaan\n");
   } */
