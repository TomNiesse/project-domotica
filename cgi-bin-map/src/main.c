#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>
#include <unistd.h>

/* Definieer de sql-variabelen hier, zodat ze globaal bekend zijn.
 * De gebruiker heeft minimale rechten, dus schade zou beperkt moeten blijven indien er een hancker in komt */
#define MYSQL_HOST       "localhost"
#define MYSQL_DATABASE   "domotica"
#define MYSQL_USERNAME   "domotica"
#define MYSQL_PASSWORD   "domotica"
#define MAX_QUERY_LENGTH 512

#define DEVICE_ID_AIRCO 2 /* Dit moet nog worden veranderd in het eindproduct. Daar moet het id nummer 3 zijn, de laatste van de 4 apparaten */

#include "commands.c"
#include "parse_webrequest.c"
#include "html.c"

#include "devices/airco.c"

int main(int argc, char** argv, char** env) {
	/*
	 * Parse all GET-requests (if any)
	 */
	int index = 0;
	char getrequest_buffer[MAX_STRING_LENGTH] = {'\0'};
	/* Create two string srrays for the parsing of get requests */
	char params[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH];
	char values[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH];
	while(env[index] != NULL) {
		if(strstr(env[index], "REQUEST_URI") != NULL) {
			strncpy(getrequest_buffer, env[index], strlen(env[index]));
			parse_webrequest(getrequest_buffer, params, values);
			break;
		}
		index++;
	}
	/*
	 * Render webpage
	 */
	printf("Content-type: text/html\r\n\n");
	print_html_page_contents("html_root/index.html");
	return 0;
}
