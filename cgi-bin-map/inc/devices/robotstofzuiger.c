/*
 * Haal de accuspanning op uit de database.
 * Returnt: laatst gemeten accuspanning en -1 bij een fout.
 */
float robotstofzuiger_get_voltage() 
{
	float voltage;
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

	/* Haal de waarde op van de robotstofzuiger */
	sprintf(sql_query, "SELECT DEVICE_CURRENT_VALUE FROM DEVICE WHERE DEVICE_ID = %d", DEVICE_ID_ROBOTSTOFZUIGER);
	if (mysql_query(sql_connection, sql_query)) 
	{
		return -1;
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
	voltage = (float)atof(row[0]);
	mysql_close(sql_connection);
	return voltage;
}
/*
 * Check of de huidige tijd van vandaag zich bevindt in het gestelde bereik voor de dag van de week.
 * Returnt: 1 als de huidige tijd in het bereik valt, 0 als de huidige tijd niet in het bereik valt en -1 als er iets is foutgegaan
 */
int robotstofzuiger_timestate() 
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

	sprintf(sql_query, "SELECT IF(CURTIME() >= (SELECT TIME_START_TIME FROM TIME WHERE TIME_DAY_OF_WEEK = WEEKDAY(CURDATE()) AND DEVICE_ID = %d) AND CURTIME() <= (SELECT TIME_STOP_TIME FROM TIME WHERE TIME_DAY_OF_WEEK = WEEKDAY(CURDATE()) AND DEVICE_ID = %d), 1, 0)", DEVICE_ID_ROBOTSTOFZUIGER, DEVICE_ID_ROBOTSTOFZUIGER);
	if (mysql_query(sql_connection, sql_query)) 
	{
		return -1;
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
/*
 * Check of de huidige tijd zich binnen het ingesteld bereik voor vandaag bevindt (door robotstofzuiger_timestate() aan te roepen) en de ingestelde state in de database en geef zo de status van de robotstofzuiger aan op de website.
 * Returnt: 0 als alles is goedgegaan, -1 als er iets is foutgegaan
 */
int robotstofzuiger_get_state(char* aan_of_uit) 
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
	
	sprintf(sql_query, "SELECT DEVICE_STATE FROM DEVICE WHERE DEVICE_ID = %d", DEVICE_ID_ROBOTSTOFZUIGER);
	if (mysql_query(sql_connection, sql_query)) 
	{
		return -1;
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

	if(strcmp(aan_of_uit, "aan") == 0)
	{
		if(robotstofzuiger_timestate() == 1 || waarde == 1)
		{
			printf("checked");
		}
	}
	else if(strcmp(aan_of_uit, "uit") == 0)
	{
		if(robotstofzuiger_timestate() == 0 && waarde == 0)
		{
			printf("checked");
		}
	}
	return 0;
}
/*
 * Lees de juiste tijd uit de database uit en print deze.
 * Returnt: 0 als alles is goedgegaan, -1 als er iets is foutgegaan
 */
int robotstofzuiger_get_time(int start_of_stop, int dagwaarde) /* 	1 = START		0 = STOP 	dagwaarde = 0-6 */
{ 
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

	/* Haal de tijd voor de robotstofzuiger uit de database (Een laat de seconden weg) */
	if (start_of_stop == 1)
	{
		sprintf(sql_query, "SELECT TIME_FORMAT(TIME_START_TIME, '%s') as TIME_START_TIME FROM TIME WHERE TIME_DAY_OF_WEEK = %d", "%H:%i", dagwaarde);
		if (mysql_query(sql_connection, sql_query)) 
		{
			return -1;
		}
	}
	else if(start_of_stop == 0)
	{
		sprintf(sql_query, "SELECT TIME_FORMAT(TIME_STOP_TIME, '%s') as TIME_STOP_TIME FROM TIME WHERE TIME_DAY_OF_WEEK = %d", "%H:%i", dagwaarde);
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
	mysql_free_result(result);
	mysql_close(sql_connection);
	printf("%s", row[0]);
	return 0;
}
/*
 * Zet de meegegven tijd in de database.
 * Returnt: 0 als alles is goedgegaan, -1 als er iets is foutgegaan
 */
int robotstofzuiger_set_time(int start_of_stop, int dagwaarde, char* new_time)
{
	char sql_query[MAX_QUERY_LENGTH] = {0};
	MYSQL *sql_connection = mysql_init(NULL);

	/* De GET request verandert ':' in '%3A'. Hier wordt er weer een ':' van gemaakt */
	char new_time2[10];
	sprintf(new_time2, "%c%c:%c%c", new_time[0], new_time[1], new_time[5], new_time[6]);


	/* Test de verbinding met MySQL */
    if (sql_connection == NULL) 
	{
        return -1;
    }
    if (mysql_real_connect(sql_connection, MYSQL_HOST, MYSQL_USERNAME, MYSQL_PASSWORD, MYSQL_DATABASE, 0, NULL, 0) == NULL) 
	{
        return -1;
    }

	/* Zet de nieuwe tijd in de database*/
	if (start_of_stop == 1)
	{
		sprintf(sql_query, "UPDATE TIME SET TIME_START_TIME = '%s' WHERE TIME_DAY_OF_WEEK = %d", new_time2, dagwaarde);
		if(mysql_query(sql_connection, sql_query))
		{
			mysql_close(sql_connection);
			return -1;
		}
	}

	else if (start_of_stop == 0)
	{
		sprintf(sql_query, "UPDATE TIME SET TIME_STOP_TIME = '%s' WHERE TIME_DAY_OF_WEEK = %d", new_time2, dagwaarde);
		if(mysql_query(sql_connection, sql_query))
		{
			mysql_close(sql_connection);
			return -1;
		}
	}
	mysql_close(sql_connection);
	return 0;
}
/*
 * Checkt of één van de knoppen voor handmatig bedienen is ingedrukt, als dit het geval is wordt de waarde in de databse geüpdatet.
 * Returnt: 0 als alles is goedgegaan, 1 als de params waarde niet geldig is en -1 als er iets fout is gegaan.
 */
int robotstofzuiger_update_state(char params[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH], char values[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH]) 
{
	char sql_query[MAX_QUERY_LENGTH] = {0};
	MYSQL *sql_connection = mysql_init(NULL);
	int array_index = 0;
	int waarde;

	/* Kijk of één van de knoppen is ingedrukt */
	while(strlen(params[array_index])) {
		if(strcmp(params[array_index], "aan") == 0) 
		{
			/* Er is een nieuwe waarde gevonden, zet deze in de database */
			waarde = atoi(values[array_index]);
		}
		else if(strcmp(params[array_index], "uit") == 0) 
		{
			/* Er is een nieuwe waarde gevonden, zet deze in de database */
			waarde = atoi(values[array_index]);
		}
		else
		{
			return 1;
		}
		array_index++;
	}
	/* Test de verbinding met MySQL */
    if (sql_connection == NULL) 
	{
        return -1;
    }
    if (mysql_real_connect(sql_connection, MYSQL_HOST, MYSQL_USERNAME, MYSQL_PASSWORD, MYSQL_DATABASE, 0, NULL, 0) == NULL) 
	{
        return -1;
    }
	sprintf(sql_query, "UPDATE DEVICE SET DEVICE_STATE = %d WHERE DEVICE_ID = %d", waarde, DEVICE_ID_ROBOTSTOFZUIGER);
	if(mysql_query(sql_connection, sql_query))
	{
		mysql_close(sql_connection);
		return -1;
	}
	mysql_close(sql_connection);
	return 0;
}
/*
 * Checkt de params en values voor nieuwe tijden per dag, als deze aanwezig zijn wordt het doorgestruud naar robotstofzuiger_set_time() functie om het op te slaan in de database.
 * Returnt: 0 als het goed is gegaan en -1 als er iets fout is gegaan.
 */
int robotstofzuiger_parse_time(char params[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH], char values[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH])
{
	int array_index = 0;
	int start_of_stop, dagwaarde;
	char nieuwe_tijd[10];
	
	while(strlen(params[array_index])) 
	{
		if(strcmp(params[array_index], "maandag1") == 0)
		{
			start_of_stop = 1;
			dagwaarde = 0;
		}
		if(strcmp(params[array_index], "maandag0") == 0)
		{
			start_of_stop = 0;
			dagwaarde = 0;
		}
		if(strcmp(params[array_index], "dinsdag1") == 0)
		{
			start_of_stop = 1;
			dagwaarde = 1;
		}
		if(strcmp(params[array_index], "dinsdag0") == 0)
		{
			start_of_stop = 0;
			dagwaarde = 1;
		}
		if(strcmp(params[array_index], "woensdag1") == 0)
		{
			start_of_stop = 1;
			dagwaarde = 2;
		}
		if(strcmp(params[array_index], "woensdag0") == 0)
		{
			start_of_stop = 0;
			dagwaarde = 2;
		}
		if(strcmp(params[array_index], "donderdag1") == 0)
		{
			start_of_stop = 1;
			dagwaarde = 3;
		}
		if(strcmp(params[array_index], "donderdag0") == 0)
		{
			start_of_stop = 0;
			dagwaarde = 3;
		}
		if(strcmp(params[array_index], "vrijdag1") == 0)
		{
			start_of_stop = 1;
			dagwaarde = 4;
		}
		if(strcmp(params[array_index], "vrijdag0") == 0)
		{
			start_of_stop = 0;
			dagwaarde = 4;
		}
		if(strcmp(params[array_index], "zaterdag1") == 0)
		{
			start_of_stop = 1;
			dagwaarde = 5;
		}
		if(strcmp(params[array_index], "zaterdag0") == 0)
		{
			start_of_stop = 0;
			dagwaarde = 5;
		}
		if(strcmp(params[array_index], "zondag1") == 0)
		{
			start_of_stop = 1;
			dagwaarde = 6;
		}
		if(strcmp(params[array_index], "zondag0") == 0)
		{
			start_of_stop = 0;
			dagwaarde = 6;
		}
		/* Als er een nieuwe waarde is wordt deze in de database gezet */
		strcpy(nieuwe_tijd, values[array_index]);
		if(robotstofzuiger_set_time(start_of_stop, dagwaarde, nieuwe_tijd) == -1) 
		{
			return -1;
		}
		array_index++;
	}
	return 0;
}