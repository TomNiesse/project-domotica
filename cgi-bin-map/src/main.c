#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>

#include "commands.c"
#include "html.c"
#include "parse_webrequest.c"

/* Include html-server */
/*#include "html.c"*/

int main(int argc, char** argv, char** env) {
	int index = 0;
	char* post_data;
	printf("Content-type: text/plain\r\n\n");
	while(env[index] != NULL) {
		printf("%d: %s\n", index, env[index]);
		if(strstr(env[index], "CONTENT_LENGTH") != NULL) {
			printf("GEVIND\n");
			/* Content length uitlezen */
			strcpy(content_length_str, env[index]);
			printf("%s\n", content_length_str);
		}
		index++;
	}
	char* test_str;
	test_str = "CONTENT_LENGTH=13";
	test_str += 15; /* Zorg ervoor dat CONTENT_LENGTH= uit de string verdwijnt */

	printf("EEN TEST %s\n", test_str);
	return 0;
	printf("Content-type: text/html\r\n\n");
	print_html_page_contents("html_root/index.html");
	return 0;
}

