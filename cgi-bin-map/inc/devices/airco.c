/*
 * Haal huidige temperatuur-waarde uit database op
 * Returnt: Laatst gemeten daadwerkelijke temperatuur, of -1 bij een fout
 */
int airco_get_current_value() {
	int current_value;
	char sql_query[MAX_QUERY_LENGTH] = {0};
	MYSQL *sql_connection = mysql_init(NULL);
	MYSQL_RES *result;
	MYSQL_ROW row;

	/* Test de verbinding met MySQL */
	if (sql_connection == NULL) {
		return -1;
	}
	if (mysql_real_connect(sql_connection, MYSQL_HOST, MYSQL_USERNAME, MYSQL_PASSWORD, MYSQL_DATABASE, 0, NULL, 0) == NULL) {
		return -1;
	}

	/* Haal de waarde op van de airco */
	sprintf(sql_query, "SELECT DEVICE_CUR_VALUE FROM DEVICE WHERE DEVICE_ID = %d", DEVICE_ID_AIRCO);
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
	current_value = atoi(row[0]);
	mysql_close(sql_connection);
	return current_value;
}

/*
 * Zet een nieuwe gemeten waarde in de database
 * Returnt: 0 als alles is goedgegaan, 1 als er iets is foutgegaan
 */
int airco_set_current_value(int new_current_value) {
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
	sprintf(sql_query, "UPDATE DEVICE SET DEVICE_CUR_VALUE = %d WHERE DEVICE_ID = %d", new_current_value, DEVICE_ID_AIRCO);
	if(mysql_query(sql_connection, sql_query))
	{
		mysql_close(sql_connection);
		return -1;
	}
	mysql_close(sql_connection);
	return 0;
}

/*
 * Haal de gewenste temperatuur-waarde uit database op
 * Returnt: De ingestelde temperatuur, of -1 bij een fout
 */
int airco_get_desired_value() {
	int desired_value;
	char sql_query[MAX_QUERY_LENGTH] = {0};
	MYSQL *sql_connection = mysql_init(NULL);
	MYSQL_RES *result;
	MYSQL_ROW row;

	/* Test de verbinding met MySQL */
	if (sql_connection == NULL) {
		return -1;
	}
	if (mysql_real_connect(sql_connection, MYSQL_HOST, MYSQL_USERNAME, MYSQL_PASSWORD, MYSQL_DATABASE, 0, NULL, 0) == NULL) {
		return -1;
	}

	/* Haal de waarde op van de airco */
	sprintf(sql_query, "SELECT DEVICE_DIS_VALUE FROM DEVICE WHERE DEVICE_ID = %d", DEVICE_ID_AIRCO);
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
	desired_value = atoi(row[0]);
	mysql_close(sql_connection);
	return desired_value;
}

/*
 * Zet een nieuwe gemeten waarde in de database
 * Returnt: 0 als alles is goedgegaan, 1 als er iets is foutgegaan
 */
int airco_set_desired_value(int new_desired_value) {
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
	sprintf(sql_query, "UPDATE DEVICE SET DEVICE_DIS_VALUE = %d WHERE DEVICE_ID = %d", new_desired_value, DEVICE_ID_AIRCO);
	if(mysql_query(sql_connection, sql_query))
	{
		mysql_close(sql_connection);
		return -1;
	}
	mysql_close(sql_connection);
	return 0;
}
