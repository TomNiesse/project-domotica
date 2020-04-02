#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <strings.h>
#include <mariadb/mysql.h>

int main()
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

	char *server = "localhost";
	char *user = "root";
	char *password = "1234"; /*password is not set in this example*/
	char *database = "domotica";

	conn = mysql_init(NULL);

	/* Connect to database */
	if (!mysql_real_connect(conn, server,
				user, password, database, 0, NULL, 0))
	{
		printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
		return 0;
	}

	/* Execute SQL query to fetch all table names.*/
	if (mysql_query(conn, "show tables"))
	{
		printf("Failed to execute quesry. Error: %s\n", mysql_error(conn));
		return 0;
	}

	res = mysql_use_result(conn);

	/* Output table name */
	printf("MySQL Tables in domotica database:\n");
	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s \n", row[0]);

	/* free results */
	mysql_free_result(res);

	/* send SQL query */
	if (mysql_query(conn, "select * from TIME"))
	{
		printf("Failed to execute quesry. Error: %s\n", mysql_error(conn));
		return 0;
	}

	res = mysql_store_result(conn);
	if (res == NULL)
	{
		return 0;
	}

	int columns = mysql_num_fields(res);

	int i = 0;

	printf("Entries in the table TIME:\n");
	while(row = mysql_fetch_row(res))
	{
		for (i = 0; i < columns; i++)
		{
			printf("%s ", row[i] ? row[i] : "NULL");
		}
		printf("\n");
	}

	mysql_free_result(res);
	mysql_close(conn);

	return 1;
}
