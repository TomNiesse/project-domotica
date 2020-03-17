#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>
#include <unistd.h>

#include "commands.c"
#include "parse_webrequest.c"
#include "html.c"

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
