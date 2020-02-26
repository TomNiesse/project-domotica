#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>
#include <unistd.h>

#include "commands.c"
#include "parse_webrequest.c"
#include "html.c"

/* Include html-server */
/*#include "html.c"*/

int main(int argc, char** argv, char** env) {
	int index = 0;
	char getrequest_buffer[MAX_STRING_LENGTH] = {'\0'};
	/* Create two string srrays for the parsing of get requests */
	char params[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH];
	char values[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH];

	printf("Content-type: text/plain\r\n\n");
	while(env[index] != NULL) {
		printf("%d: %s\n", index, env[index]);
		if(strstr(env[index], "REQUEST_URI") != NULL) {
			printf("GEVIND\n");
			strncpy(getrequest_buffer, env[index], strlen(env[index]));
			printf("%s\n", getrequest_buffer);
			parse_webrequest(getrequest_buffer, params, values);
		}
		index++;
	}
	printf("\nNow for debugging:\n\n");
	for(index = 0; index < MAX_ARRAY_LENGTH; index++) {
		if(strlen(params[index]) != 0) {
			printf("\tPARAM '%s' has VALUE '%s'\n", params[index], values[index]);
		} else {
			printf("\nEnd of get request input\n");
			break;
		}
	}
	return 0;
	printf("Content-type: text/html\r\n\n");
	print_html_page_contents("html_root/index.html");
	return 0;
}
